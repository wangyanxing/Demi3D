
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

#include "SkeletonInstance.h"
#include "SkeletonDef.h"
#include "SkeletonAnimationDef.h"

#include "Id.h"

namespace Demi
{
    SkeletonInstance::SkeletonInstance( const SkeletonDef *skeletonDef,
                                        BoneMemoryManager *boneMemoryManager ) :
            mDefinition( skeletonDef ),
            mParentNode( 0 )
    {
        mBones.resize(mDefinition->getBones().size(), DiNewBone());

        auto itDepth = mDefinition->mBonesPerDepth.begin();
        auto enDepth = mDefinition->mBonesPerDepth.end();

        while( itDepth != enDepth )
        {
            auto itor = itDepth->begin();
            auto end = itDepth->end();

            while( itor != end )
            {
                DiNewBone *parent = 0;
                size_t parentIdx = mDefinition->mBones[*itor].parent;
                const SkeletonDef::BoneData &boneData = mDefinition->mBones[*itor];

                if( parentIdx != std::numeric_limits<size_t>::max() )
                    parent = &mBones[parentIdx];

                DiNewBone &newBone = mBones[*itor];
                newBone._initialize( Id::generateNewId<DiNode>(), boneMemoryManager, parent, 0 );
                newBone.setPosition( boneData.vPos );
                newBone.setOrientation( boneData.qRot );
                newBone.setScale( boneData.vScale );
                newBone.setInheritOrientation( boneData.bInheritOrientation );
                newBone.setInheritScale( boneData.bInheritScale );
                newBone.setName( boneData.name );
                newBone.mGlobalIndex = *itor;

                ++itor;
            }

            ++itDepth;
        }

        {
            const SkeletonDef::DepthLevelInfoVec &depthLevelInfo = mDefinition->getDepthLevelInfo();
            mSlotStarts.reserve( depthLevelInfo.size() );
            mBoneStartTransforms.reserve( depthLevelInfo.size() );

            mManualBones = RawSimdUniquePtr<ArrayFloat, MEMCATEGORY_ANIMATION>(
                                mDefinition->getNumberOfBoneBlocks( depthLevelInfo.size() ) );
            float *manualBones = reinterpret_cast<float*>( mManualBones.get() );

            // FIXME: manualBones could possibly be null. What to do?

            size_t currentUnusedSlotIdx = 0;
            mUnusedNodes.resize(mDefinition->mNumUnusedSlots, DiNewBone());

            ArrayMatrixAf4x3 const *reverseBindPose = mDefinition->mReverseBindPose.get();

            SkeletonDef::DepthLevelInfoVec::const_iterator itor = depthLevelInfo.begin();
            SkeletonDef::DepthLevelInfoVec::const_iterator end  = depthLevelInfo.end();

            while( itor != end )
            {
                const BoneTransform &firstBoneTransform = mBones[itor->firstBoneIndex]._getTransform();
                mBoneStartTransforms.push_back( firstBoneTransform );
                mSlotStarts.push_back( firstBoneTransform.mIndex );

                assert( (itor->numBonesInLevel <= (ARRAY_PACKED_REALS >> 1)) ||
                        !firstBoneTransform.mIndex );

                if( itor->numBonesInLevel > (ARRAY_PACKED_REALS >> 1) )
                {
                    //TODO: Reserve enough space in mUnusedNodes (amount can be cached in SkeletonDef)
                    size_t unusedSlots = ARRAY_PACKED_REALS -
                                            (itor->numBonesInLevel % ARRAY_PACKED_REALS);

                    // When x is a multiple of ARRAY_PACKED_REALS, then the formula gives:
                    //      ARRAY_PACKED_REALS - x % ARRAY_PACKED_REALS = ARRAY_PACKED_REALS;
                    // but we don't need to create any unused node, as the slots are already
                    // aligned to the block
                    if( unusedSlots != ARRAY_PACKED_REALS )
                    {
                        for( size_t i=0; i<unusedSlots; ++i )
                        {
                            //Dummy bones need the right parent so they
                            //consume memory from the right depth level
                            DiNewBone *parent = 0;
                            if( itor != depthLevelInfo.begin() )
                                parent = &mBones[(itor-1)->firstBoneIndex];

                            DiNewBone &unused = mUnusedNodes[currentUnusedSlotIdx];
                            unused._initialize( Id::generateNewId<DiNode>(), boneMemoryManager,
                                                parent, 0 );
                            unused.setName( "Unused" );
                            unused.mGlobalIndex = i;

                            ++currentUnusedSlotIdx;
                        }
                    }
                }

                //Prepare for default pose, 0.0f for manually animated or a slot that
                //doesn't belong to us, 1.0f when we should apply animation
                size_t slotStart = firstBoneTransform.mIndex;
                size_t remainder = (slotStart + itor->numBonesInLevel) % ARRAY_PACKED_REALS;
                for( size_t i=0; i<slotStart; ++i )
                    *manualBones++ = 0.0f;
                for( size_t i=slotStart; i<slotStart + itor->numBonesInLevel; ++i )
                    *manualBones++ = 1.0f;
                if( remainder != 0 )
                {
                    for( size_t i=0; i<ARRAY_PACKED_REALS - remainder; ++i )
                        *manualBones++ = 0.0f;
                }

                //Take advantage that all Bones in mOwner are planar in memory
                DiNewBone **bonesPtr = firstBoneTransform.mOwner;
                for( size_t i=slotStart; i<slotStart + itor->numBonesInLevel; ++i )
                {
                    bonesPtr[i]->_setReverseBindPtr( reverseBindPose );
                    if( !( (i+1) % ARRAY_PACKED_REALS) )
                        ++reverseBindPose;
                }

                if( remainder != 0 )
                    ++reverseBindPose;

                ++itor;
            }
        }

        const SkeletonAnimationDefVec &animationDefs = mDefinition->getAnimationDefs();
        mAnimations.reserve( animationDefs.size() );

        SkeletonAnimationDefVec::const_iterator itor = animationDefs.begin();
        SkeletonAnimationDefVec::const_iterator end  = animationDefs.end();

        while( itor != end )
        {
            SkeletonAnimation animation( &(*itor), &mSlotStarts, this );
            mAnimations.push_back( animation );
            mAnimations.back().initialize();
            ++itor;
        }
    }
    
    SkeletonInstance::~SkeletonInstance()
    {
        //Detach all bones in the reverse order they were attached (LIFO!!!)
        size_t currentDepth = mDefinition->mBonesPerDepth.size() - 1;
        auto ritDepth = mDefinition->mBonesPerDepth.rbegin();
        auto renDepth = mDefinition->mBonesPerDepth.rend();

        BoneVec::reverse_iterator ritUnusedNodes = mUnusedNodes.rbegin();
        BoneVec::reverse_iterator renUnusedNodes = mUnusedNodes.rend();

        while( ritDepth != renDepth )
        {
            while( ritUnusedNodes != renUnusedNodes && ritUnusedNodes->getDepthLevel() == currentDepth )
            {
                ritUnusedNodes->_deinitialize();
                ++ritUnusedNodes;
            }

            auto ritor = ritDepth->rbegin();
            auto rend  = ritDepth->rend();
            while( ritor != rend )
            {
                mBones[*ritor]._deinitialize();
                ++ritor;
            }

            --currentDepth;
            ++ritDepth;
        }

        mAnimations.clear();

        mUnusedNodes.clear();
        mBones.clear();
    }
    
    void SkeletonInstance::update(void)
    {
        if( !mActiveAnimations.empty() )
            resetToPose();

        ActiveAnimationsVec::iterator itor = mActiveAnimations.begin();
        ActiveAnimationsVec::iterator end  = mActiveAnimations.end();

        while( itor != end )
        {
            (*itor)->_applyAnimation( mBoneStartTransforms );
            ++itor;
        }
    }
    
    void SkeletonInstance::resetToPose(void)
    {
        KfTransform const * RESTRICT_ALIAS bindPose = mDefinition->getBindPose();
        ArrayFloat const * RESTRICT_ALIAS manualBones = mManualBones.get();

        SkeletonDef::DepthLevelInfoVec::const_iterator itDepthLevelInfo =
                                                mDefinition->getDepthLevelInfo().begin();

        TransformArray::iterator itor = mBoneStartTransforms.begin();
        TransformArray::iterator end  = mBoneStartTransforms.end();

        while( itor != end )
        {
            BoneTransform t = *itor;
            for( size_t i=0; i<itDepthLevelInfo->numBonesInLevel; i += ARRAY_PACKED_REALS )
            {
                DEMI_PREFETCH_T0((const char*)(t.mPosition + 4));
                DEMI_PREFETCH_T0((const char*)(t.mOrientation + 4));
                DEMI_PREFETCH_T0((const char*)(t.mScale + 4));
                DEMI_PREFETCH_T0((const char*)(t.mPosition + 8));
                DEMI_PREFETCH_T0((const char*)(t.mOrientation + 8));
                DEMI_PREFETCH_T0((const char*)(t.mScale + 8));

                *t.mPosition = DiMath::Lerp(*t.mPosition, bindPose->mPosition, *manualBones);
                *t.mOrientation = DiMath::Lerp(*t.mOrientation, bindPose->mOrientation, *manualBones);
                *t.mScale = DiMath::Lerp(*t.mScale, bindPose->mScale, *manualBones);
                t.advancePack();

                ++bindPose;
                ++manualBones;
            }

            ++itor;
            ++itDepthLevelInfo;
        }
    }
    
    void SkeletonInstance::setManualBone(DiNewBone *bone, bool isManual)
    {
        assert( &mBones[bone->mGlobalIndex] == bone && "The bone doesn't belong to this instance!" );

        uint32 depthLevel = bone->getDepthLevel();
        DiNewBone &firstBone = mBones[mDefinition->getDepthLevelInfo()[depthLevel].firstBoneIndex];

        uintptr_t diff = bone->_getTransform().mOwner - firstBone._getTransform().mOwner;
        float *manualBones = reinterpret_cast<float*>( mManualBones.get() );
        manualBones[diff] = isManual ? 1.0f : 0.0f;
    }
    
    bool SkeletonInstance::isManualBone(DiNewBone *bone)
    {
        assert( &mBones[bone->mGlobalIndex] == bone && "The bone doesn't belong to this instance!" );

        uint32 depthLevel = bone->getDepthLevel();
        DiNewBone &firstBone = mBones[mDefinition->getDepthLevelInfo()[depthLevel].firstBoneIndex];

        uintptr_t diff = bone->_getTransform().mOwner - firstBone._getTransform().mOwner;
        const float *manualBones = reinterpret_cast<const float*>( mManualBones.get() );
        return manualBones[diff] != 0.0f;
    }
    
    DiNewBone* SkeletonInstance::getBone(IdString boneName)
    {
        SkeletonDef::BoneNameMap::const_iterator itor = mDefinition->mBoneIndexByName.find( boneName );

        if( itor == mDefinition->mBoneIndexByName.end() )
        {
            DI_WARNING("Cannot find the bone: %s", boneName.getFriendlyText().c_str());
        }

        return &mBones[itor->second];
    }
    
    bool SkeletonInstance::hasAnimation( IdString name ) const
    {
        auto itor = mAnimations.begin();
        auto end  = mAnimations.end();

        while( itor != end && itor->getName() != name )
            ++itor;

        return itor != end;
    }
    
    SkeletonAnimation* SkeletonInstance::getAnimation( IdString name )
    {
        auto itor = mAnimations.begin();
        auto end  = mAnimations.end();

        while( itor != end )
        {
            if( itor->getName() == name )
                return &(*itor);
            ++itor;
        }
        DI_WARNING("Cannot find the animation: %s", name.getFriendlyText().c_str());
        return 0;
    }
    
    void SkeletonInstance::_enableAnimation( SkeletonAnimation *animation )
    {
        mActiveAnimations.push_back( animation );
    }
    
    void SkeletonInstance::_disableAnimation( SkeletonAnimation *animation )
    {
        ActiveAnimationsVec::iterator it = std::find( mActiveAnimations.begin(), mActiveAnimations.end(),
                                                        animation );
        if( it != mActiveAnimations.end() )
            efficientVectorRemove( mActiveAnimations, it );
    }
    
    void SkeletonInstance::setParentNode( DiNode *parentNode )
    {
        mParentNode = parentNode;

        auto itor = mBones.begin();
        auto end = mBones.end();

        while( itor != end )
        {
            itor->_setNodeParent( mParentNode );
            ++itor;
        }
    }
    
    void SkeletonInstance::getTransforms( SimpleMatrixAf4x3 * RESTRICT_ALIAS outTransform,
                                            const FastArray<unsigned short> &usedBones ) const
    {
        FastArray<unsigned short>::const_iterator itor = usedBones.begin();
        FastArray<unsigned short>::const_iterator end  = usedBones.end();

        while( itor != end )
        {
            *outTransform++ = mBones[*itor]._getFullTransform();
            ++itor;
        }
    }
    
    void SkeletonInstance::_updateBoneStartTransforms(void)
    {
        const SkeletonDef::DepthLevelInfoVec &depthLevelInfo = mDefinition->getDepthLevelInfo();

        assert( mBoneStartTransforms.size() == depthLevelInfo.size() );

        TransformArray::iterator itBoneStartTr = mBoneStartTransforms.begin();

        auto itor = depthLevelInfo.begin();
        auto end = depthLevelInfo.end();

        while( itor != end )
        {
            *itBoneStartTr++ = mBones[itor->firstBoneIndex]._getTransform();
            ++itor;
        }
    }
    
    const void* SkeletonInstance::_getMemoryBlock(void) const
    {
        return reinterpret_cast<const void*>( mBoneStartTransforms[0].mOwner );
    }
    
    const void* SkeletonInstance::_getMemoryUniqueOffset(void) const
    {
        return reinterpret_cast<const void*>(
                        mBoneStartTransforms[0].mOwner + mBoneStartTransforms[0].mIndex );
    }
}
