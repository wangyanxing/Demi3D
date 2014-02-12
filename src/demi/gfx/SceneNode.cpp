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
#include "SceneNode.h"
#include "SceneManager.h"
#include "TransformUnit.h"
#include "OctreeCuller.h"
#include "RenderPipeline.h"
#include "RenderBatchGroup.h"

namespace Demi
{
    DiSceneNode::DiSceneNode(DiSceneManager* sm)
        :mDirty(false),
        mPos(DiVec3::ZERO),
        mRot(DiQuat::IDENTITY),
        mScale(DiVec3::UNIT_SCALE),
        mVisible(true),
        mCreator(sm),
        mOctant(nullptr),
        mQueuedUpdated(false),
        mQueuedReinsertion(false),
        mOctreeNode(nullptr),
        mNeedParentUpdate(false),
        mNeedChildUpdate(false),
        mParentNotified(false),
        mQueuedForUpdate(false)
    {
        mWorldTrans.setIdentity();
        NeedUpdate();
    }

    DiSceneNode::DiSceneNode(DiSceneManager* sm, const DiString& name)
        :mDirty(false),
        mPos(DiVec3::ZERO),
        mRot(DiQuat::IDENTITY),
        mScale(DiVec3::UNIT_SCALE),
        mVisible(true),
        mCreator(sm),
        mName(name),
        mOctant(nullptr),
        mQueuedUpdated(false),
        mQueuedReinsertion(false),
        mOctreeNode(nullptr),
        mNeedParentUpdate(false),
        mNeedChildUpdate(false),
        mParentNotified(false),
        mQueuedForUpdate(false)
    {
        mWorldTrans.setIdentity();
        NeedUpdate();
    }

    DiSceneNode::~DiSceneNode()
    {
        RemoveAllChildren();
        RemoveAllObjects();
    }

    void  DiSceneNode::SetName(const DiString& name)
    {
        mName = name;
    }

    void  DiSceneNode::SetPosition(const DiVec3& position)
    {
        mPos = position;
        NeedUpdate();
    }

    void  DiSceneNode::SetRotation(const DiQuat& rotation)
    {
        mRot = rotation;
        NeedUpdate();
    }

    void  DiSceneNode::SetScale(float scale)
    {
        SetScale(DiVec3(scale, scale, scale));
    }

    void  DiSceneNode::SetScale(const DiVec3& scale)
    {
        mScale = scale.abs();
        NeedUpdate();
    }

    void  DiSceneNode::SetTransform(const DiVec3& position, const DiQuat& rotation)
    {
        mPos = position;
        mRot = rotation;
        NeedUpdate();
    }

    void  DiSceneNode::SetTransform(const DiVec3& position, const DiQuat& rotation, float scale)
    {
        SetTransform(position, rotation, DiVec3(scale, scale, scale));
    }

    void  DiSceneNode::SetTransform(const DiVec3& position, const DiQuat& rotation, const DiVec3& scale)
    {
        mPos = position;
        mRot = rotation;
        mScale = scale;
        NeedUpdate();
    }

    void  DiSceneNode::SetWorldPosition(const DiVec3& position)
    {
        if (!mParent)
            SetPosition(position);
        else
            SetPosition(mParent->GetWorldTransform().inverse() * position);
    }

    void  DiSceneNode::SetWorldRotation(const DiQuat& rotation)
    {
        if (!mParent)
            SetRotation(rotation);
        else
            SetRotation(mParent->GetWorldRotation().Inverse() * rotation);
    }

    void  DiSceneNode::SetWorldScale(float scale)
    {
        if (!mParent)
            SetScale(scale);
        else
        {
            DiVec3 parentWorldScale = mParent->GetWorldScale();
            SetScale(DiVec3(scale / parentWorldScale.x, scale / parentWorldScale.y, scale / parentWorldScale.z));
        }
    }

    void  DiSceneNode::SetWorldScale(const DiVec3& scale)
    {
        if (!mParent)
            SetScale(scale);
        else
            SetScale(scale / mParent->GetWorldScale());
    }

    void  DiSceneNode::SetWorldTransform(const DiVec3& position, const DiQuat& rotation)
    {
        SetWorldPosition(position);
        SetWorldRotation(rotation);
    }

    void  DiSceneNode::SetWorldTransform(const DiVec3& position, const DiQuat& rotation, float scale)
    {
        SetWorldPosition(position);
        SetWorldRotation(rotation);
        SetWorldScale(scale);
    }

    void  DiSceneNode::SetWorldTransform(const DiVec3& position, const DiQuat& rotation, const DiVec3& scale)
    {
        SetWorldPosition(position);
        SetWorldRotation(rotation);
        SetWorldScale(scale);
    }

    void  DiSceneNode::Translate(const DiVec3& delta)
    {
        mPos += delta;
        NeedUpdate();
    }

    void  DiSceneNode::TranslateRelative(const DiVec3& delta)
    {
        mPos += mRot * delta;
        NeedUpdate();
    }

    void  DiSceneNode::Rotate(const DiQuat& delta, bool fixedAxis)
    {
        if (!fixedAxis)
            mRot = (mRot * delta).normalise();
        else
            mRot = (delta * mRot).normalise();
        NeedUpdate();
    }

    void  DiSceneNode::Pitch(float angle, bool fixedAxis)
    {
        Rotate(DiQuat(angle, DiVec3::UNIT_X), fixedAxis);
    }

    void  DiSceneNode::Yaw(float angle, bool fixedAxis)
    {
        Rotate(DiQuat(angle, DiVec3::UNIT_Y), fixedAxis);
    }

    void  DiSceneNode::Roll(float angle, bool fixedAxis)
    {
        Rotate(DiQuat(angle, DiVec3::UNIT_Z), fixedAxis);
    }

    void  DiSceneNode::LookAt(const DiVec3& target, const DiVec3& upAxis, bool worldSpace)
    {
        DiVec3 targetZ;
        if (worldSpace)
            targetZ = (target - GetWorldPosition()).normalisedCopy();
        else
            targetZ = (target - mPos).normalisedCopy();

        DiVec3 targetX = upAxis.crossProduct(targetZ).normalisedCopy();
        DiVec3 targetY = targetZ.crossProduct(targetX).normalisedCopy();

        if (!worldSpace || !mParent)
            SetRotation(DiQuat(targetX, targetY, targetZ));
        else
            SetRotation(mParent->GetWorldRotation().Inverse() * DiQuat(targetX, targetY, targetZ));
    }

    void  DiSceneNode::Scale(float scale)
    {
        Scale(DiVec3(scale, scale, scale));
    }

    void  DiSceneNode::Scale(const DiVec3& scale)
    {
        mScale *= scale;
        NeedUpdate();
    }

    void  DiSceneNode::ProcessBatchGroup(DiCamera* camera, DiRenderPipeline* pipeline)
    {
        for (auto i = mAddedObjs.begin(); i != mAddedObjs.end(); ++i)
        {
            DiTransUnitPtr obj = *i;
            DiRenderBatchGroup* gp = pipeline->GetBatchGroup(obj->GetBatchGroup());
            obj->NotifyCurrentCamera(camera);
            obj->CullingUpdate(gp, camera);
        }
    }


    DiVec3 DiSceneNode::GetWorldPosition() const
    {
        if (mNeedParentUpdate)
            UpdateFromParent();
        return mWorldPos;
    }

    DiQuat DiSceneNode::GetWorldRotation() const
    {
        if (mNeedParentUpdate)
            UpdateFromParent();
        return mWorldRot;
    }

    DiVec3 DiSceneNode::GetWorldScale() const
    {
        if (mNeedParentUpdate)
            UpdateFromParent();
        return mWorldScale;
    }

    const DiMat4& DiSceneNode::GetWorldTransform() const
    {
        if (mDirty)
        {
            mWorldTrans.makeTransform(
                GetWorldPosition(),
                GetWorldScale(),
                GetWorldRotation());
            mDirty = false;
        }
        return mWorldTrans;
    }

    void DiSceneNode::UpdateFromParent(void) const
    {
        if (mParent)
        {
            const DiQuat& parentRot = mParent->GetWorldRotation();
            mWorldRot = parentRot * mRot;
            
            const DiVec3& parentScale = mParent->GetWorldScale();
            mWorldScale = parentScale * mScale;

            mWorldPos = parentRot * (parentScale * mPos);
            mWorldPos += mParent->GetWorldPosition();
        }
        else
        {
            mWorldRot = mRot;
            mWorldPos = mPos;
            mWorldScale = mScale;
        }

        mDirty = true;
        mNeedParentUpdate = false;
    }

    void DiSceneNode::Update(bool updateChildren, bool parentHasChanged)
    {
        mParentNotified = false;

        if (!updateChildren && !mNeedParentUpdate && !mNeedChildUpdate && !parentHasChanged)
            return;

        if (mNeedParentUpdate || parentHasChanged)
            UpdateFromParent();

        if (mNeedChildUpdate || parentHasChanged)
        {
            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
                (*i)->Update(true, true);
            mChildrenToUpdate.clear();
        }
        else
        {
            // Just update selected children
            auto itend = mChildrenToUpdate.end();
            for (auto it = mChildrenToUpdate.begin(); it != itend; ++it)
                (*it)->Update(true, false);

            mChildrenToUpdate.clear();
        }

        mNeedChildUpdate = false;

        UpdateAABB();
    }

    void DiSceneNode::UpdateAABB()
    {
        mWorldAABB.SetNull();

        for (auto i = mAddedObjs.begin(); i != mAddedObjs.end(); ++i)
            mWorldAABB.Merge((*i)->GetWorldBoundingBox(true));

        if (!mWorldAABB.IsNull())
        {
            if (mOctreeNode)
                mOctreeNode->DeleteSceneNode(this);

            if (mVisible)
            {
                DiAABB box = GetWorldAABB();
                float radiusSqr = box.GetRadiusSqr();
                DiVec3 center = box.GetCenter();
                mCreator->GetOctreeNode()->InsertSceneNode(this, box, radiusSqr, center);
            }
        }
    }

    DiVec3 DiSceneNode::LocalToWorld(const DiVec3& position) const
    {
        return GetWorldTransform() * position;
    }

    DiVec3 DiSceneNode::WorldToLocal(const DiVec3& position) const
    {
        return GetWorldTransform().inverse() * position;
    }

    unsigned DiSceneNode::GetNumChildren(bool recursive) const
    {
        if (!recursive)
            return mChildren.size();
        else
        {
            unsigned allChildren = mChildren.size();
            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
                allChildren += (*i)->GetNumChildren(true);

            return allChildren;
        }
    }

    DiSceneNodePtr  DiSceneNode::CreateChild(const DiString& name)
    {
        DiSceneNodePtr child = make_shared<DiSceneNode>(mCreator, name);
        AddChild(child);
        return child;
    }

    void DiSceneNode::AddChild(DiSceneNodePtr node)
    {
        if (!node || node.get() == this || node->mParent.get() == this)
        {
            DI_WARNING("Cannot add child.");
            return;
        }

        DiSceneNodePtr parent = mParent;
        while (parent)
        {
            if (parent == node)
            {
                DI_WARNING("Cannot add child, cyclic parent assignment.");
                return;
            }
            parent = parent->mParent;
        }

        mChildren.push_back(node);
        node->Remove();

        node->SetParent(shared_from_this());
    }

    void DiSceneNode::RemoveChild(DiSceneNodePtr node)
    {
        if (!node)
            return;

        for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
        {
            if (*i == node)
            {
                RemoveChild(i);
                return;
            }
        }
    }

    void DiSceneNode::RemoveChild(DiVector<DiSceneNodePtr>::iterator i)
    {
        CancelUpdate(i->get());
        (*i)->SetParent(nullptr);
        mChildren.erase(i);
    }

    void DiSceneNode::RemoveAllChildren()
    {
        while (!mChildren.empty())
            RemoveChild(--mChildren.end());
    }

    void DiSceneNode::Remove()
    {
        if (mParent)
            mParent->RemoveChild(shared_from_this());
    }

    DiSceneNodePtr DiSceneNode::GetChild(unsigned index) const
    {
        return index < mChildren.size() ? mChildren[index] : nullptr;
    }

    DiSceneNodePtr DiSceneNode::GetChild(const DiString& name, bool recursive) const
    {
        for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
        {
            if ((*i)->GetName() == mName)
                return *i;

            if (recursive)
            {
                DiSceneNodePtr node = (*i)->GetChild(name, true);
                if (node)
                    return node;
            }
        }

        return nullptr;
    }

    void DiSceneNode::SetVisible(bool visible, bool recursive)
    {
        if (visible == mVisible)
            return;

        mVisible = visible;
        
        NeedUpdate();

        if (recursive)
        {
            for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
                (*i)->SetVisible(visible, recursive);
        }
    }

    void DiSceneNode::UpdateWorldTransform() const
    {
        DiMat4 transform = GetTransform();
        if (mParent)
            mWorldTrans = mParent->GetWorldTransform() * transform;
        else
            mWorldTrans = transform;

        mDirty = false;
    }

    void DiSceneNode::AddObject(DiTransUnitPtr obj)
    {
        DI_ASSERT(obj);
        if (!obj)
            return;

        obj->SetSceneNode(this);
        
        mAddedObjs.push_back(obj);

        //if (!mQueuedReinsertion && mOctant)
        //    mOctant->GetRoot()->QueueReinsertion(this);

        //if (!mOctant)
        //    AddToOctree();

        NeedUpdate();
    }

    DiTransUnitPtr    DiSceneNode::GetAddedObject(size_t index)
    {
        return mAddedObjs[index];
    }

    void DiSceneNode::RemoveObject(size_t index)
    {
        RemoveObject(mAddedObjs.begin() + index);
    }

    void DiSceneNode::RemoveObject(DiVector<DiTransUnitPtr>::iterator i)
    {
        DiTransUnitPtr obj = (*i);
        mAddedObjs.erase(i);

        if (obj)
            obj->SetSceneNode(nullptr);

        //if (mAddedObjs.empty())
        //    RemoveFromOctree();
        NeedUpdate();
    }

    void DiSceneNode::RemoveObject(DiTransUnitPtr obj)
    {
        for (auto i = mAddedObjs.begin(); i != mAddedObjs.end(); ++i)
        {
            if (*i == obj)
            {
                RemoveObject(i);
                return;
            }
        }
    }

    void DiSceneNode::RemoveAllObjects(void)
    {
        while (!mAddedObjs.empty())
            RemoveObject(--mAddedObjs.end());
    }

    void DiSceneNode::SetParent(DiSceneNodePtr parent)
    {
        mParent = parent;
        mParentNotified = false;
        NeedUpdate();
    }

    const DiAABB& DiSceneNode::GetWorldAABB()
    {
        mWorldAABB.SetNull();

        for (auto i = mAddedObjs.begin(); i != mAddedObjs.end(); ++i)
        {
            mWorldAABB.Merge((*i)->GetWorldBoundingBox(true));
        }

        return mWorldAABB;
    }

    void DiSceneNode::RequestUpdate(DiSceneNode* child, bool forceParentUpdate)
    {
        if (mNeedChildUpdate)
            return;

        mChildrenToUpdate.insert(child);
        if (mParent && (!mParentNotified || forceParentUpdate))
        {
            mParent->RequestUpdate(this, forceParentUpdate);
            mParentNotified = true;
        }
    }

    void DiSceneNode::CancelUpdate(DiSceneNode* child)
    {
        mChildrenToUpdate.erase(child);

        if (mChildrenToUpdate.empty() &&
            mParent && !mNeedChildUpdate)
        {
            mParent->CancelUpdate(this);
            mParentNotified = false;
        }
    }

    void DiSceneNode::NeedUpdate(bool forceParentUpdate)
    {
        mNeedParentUpdate = true;
        mNeedChildUpdate = true;
        mDirty = true;

        if (mParent && (!mParentNotified || forceParentUpdate))
        {
            mParent->RequestUpdate(this, forceParentUpdate);
            mParentNotified = true;
        }

        mChildrenToUpdate.clear();
    }
}