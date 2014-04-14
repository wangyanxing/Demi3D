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
#include "NewBone.h"

namespace Demi
{
    DiNewBone::DiNewBone() :
        IdObject(0),
        mReverseBind(0),
        mTransform(BoneTransform()),

        mDepthLevel(0),
        mParent(0),
        mName("dummy bone"),
        mBoneMemoryManager(0),
        mGlobalIndex(-1),
        mParentIndex(-1)
    {
    }
    
    DiNewBone::~DiNewBone()
    {
        DI_ASSERT(!mInitialized && "Must call _deinitialize() before destructor!");
    }
    
    void DiNewBone::_initialize(IdType id, BoneMemoryManager *boneMemoryManager,
        DiNewBone *parent, ArrayMatrixAf4x3 const * RESTRICT_ALIAS reverseBind)
    {
        DI_ASSERT(!mInitialized);

        this->_setId(id);
        mReverseBind = reverseBind;
        mParent = parent;
        mBoneMemoryManager = boneMemoryManager;

        if (mParent)
            mDepthLevel = mParent->mDepthLevel + 1;

        //Will initialize mTransform
        mBoneMemoryManager->nodeCreated(mTransform, mDepthLevel);
        mTransform.mOwner[mTransform.mIndex] = this;
        if (mParent)
        {
            const BoneTransform parentTransform = mParent->mTransform;
            mTransform.mParentTransform[mTransform.mIndex] =
                &parentTransform.mDerivedTransform[parentTransform.mIndex];

            mParent->mChildren.push_back(this);
            this->mParentIndex = mParent->mChildren.size() - 1;
        }
    }
    
    void DiNewBone::_deinitialize(void)
    {
        if (mBoneMemoryManager)
            mBoneMemoryManager->nodeDestroyed(mTransform, mDepthLevel);

        mReverseBind = 0;
        mBoneMemoryManager = 0;
    }
    
    void DiNewBone::setCachedTransformOutOfDate(void) const
    {
    }
    
    void DiNewBone::resetParentTransformPtr(void)
    {
        if (mParent)
        {
            const BoneTransform parentTransform = mParent->mTransform;
            mTransform.mParentTransform[mTransform.mIndex] =
                &parentTransform.mDerivedTransform[parentTransform.mIndex];
        }

        _memoryRebased();
    }
    
    void DiNewBone::_memoryRebased(void)
    {
        BoneVec::iterator itor = mChildren.begin();
        BoneVec::iterator end = mChildren.end();
        while (itor != end)
        {
            (*itor)->resetParentTransformPtr();
            ++itor;
        }
    }
    
    void DiNewBone::_setNodeParent(DiNode *nodeParent)
    {
        if (nodeParent)
        {
            //This "Hack" just works. Don't ask. And it's fast!
            //(we're responsible for ensuring the memory layout matches)
            Transform parentTransf = nodeParent->_getTransform();
            mTransform.mParentNodeTransform[mTransform.mIndex] = reinterpret_cast<SimpleMatrixAf4x3*>(
                &parentTransf.mDerivedTransform[parentTransf.mIndex]);
        }
        else
        {
            mTransform.mParentNodeTransform[mTransform.mIndex] = &SimpleMatrixAf4x3::IDENTITY;
        }
    }
    
    void DiNewBone::setInheritOrientation(bool inherit)
    {
        mTransform.mInheritOrientation[mTransform.mIndex] = inherit;
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    bool DiNewBone::getInheritOrientation(void) const
    {
        return mTransform.mInheritOrientation[mTransform.mIndex];
    }
    
    void DiNewBone::setInheritScale(bool inherit)
    {
        mTransform.mInheritScale[mTransform.mIndex] = inherit;
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    bool DiNewBone::getInheritScale(void) const
    {
        return mTransform.mInheritScale[mTransform.mIndex];
    }
    
    const SimpleMatrixAf4x3& DiNewBone::_getFullTransformUpdated(void)
    {
        _updateFromParent();
        return mTransform.mDerivedTransform[mTransform.mIndex];
    }
    
    void DiNewBone::_updateFromParent(void)
    {
        if (mParent)
            mParent->_updateFromParent();

        updateFromParentImpl();
    }
    
    void DiNewBone::updateFromParentImpl(void)
    {
    }
    
    void DiNewBone::updateAllTransforms(const size_t numNodes, BoneTransform t,
        ArrayMatrixAf4x3 const * RESTRICT_ALIAS _reverseBind,
        size_t numBinds)
    {
        size_t currentBind = 0;
        numBinds = (numBinds + ARRAY_PACKED_REALS - 1) / ARRAY_PACKED_REALS;

        ArrayMatrixAf4x3 derivedTransform;
        for (size_t i = 0; i<numNodes; i += ARRAY_PACKED_REALS)
        {
            //Retrieve from parents. Unfortunately we need to do SoA -> AoS -> SoA conversion
            ArrayMatrixAf4x3 nodeMat;
            ArrayMatrixAf4x3 parentMat;

            nodeMat.loadFromAoS(t.mParentNodeTransform);
            parentMat.loadFromAoS(t.mParentTransform);

            //ArrayMatrixAf4x3::retain is quite lengthy in instruction count, and the
            //general case is to inherit both attributes. This branch is justified.
            if (!BooleanMask4::allBitsSet(t.mInheritOrientation, t.mInheritScale))
            {
                ArrayMaskR inheritOrientation = BooleanMask4::getMask(t.mInheritOrientation);
                ArrayMaskR inheritScale = BooleanMask4::getMask(t.mInheritScale);
                parentMat.retain(inheritOrientation, inheritScale);
            }

            const ArrayMatrixAf4x3 * RESTRICT_ALIAS reverseBind = _reverseBind + currentBind;

            derivedTransform.makeTransform(*t.mPosition, *t.mScale, *t.mOrientation);
            derivedTransform = parentMat * derivedTransform;
            derivedTransform.storeToAoS(t.mDerivedTransform);

            /*
            Calculating the bone matrices
            -----------------------------
            Now that we have the derived scaling factors, orientations & positions matrices,
            we have to compute the Matrix4x3 to apply to the vertices of a mesh.
            Because any modification of a vertex has to be relative to the bone, we must
            first reverse transform by the DiNewBone's original derived position/orientation/scale,
            then transform by the new derived position/orientation/scale.
            */
            //derivedTransform = nodeMat * ( derivedTransform * (*reverseBind) );
            derivedTransform *= *reverseBind;
            derivedTransform = nodeMat * derivedTransform;
            derivedTransform.streamToAoS(t.mFinalTransform);

            t.advancePack();
            currentBind = (currentBind + 1) % numBinds;
        }
    }
    
    void DiNewBone::removeChild(DiNewBone* child)
    {
        DI_ASSERT(child->getParent() == this && "Node says it's not our child");
        DI_ASSERT(child->mParentIndex < mChildren.size() && "mParentIndex was out of date!!!");

        if (child->mParentIndex < mChildren.size())
        {
            BoneVec::iterator itor = mChildren.begin() + child->mParentIndex;

            DI_ASSERT(child == *itor && "mParentIndex was out of date!!!");

            if (child == *itor)
            {
                itor = efficientVectorRemove(mChildren, itor);
                child->mParentIndex = -1;

                //The node that was at the end got swapped and has now a different index
                if (itor != mChildren.end())
                    (*itor)->mParentIndex = itor - mChildren.begin();
            }
        }
    }
}