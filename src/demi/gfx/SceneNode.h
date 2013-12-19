
/********************************************************************
    File:       SceneNode.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "DList.h"

namespace Demi
{
    typedef shared_ptr<DiSceneNode> DiSceneNodePtr;

    class DiOctreeNode;

    class DI_GFX_API DiSceneNode : public DiBase, public enable_shared_from_this<DiSceneNode>, public DiDListBase<DiSceneNode*>
    {
    public:

        DiSceneNode(DiSceneManager* sm);

        DiSceneNode(DiSceneManager* sm, const DiString& name);

        virtual         ~DiSceneNode();

        friend class DiOctant;
        friend class DiOctantRoot;
        friend class DiOctreeNode;

    public:

        const DiString& GetName() const { return mName; }

        void            SetName(const DiString& name);

        void            SetPosition(const DiVec3& position);
        
        void            SetRotation(const DiQuat& rotation);
        
        void            SetScale(float scale);

        void            SetScale(const DiVec3& scale);

        void            SetTransform(const DiVec3& position, const DiQuat& rotation);

        void            SetTransform(const DiVec3& position, const DiQuat& rotation, float scale);

        void            SetTransform(const DiVec3& position, const DiQuat& rotation, const DiVec3& scale);

        void            SetWorldPosition(const DiVec3& position);

        void            SetWorldRotation(const DiQuat& rotation);

        void            SetWorldScale(float scale);

        void            SetWorldScale(const DiVec3& scale);

        void            SetWorldTransform(const DiVec3& position, const DiQuat& rotation);

        void            SetWorldTransform(const DiVec3& position, const DiQuat& rotation, float scale);

        void            SetWorldTransform(const DiVec3& position, const DiQuat& rotation, const DiVec3& scale);

        void            Translate(const DiVec3& delta);

        void            TranslateRelative(const DiVec3& delta);

        void            Rotate(const DiQuat& delta, bool fixedAxis = false);

        void            Pitch(float angle, bool fixedAxis = false);

        void            Yaw(float angle, bool fixedAxis = false);

        void            Roll(float angle, bool fixedAxis = false);

        void            LookAt(const DiVec3& target, const DiVec3& upAxis = DiVec3::UNIT_Y, bool worldSpace = false);

        void            Scale(float scale);

        void            Scale(const DiVec3& scale);

        const DiVec3&   GetPosition() const { return mPos; }

        const DiQuat&   GetRotation() const { return mRot; }
 
        const DiVec3&   GetScale() const { return mScale; }

        DiMat4          GetTransform() const { return DiMat4(mPos, mRot, mScale); }

        DiVec3          GetWorldPosition() const;

        DiQuat          GetWorldRotation() const;

        DiVec3          GetWorldScale() const;

        const DiMat4&   GetWorldTransform() const;

        DiVec3          LocalToWorld(const DiVec3& position) const;
        
        DiVec3          WorldToLocal(const DiVec3& position) const;
        
        bool            IsDirty() const { return mDirty; }
        
        unsigned        GetNumChildren(bool recursive = false) const;
        
        DiSceneNodePtr  CreateChild(const DiString& name = DiString::BLANK);

        void            SetParent(DiSceneNodePtr parent);

        void            AddChild(DiSceneNodePtr node);
        
        void            RemoveChild(DiSceneNodePtr node);
        
        void            RemoveAllChildren();

        // Remove from the parent node
        void            Remove();

        DiSceneNodePtr  GetChild(unsigned index) const;

        DiSceneNodePtr  GetChild(const DiString& name, bool recursive = false) const;

        void            SetVisible(bool visible, bool recursive = false);

        void            AddObject(DiTransUnitPtr obj);

        size_t          NumAddedObjects(void) const { return mAddedObjs.size(); }

        DiTransUnitPtr  GetAddedObject(size_t index);

        void            RemoveObject(size_t index);

        void            RemoveObject(DiTransUnitPtr obj);

        void            RemoveAllObjects(void);

        DiOctant*       GetOctant() const { return mOctant; }

        const DiAABB&   GetWorldAABB();

        void            Update(bool updateChildren, bool parentHasChanged);

        void            NeedUpdate(bool forceParentUpdate = false);

        void            ProcessBatchGroup(DiCamera* camera, DiRenderPipeline* pipeline);

    private:

        void            UpdateAABB();

        void            UpdateFromParent(void) const;

        void            UpdateWorldTransform() const;

        void            RemoveChild(DiVector<DiSceneNodePtr>::iterator i);

        void            RemoveObject(DiVector<DiTransUnitPtr>::iterator i);

        void            SetOctant(DiOctant* octant) { mOctant = octant; }

        void            RequestUpdate(DiSceneNode* child, bool forceParentUpdate);

        void            CancelUpdate(DiSceneNode* child);

    private:

        DiString        mName;

        mutable DiMat4  mWorldTrans;
        
        mutable bool    mDirty;

        DiVec3          mPos;
        
        DiQuat          mRot;
        
        DiVec3          mScale;

        DiSceneNodePtr  mParent;

        typedef DiVector<DiTransUnitPtr> AddedObjs;

        AddedObjs       mAddedObjs;

        typedef DiVector<DiSceneNodePtr> Children;

        Children        mChildren;

        DiSceneManager*    mCreator;

        bool            mVisible;

        DiOctant*       mOctant;

        DiAABB          mWorldAABB;

        bool            mQueuedUpdated;

        bool            mQueuedReinsertion;

        DiOctreeNode*   mOctreeNode;

        typedef DiSet<DiSceneNode*>    ChildrenSet;

        mutable ChildrenSet    mChildrenToUpdate;

        mutable bool     mNeedParentUpdate;

        mutable bool     mNeedChildUpdate;

        mutable bool     mParentNotified;

        mutable bool     mQueuedForUpdate;

        mutable DiQuat    mWorldRot;

        mutable DiVec3    mWorldPos;
        
        mutable DiVec3    mWorldScale;
    };
}