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
#include "Node.h"
#include "RenderWindow.h"
#include "NodeMemoryManager.h"
#include <sstream>

#include "Math/Array/BooleanMask.h"

#ifndef NDEBUG
#   define CACHED_TRANSFORM_OUT_OF_DATE() this->SetCachedTransformOutOfDate()
#else
#   define CACHED_TRANSFORM_OUT_OF_DATE() ((void)0)
#endif

namespace Demi 
{
    DiNode::DiNode(IdType id, NodeMemoryManager *nodeMemoryManager, DiNode *parent) :
        IdObject(id),
        mDepthLevel(0),
        mParent(parent),
        mName(""),
#ifndef NDEBUG
        mCachedTransformOutOfDate(true),
#endif
        mListener(nullptr),
        mNodeMemoryManager(nodeMemoryManager),
        mGlobalIndex(-1),
        mParentIndex(-1)
    {
        if (mParent)
            mDepthLevel = mParent->mDepthLevel + 1;

        //Will initialize mTransform
        mNodeMemoryManager->nodeCreated(mTransform, mDepthLevel);
        mTransform.mOwner[mTransform.mIndex] = this;
        if (mParent)
            mTransform.mParents[mTransform.mIndex] = mParent;
    }
    
    DiNode::DiNode(const Transform &transformPtrs) :
        IdObject(0),
        mDepthLevel(0),
        mParent(nullptr),
        mName("Dummy Node"),
#ifndef NDEBUG
        mCachedTransformOutOfDate(true),
#endif
        mListener(nullptr),
        mNodeMemoryManager(0),
        mGlobalIndex(-1),
        mParentIndex(-1)
    {
        mTransform = transformPtrs;
    }
    
    DiNode::~DiNode()
    {
        // Call listener (note, only called if there's something to do)
        if (mListener)
        {
            mListener->NodeDestroyed(this);
        }

        RemoveAllChildren();
        if (mParent)
            mParent->RemoveChild(this);

        if (mNodeMemoryManager)
            mNodeMemoryManager->nodeDestroyed(mTransform, mDepthLevel);
        mDepthLevel = 0;
    }
    
    DiNode* DiNode::GetParent(void) const
    {
        return mParent;
    }
    
    bool DiNode::IsStatic() const
    {
        return mNodeMemoryManager->getMemoryManagerType() == SCENE_STATIC;
    }
    
    bool DiNode::SetStatic(bool bStatic)
    {
        bool retVal = false;
        if (mNodeMemoryManager->getTwin() &&
            ((mNodeMemoryManager->getMemoryManagerType() == SCENE_STATIC && !bStatic) ||
            (mNodeMemoryManager->getMemoryManagerType() == SCENE_DYNAMIC && bStatic)))
        {
            mNodeMemoryManager->migrateTo(mTransform, mDepthLevel, mNodeMemoryManager->getTwin());
            mNodeMemoryManager = mNodeMemoryManager->getTwin();
            CallMemoryChangeListeners();
            retVal = true;
        }

        return retVal;
    }
    
    void DiNode::SetParent(DiNode* parent)
    {
        if (mParent != parent)
        {
            mParent = parent;

            // Call listener
            if (mListener)
                mListener->NodeAttached(this);

            size_t oldDepthLevel = mDepthLevel;

            //NodeMemoryManager will set mTransform.mParents to a dummy parent node
            //(as well as transfering the memory)
            mNodeMemoryManager->nodeDettached(mTransform, mDepthLevel);

            mDepthLevel = mParent->mDepthLevel + 1;
            mTransform.mParents[mTransform.mIndex] = parent;
            mNodeMemoryManager->nodeAttached(mTransform, mDepthLevel);

            if (oldDepthLevel != mDepthLevel)
            {
                //Propagate the change to our children
                NodeVec::const_iterator itor = mChildren.begin();
                NodeVec::const_iterator end = mChildren.end();

                while (itor != end)
                {
                    (*itor)->ParentDepthLevelChanged();
                    ++itor;
                }

                CallMemoryChangeListeners();
            }
        }
    }
    
    void DiNode::UnsetParent(void)
    {
        if (mParent)
        {
            // Call listener
            if (mListener)
                mListener->NodeDetached(this);

            mParent = 0;

            //NodeMemoryManager will set mTransform.mParents to a dummy parent node
            //(as well as transfering the memory)
            mNodeMemoryManager->nodeDettached(mTransform, mDepthLevel);

            if (mDepthLevel != 0)
            {
                mDepthLevel = 0;

                //Propagate the change to our children
                NodeVec::const_iterator itor = mChildren.begin();
                NodeVec::const_iterator end = mChildren.end();

                while (itor != end)
                {
                    (*itor)->ParentDepthLevelChanged();
                    ++itor;
                }

                CallMemoryChangeListeners();
            }
        }
    }
    
    void DiNode::ParentDepthLevelChanged(void)
    {
        mNodeMemoryManager->nodeMoved(mTransform, mDepthLevel, mParent->mDepthLevel + 1);
        mDepthLevel = mParent->mDepthLevel + 1;

        //Keep propagating changes to our children
        NodeVec::const_iterator itor = mChildren.begin();
        NodeVec::const_iterator end = mChildren.end();

        while (itor != end)
        {
            (*itor)->ParentDepthLevelChanged();
            ++itor;
        }
    }

    const DiMat4& DiNode::GetFullTransformUpdated(void)
    {
        UpdateFromParent();
        return mTransform.mDerivedTransform[mTransform.mIndex];
    }
    
    void DiNode::UpdateFromParent(void)
    {
        if (mParent)
            mParent->UpdateFromParent();

        UpdateFromParentImpl();

        // Call listener (note, this method only called if there's something to do)
        if (mListener)
        {
            mListener->NodeUpdated(this);
        }
    }
    
    void DiNode::UpdateFromParentImpl(void)
    {
        //Retrieve from parents. Unfortunately we need to do SoA -> AoS -> SoA conversion
        ArrayVector3 parentPos, parentScale;
        ArrayQuaternion parentRot;

        for (size_t j = 0; j<ARRAY_PACKED_REALS; ++j)
        {
            DiVec3 pos, nodeScale;
            DiQuat qRot;
            const Transform &parentTransform = mTransform.mParents[j]->mTransform;
            parentTransform.mDerivedPosition->getAsVector3(pos, parentTransform.mIndex);
            parentTransform.mDerivedOrientation->getAsQuaternion(qRot, parentTransform.mIndex);
            parentTransform.mDerivedScale->getAsVector3(nodeScale, parentTransform.mIndex);

            parentPos.setFromVector3(pos, j);
            parentRot.setFromQuaternion(qRot, j);
            parentScale.setFromVector3(nodeScale, j);
        }

        parentRot.Cmov4(BooleanMask4::getMask(mTransform.mInheritOrientation),
            ArrayQuaternion::IDENTITY);
        parentScale.Cmov4(BooleanMask4::getMask(mTransform.mInheritScale),
            ArrayVector3::UNIT_SCALE);

        // Scale own position by parent scale, NB just combine
        // as equivalent axes, no shearing
        *mTransform.mDerivedScale = parentScale * (*mTransform.mScale);

        // Combine orientation with that of parent
        *mTransform.mDerivedOrientation = parentRot * (*mTransform.mOrientation);

        // Change position vector based on parent's orientation & scale
        *mTransform.mDerivedPosition = parentRot * (parentScale * (*mTransform.mPosition));

        // Add altered position vector to parents
        *mTransform.mDerivedPosition += parentPos;

        ArrayMatrix4 derivedTransform;
        derivedTransform.makeTransform(*mTransform.mDerivedPosition,
            *mTransform.mDerivedScale,
            *mTransform.mDerivedOrientation);
        derivedTransform.storeToAoS(mTransform.mDerivedTransform);
#ifndef NDEBUG
        for (size_t j = 0; j<ARRAY_PACKED_REALS; ++j)
        {
            if (mTransform.mOwner[j])
                mTransform.mOwner[j]->mCachedTransformOutOfDate = false;
        }
#endif
    }
    
    void DiNode::UpdateAllTransforms(const size_t numNodes, Transform t)
    {
        ArrayMatrix4 derivedTransform;
        for (size_t i = 0; i<numNodes; i += ARRAY_PACKED_REALS)
        {
            //Retrieve from parents. Unfortunately we need to do SoA -> AoS -> SoA conversion
            ArrayVector3 parentPos, parentScale;
            ArrayQuaternion parentRot;

            for (size_t j = 0; j<ARRAY_PACKED_REALS; ++j)
            {
                DiVec3 pos, scale;
                DiQuat qRot;
                const Transform &parentTransform = t.mParents[j]->mTransform;
                parentTransform.mDerivedPosition->getAsVector3(pos, parentTransform.mIndex);
                parentTransform.mDerivedOrientation->getAsQuaternion(qRot, parentTransform.mIndex);
                parentTransform.mDerivedScale->getAsVector3(scale, parentTransform.mIndex);

                parentPos.setFromVector3(pos, j);
                parentRot.setFromQuaternion(qRot, j);
                parentScale.setFromVector3(scale, j);
            }

            // Change position vector based on parent's orientation & scale
            *t.mDerivedPosition = parentRot * (parentScale * (*t.mPosition));

            // Combine orientation with that of parent
            *t.mDerivedOrientation = ArrayQuaternion::Cmov4(parentRot * (*t.mOrientation),
                *t.mOrientation,
                BooleanMask4::getMask(t.mInheritOrientation));

            // Scale own position by parent scale, NB just combine
            // as equivalent axes, no shearing
            *t.mDerivedScale = ArrayVector3::Cmov4(parentScale * (*t.mScale), *t.mScale,
                BooleanMask4::getMask(t.mInheritScale));

            // Add altered position vector to parents
            *t.mDerivedPosition += parentPos;

            derivedTransform.makeTransform(*t.mDerivedPosition,
                *t.mDerivedScale,
                *t.mDerivedOrientation);
            derivedTransform.storeToAoS(t.mDerivedTransform);
#ifndef NDEBUG
            for (size_t j = 0; j<ARRAY_PACKED_REALS; ++j)
            {
                if (t.mOwner[j])
                    t.mOwner[j]->mCachedTransformOutOfDate = false;
            }
#endif

            t.advancePack();
        }
    }
    
    DiNode* DiNode::CreateChild(SceneMemoryMgrTypes sceneType,
        const DiVec3& inTranslate, const DiQuat& inRotate)
    {
        DiNode* newNode = CreateChildImpl(sceneType);
        newNode->SetPosition(inTranslate);
        newNode->SetOrientation(inRotate);

        //createChildImpl must have passed us as parent. It's a special
        //case to improve memory usage (avoid transfering mTransform)
        mChildren.push_back(newNode);
        newNode->mParentIndex = mChildren.size() - 1;

        return newNode;
    }
    
    void DiNode::AddChild(DiNode* child)
    {
        if (child->mParent)
        {
            DI_WARNING("DiNode ID: %d, named %s already was a child of node ID: %d, named %s",
                child->getId(), child->GetName().c_str(), child->mParent->getId(), child->mParent->GetName().c_str());
        }

        mChildren.push_back(child);
        child->mParentIndex = mChildren.size() - 1;
        child->SetParent(this);
    }
    
    void DiNode::RemoveChild(DiNode* child)
    {
        DI_ASSERT(child->GetParent() == this && "DiNode says it's not our child");
        DI_ASSERT(child->mParentIndex < mChildren.size() && "mParentIndex was out of date!!!");

        if (child->mParentIndex < mChildren.size())
        {
            NodeVec::iterator itor = mChildren.begin() + child->mParentIndex;

            DI_ASSERT(child == *itor && "mParentIndex was out of date!!!");

            if (child == *itor)
            {
                itor = efficientVectorRemove(mChildren, itor);
                child->UnsetParent();
                child->mParentIndex = -1;

                //The node that was at the end got swapped and has now a different index
                if (itor != mChildren.end())
                    (*itor)->mParentIndex = itor - mChildren.begin();
            }
        }
    }
    
    DiQuat DiNode::GetOrientation() const
    {
        return mTransform.mOrientation->getAsQuaternion(mTransform.mIndex);
    }
    
    void DiNode::SetOrientation(DiQuat q)
    {
        DI_ASSERT(!q.isNaN() && "Invalid orientation supplied as parameter");
        q.normalise();
        mTransform.mOrientation->setFromQuaternion(q, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    void DiNode::SetOrientation(float w, float x, float y, float z)
    {
        SetOrientation(DiQuat(w, x, y, z));
    }
    
    void DiNode::ResetOrientation(void)
    {
        mTransform.mOrientation->setFromQuaternion(DiQuat::IDENTITY, mTransform.mIndex);
    }

    
    void DiNode::SetPosition(const DiVec3& pos)
    {
        DI_ASSERT(!pos.isNaN());
        mTransform.mPosition->setFromVector3(pos, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    void DiNode::SetPosition(float x, float y, float z)
    {
        SetPosition(DiVec3(x, y, z));
    }
    
    DiVec3 DiNode::GetPosition(void) const
    {
        return mTransform.mPosition->getAsVector3(mTransform.mIndex);
    }
    
    DiMat3 DiNode::GetLocalAxes(void) const
    {
        DiQuat q;
        mTransform.mOrientation->getAsQuaternion(q, mTransform.mIndex);
        DiMat3 retVal;
        q.ToRotationMatrix(retVal);

        /* Equivalent code (easier to visualize):
        axisX = q.xAxis();
        axisY = q.yAxis();
        axisZ = q.zAxis();

        return DiMat3(axisX.x, axisY.x, axisZ.x,
        axisX.y, axisY.y, axisZ.y,
        axisX.z, axisY.z, axisZ.z);*/

        return retVal;
    }

    
    void DiNode::Translate(const DiVec3& d, TransformSpace relativeTo)
    {
        DiVec3 position;
        mTransform.mPosition->getAsVector3(position, mTransform.mIndex);

        switch (relativeTo)
        {
        case TS_LOCAL:
            // position is relative to parent so transform downwards
            position += mTransform.mOrientation->getAsQuaternion(mTransform.mIndex) * d;
            break;
        case TS_WORLD:
            // position is relative to parent so transform upwards
            if (mParent)
            {
                position += (mParent->GetDerivedOrientation().Inverse() * d)
                    / mParent->GetDerivedScale();
            }
            else
            {
                position += d;
            }
            break;
        case TS_PARENT:
            position += d;
            break;
        }

        mTransform.mPosition->setFromVector3(position, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    void DiNode::Translate(float x, float y, float z, TransformSpace relativeTo)
    {
        DiVec3 v(x, y, z);
        Translate(v, relativeTo);
    }
    
    void DiNode::Translate(const DiMat3& axes, const DiVec3& move, TransformSpace relativeTo)
    {
        DiVec3 derived = axes * move;
        Translate(derived, relativeTo);
    }
    
    void DiNode::Translate(const DiMat3& axes, float x, float y, float z, TransformSpace relativeTo)
    {
        DiVec3 d(x, y, z);
        Translate(axes, d, relativeTo);
    }
    
    void DiNode::Roll(const DiRadian& angle, TransformSpace relativeTo)
    {
        Rotate(DiVec3::UNIT_Z, angle, relativeTo);
    }
    
    void DiNode::Pitch(const DiRadian& angle, TransformSpace relativeTo)
    {
        Rotate(DiVec3::UNIT_X, angle, relativeTo);
    }
    
    void DiNode::Yaw(const DiRadian& angle, TransformSpace relativeTo)
    {
        Rotate(DiVec3::UNIT_Y, angle, relativeTo);

    }
    
    void DiNode::Rotate(const DiVec3& axis, const DiRadian& angle, TransformSpace relativeTo)
    {
        DiQuat q;
        q.FromAngleAxis(angle, axis);
        Rotate(q, relativeTo);
    }

    
    void DiNode::Rotate(const DiQuat& q, TransformSpace relativeTo)
    {
        // Normalise quaternion to avoid drift
        DiQuat qnorm = q;
        qnorm.normalise();

        DiQuat orientation;
        mTransform.mOrientation->getAsQuaternion(orientation, mTransform.mIndex);

        switch (relativeTo)
        {
        case TS_PARENT:
            // Rotations are normally relative to local axes, transform up
            orientation = qnorm * orientation;
            break;
        case TS_WORLD:
            // Rotations are normally relative to local axes, transform up
            orientation = orientation * GetDerivedOrientation().Inverse()
                * qnorm * GetDerivedOrientation();
            break;
        case TS_LOCAL:
            // Note the order of the mult, i.e. q comes after
            orientation = orientation * qnorm;
            break;
        }

        mTransform.mOrientation->setFromQuaternion(orientation, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    void DiNode::SetDerivedPosition(const DiVec3& pos)
    {
        //find where the node would end up in parent's local space
        if (mParent)
            SetPosition(mParent->ConvertWorldToLocalPosition(pos));
    }
    
    void DiNode::SetDerivedOrientation(const DiQuat& q)
    {
        //find where the node would end up in parent's local space
        if (mParent)
            SetOrientation(mParent->ConvertWorldToLocalOrientation(q));
    }

    
    DiQuat DiNode::GetDerivedOrientation(void) const
    {
        DI_ASSERT(!mCachedTransformOutOfDate);
        return mTransform.mDerivedOrientation->getAsQuaternion(mTransform.mIndex);
    }
    
    DiQuat DiNode::GetDerivedOrientationUpdated(void)
    {
        UpdateFromParent();
        return mTransform.mDerivedOrientation->getAsQuaternion(mTransform.mIndex);
    }
    
    DiVec3 DiNode::GetDerivedPosition(void) const
    {
        DI_ASSERT(!mCachedTransformOutOfDate);
        return mTransform.mDerivedPosition->getAsVector3(mTransform.mIndex);
    }
    
    DiVec3 DiNode::GetDerivedPositionUpdated(void)
    {
        UpdateFromParent();
        return mTransform.mDerivedPosition->getAsVector3(mTransform.mIndex);
    }
    
    DiVec3 DiNode::GetDerivedScale(void) const
    {
        DI_ASSERT(!mCachedTransformOutOfDate);
        return mTransform.mDerivedScale->getAsVector3(mTransform.mIndex);
    }
    
    DiVec3 DiNode::GetDerivedScaleUpdated(void)
    {
        UpdateFromParent();
        return mTransform.mDerivedScale->getAsVector3(mTransform.mIndex);
    }
    
    DiVec3 DiNode::ConvertWorldToLocalPosition(const DiVec3 &worldPos)
    {
        DI_ASSERT(!mCachedTransformOutOfDate);

        ArrayVector3 arrayWorldPos;
        arrayWorldPos.setAll(worldPos);
        arrayWorldPos = mTransform.mDerivedOrientation->Inverse() *
            (arrayWorldPos - (*mTransform.mDerivedPosition)) /
            (*mTransform.mDerivedScale);

        DiVec3 retVal;
        arrayWorldPos.getAsVector3(retVal, mTransform.mIndex);
        return retVal;
    }
    
    DiVec3 DiNode::ConvertLocalToWorldPosition(const DiVec3 &localPos)
    {
        DI_ASSERT(!mCachedTransformOutOfDate);
        ArrayVector3 arrayLocalPos;
        arrayLocalPos.setAll(localPos);
        arrayLocalPos = ((*mTransform.mDerivedOrientation) *
            (arrayLocalPos * (*mTransform.mDerivedScale))) +
            (*mTransform.mDerivedPosition);

        DiVec3 retVal;
        arrayLocalPos.getAsVector3(retVal, mTransform.mIndex);
        return retVal;
    }
    
    DiQuat DiNode::ConvertWorldToLocalOrientation(const DiQuat &worldOrientation)
    {
        DI_ASSERT(!mCachedTransformOutOfDate);
        return mTransform.mDerivedOrientation->getAsQuaternion(mTransform.mIndex).Inverse() *
            worldOrientation;
    }
    
    DiQuat DiNode::ConvertLocalToWorldOrientation(const DiQuat &localOrientation)
    {
        DI_ASSERT(!mCachedTransformOutOfDate);
        return mTransform.mDerivedOrientation->getAsQuaternion(mTransform.mIndex) * localOrientation;
    }
    
    void DiNode::RemoveAllChildren(void)
    {
        NodeVec::iterator itor = mChildren.begin();
        NodeVec::iterator end = mChildren.end();
        while (itor != end)
        {
            (*itor)->UnsetParent();
            (*itor)->mParentIndex = -1;
            ++itor;
        }
        mChildren.clear();
    }
    
    void DiNode::SetScale(const DiVec3& inScale)
    {
        DI_ASSERT(!inScale.isNaN() && "Invalid vector supplied as parameter");
        mTransform.mScale->setFromVector3(inScale, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    void DiNode::SetScale(float x, float y, float z)
    {
        SetScale(DiVec3(x, y, z));
    }
    
    DiVec3 DiNode::GetScale(void) const
    {
        return mTransform.mScale->getAsVector3(mTransform.mIndex);
    }
    
    void DiNode::SetInheritOrientation(bool inherit)
    {
        mTransform.mInheritOrientation[mTransform.mIndex] = inherit;
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    bool DiNode::GetInheritOrientation(void) const
    {
        return mTransform.mInheritOrientation[mTransform.mIndex];
    }
    
    void DiNode::SetInheritScale(bool inherit)
    {
        mTransform.mInheritScale[mTransform.mIndex] = inherit;
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    bool DiNode::GetInheritScale(void) const
    {
        return mTransform.mInheritScale[mTransform.mIndex];
    }
    
    void DiNode::Scale(const DiVec3& inScale)
    {
        mTransform.mScale->setFromVector3(mTransform.mScale->getAsVector3(mTransform.mIndex) *
            inScale, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    void DiNode::Scale(float x, float y, float z)
    {
        Scale(DiVec3(x, y, z));
    }

    float DiNode::GetSquaredViewDepth(const DiCamera* cam) const
    {
        DiVec3 diff = GetDerivedPosition() - cam->GetDerivedPosition();

        // NB use squared length rather than real depth to avoid square root
        return diff.squaredLength();
    }
    
#ifndef NDEBUG
    void DiNode::SetCachedTransformOutOfDate(void)
    {
        mCachedTransformOutOfDate = true;

        NodeVec::const_iterator itor = mChildren.begin();
        NodeVec::const_iterator end = mChildren.end();

        while (itor != end)
        {
            (*itor)->SetCachedTransformOutOfDate();
            ++itor;
        }
    }
#endif
   
}

#undef CACHED_TRANSFORM_OUT_OF_DATE
