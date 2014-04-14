/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#include "GfxPch.h"

#include "SkeletonDef.h"
#include "SkeletonAnimationDef.h"
#include "NewBone.h"
#include "BoneMemoryManager.h"
#include "KfTransformArrayMemoryManager.h"

#include "Id.h"

namespace Demi
{
    SkeletonDef::SkeletonDef( const Skeleton *originalSkeleton, float frameRate ) :
        mNumUnusedSlots( 0 ),
        mName( originalSkeleton->getName() )
    {
        mBones.reserve( originalSkeleton->getNumBones() );

        //Clone the bone data
        size_t numDepthLevels = 1;
        Skeleton::ConstBoneIterator itor = originalSkeleton->getBoneIteratorConst();
        while( itor.hasMoreElements() )
        {
            OldBone *bone = itor.getNext();
            size_t parentIndex = -1;
            if( bone->getParent() )
            {
                assert( !bone->getParent() || dynamic_cast<OldBone*>( bone->getParent() ) );
                OldBone *parent = static_cast<OldBone*>( bone->getParent() );
                parentIndex = parent->getHandle();

                size_t tmpDepthLevels = 2;
                OldNode *tmpParent = parent;
                while( (tmpParent = tmpParent->getParent()) )
                    ++tmpDepthLevels;
                numDepthLevels = std::max( numDepthLevels, tmpDepthLevels );
            }

            BoneData boneData( bone->getHandle(), parentIndex, bone->getPosition(),
                                bone->getOrientation(), bone->getScale(),
                                bone->getName(), bone->getInheritOrientation(),
                                bone->getInheritScale() );
            mBoneIndexByName[bone->getName()] = mBones.size();
            mBones.push_back( boneData );
        }

        mBonesPerDepth.resize( numDepthLevels );
        mDepthLevelInfoVec.resize( numDepthLevels );
        for( size_t i=0; i<mBones.size(); ++i )
        {
            size_t currentDepthLevel = 0;
            BoneData const *tmpBone = &mBones[i];
            while( tmpBone->parent != std::numeric_limits<size_t>::max() )
            {
                tmpBone = &mBones[tmpBone->parent];
                ++currentDepthLevel;
            }

            DepthLevelInfo &info = mDepthLevelInfoVec[currentDepthLevel];
            info.firstBoneIndex = std::min( i, info.firstBoneIndex );
            ++info.numBonesInLevel;

            mBonesPerDepth[currentDepthLevel].push_back( i );
        }

        //Clone the animations
        mAnimationDefs.resize( originalSkeleton->getNumAnimations() );
        for( size_t i=0; i<originalSkeleton->getNumAnimations(); ++i )
        {
            mAnimationDefs[i].setName( originalSkeleton->getAnimation( i )->getName() );
            mAnimationDefs[i].build( originalSkeleton, originalSkeleton->getAnimation( i ), frameRate );
        }

        //Create the bones (just like we would for SkeletonInstance)so we can
        //get derived position/rotation/scale and then calculate its inverse
        BoneMemoryManager boneMemoryManager;
        DiVector<Bone> boneNodes( mBones.size(), Bone() );
        auto itDepth = mBonesPerDepth.begin();
        auto enDepth = mBonesPerDepth.end();

        while( itDepth != enDepth )
        {
            auto bonesItor = itDepth->begin();
            auto bonesItorEnd = itDepth->end();

            while( bonesItor != bonesItorEnd )
            {
                Bone *parent = 0;
                size_t parentIdx = mBones[*bonesItor].parent;
                const BoneData &boneData = mBones[*bonesItor];

                if( parentIdx != std::numeric_limits<size_t>::max() )
                    parent = &boneNodes[parentIdx];

                Bone &newBone = boneNodes[*bonesItor];
                newBone._initialize( Id::generateNewId<Bone>(), &boneMemoryManager, parent, 0 );
                
                newBone.setPosition( boneData.vPos );
                newBone.setOrientation( boneData.qRot );
                newBone.setScale( boneData.vScale );
                newBone.setInheritOrientation( boneData.bInheritOrientation );
                newBone.setInheritScale( boneData.bInheritScale );
                newBone.setName( boneData.name );
                newBone.mGlobalIndex = *bonesItor;

                ++bonesItor;
            }

            ++itDepth;
        }

        //Calculate in bulk. Calling getDerivedPositionUpdated & Co like
        //we need would need, has O(N!) complexity.
        for( size_t i=0; i<boneMemoryManager.getNumDepths(); ++i )
        {
            BoneTransform t;
            const size_t numNodes = boneMemoryManager.getFirstNode( t, i );
            Bone::updateAllTransforms( numNodes, t, &ArrayMatrixAf4x3::IDENTITY, 1 );
        }

        {
            size_t numBoneBlocks = getNumberOfBoneBlocks( mDepthLevelInfoVec.size() );

            //Create initial bind pose.
            mBindPose = RawSimdUniquePtr<KfTransform, MEMCATEGORY_ANIMATION>( numBoneBlocks );
            RawSimdUniquePtr<SimpleMatrixAf4x3, MEMCATEGORY_ANIMATION> derivedPosesPtr =
                        RawSimdUniquePtr<SimpleMatrixAf4x3, MEMCATEGORY_ANIMATION>(numBoneBlocks * ARRAY_PACKED_REALS );

            size_t bindPoseIndex = 0;
            KfTransform *bindPose = mBindPose.get();
            SimpleMatrixAf4x3 *derivedPose = derivedPosesPtr.get();

            size_t currentDepthLevel = 0;
            auto bonesItor = mDepthLevelInfoVec.begin();
            auto bonesItorEnd = mDepthLevelInfoVec.end();

            while( bonesItor != bonesItorEnd )
            {
                auto itBoneIdx = mBonesPerDepth[currentDepthLevel].begin();
                auto enBoneIdx = mBonesPerDepth[currentDepthLevel].end();
                while( itBoneIdx != enBoneIdx )
                {
                    const BoneData &boneData = mBones[*itBoneIdx];
                    bindPose->mPosition.setFromVector3( boneData.vPos, bindPoseIndex );
                    bindPose->mOrientation.setFromQuaternion( boneData.qRot, bindPoseIndex );
                    bindPose->mScale.setFromVector3( boneData.vScale, bindPoseIndex );

                    const Bone &derivedBone = boneNodes[*itBoneIdx];
                    derivedPose[bindPoseIndex] = derivedBone._getLocalSpaceTransform();

                    ++bindPoseIndex;
                    if( bindPoseIndex >= ARRAY_PACKED_REALS )
                    {
                        bindPoseIndex = 0;
                        ++bindPose;
                        derivedPose += ARRAY_PACKED_REALS;
                    }

                    ++itBoneIdx;
                }

                if( bonesItor->numBonesInLevel <= (ARRAY_PACKED_REALS >> 1) )
                {
                    //Repeat the slots in patterns to save memory when instances get created
                    //Do the same in SkeletonTrack::_bakeUnusedSlots
                    size_t k=0;
                    for( size_t j=bonesItor->numBonesInLevel; j<ARRAY_PACKED_REALS; ++j )
                    {
                        DiVec3 vTmp;
                        DiQuat qTmp;
                        bindPose->mPosition.getAsVector3( vTmp, k );
                        bindPose->mPosition.setFromVector3( vTmp, j );
                        bindPose->mOrientation.getAsQuaternion( qTmp, k );
                        bindPose->mOrientation.setFromQuaternion( qTmp, j );
                        bindPose->mScale.getAsVector3( vTmp, k );
                        bindPose->mScale.setFromVector3( vTmp, j );

                        derivedPose[j] = derivedPose[k];

                        k = (k+1) % bonesItor->numBonesInLevel;
                    }

                    bindPoseIndex = 0;
                    ++bindPose;
                    derivedPose += ARRAY_PACKED_REALS;
                }
                else if( bindPoseIndex != 0 )
                {
                    //We can't repeat to save memory per instance. But we still need
                    //to create a hole so the next depth level starts in the first slot
                    //of the next block
                    for( ; bindPoseIndex<ARRAY_PACKED_REALS; ++bindPoseIndex )
                    {
                        bindPose->mPosition.setFromVector3( DiVec3::ZERO, bindPoseIndex );
                        bindPose->mOrientation.setFromQuaternion( DiQuat::IDENTITY, bindPoseIndex );
                        bindPose->mScale.setFromVector3( DiVec3::UNIT_SCALE, bindPoseIndex );

                        derivedPose[bindPoseIndex] = SimpleMatrixAf4x3::IDENTITY;
                    }

                    bindPoseIndex = 0;
                    ++bindPose;
                    derivedPose += ARRAY_PACKED_REALS;
                }

                ++currentDepthLevel;
                ++bonesItor;
            }

            //Now set the reverse of the binding pose (so we can pass the
            //construct the derived transform matrices in the correct space)
            mReverseBindPose = RawSimdUniquePtr<ArrayMatrixAf4x3, MEMCATEGORY_ANIMATION>(numBoneBlocks);
            ArrayMatrixAf4x3 *reverseBindPose = mReverseBindPose.get();
            derivedPose = derivedPosesPtr.get();
            for( size_t i=0; i<numBoneBlocks; ++i )
            {
                reverseBindPose[i].loadFromAoS( derivedPose );
                reverseBindPose[i].setToInverseDegeneratesAsIdentity();
                derivedPose += ARRAY_PACKED_REALS;
            }
        }

        {
            //Cache the amount of unused slots for the SkeletonInstance
            mNumUnusedSlots = 0;
            SkeletonDef::DepthLevelInfoVec::const_iterator depthLevelItor = mDepthLevelInfoVec.begin();
            while( depthLevelItor != mDepthLevelInfoVec.end() )
            {
                if( depthLevelItor->numBonesInLevel > (ARRAY_PACKED_REALS >> 1) )
                {
                    size_t unusedSlots = ARRAY_PACKED_REALS -
                                            (depthLevelItor->numBonesInLevel % ARRAY_PACKED_REALS);
                    if( unusedSlots != ARRAY_PACKED_REALS )
                        mNumUnusedSlots += unusedSlots;
                }
                ++depthLevelItor;
            }
        }

        {
            auto boneNodeItor = boneNodes.begin();
            auto end = boneNodes.end();
            while( boneNodeItor != end )
            {
                boneNodeItor->_deinitialize();
                ++boneNodeItor;
            }
        }
    }
    
    void SkeletonDef::getBonesPerDepth( vector<size_t>::type &out ) const
    {
        out.clear();
        out.reserve( mDepthLevelInfoVec.size() );

        auto itor = mDepthLevelInfoVec.begin();
        auto end = mDepthLevelInfoVec.end();

        while( itor != end )
        {
            out.push_back( itor->numBonesInLevel );
            ++itor;
        }
    }
    
    size_t SkeletonDef::getNumberOfBoneBlocks( size_t numLevels ) const
    {
        size_t numBlocks = 0;

        auto itor = mDepthLevelInfoVec.begin();
        auto end = mDepthLevelInfoVec.begin() + numLevels;

        while( itor != end )
        {
            numBlocks += (itor->numBonesInLevel - 1 + ARRAY_PACKED_REALS) / ARRAY_PACKED_REALS;
            ++itor;
        }

        return numBlocks;
    }
}
