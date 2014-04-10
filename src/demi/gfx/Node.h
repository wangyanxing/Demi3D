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

/// This file is adapted from Ogre 2.0 (unstable version)

#ifndef DiNode_h__
#define DiNode_h__

#include "Id.h"
#include "Math/Array/Transform.h"

namespace Demi
{
    class DI_GFX_API DiNode : public DiBase, public IdObject
    {
    public:

        /** Enumeration denoting the spaces which a transform can be relative to.
        */
        enum TransformSpace
        {
            /// Transform is relative to the local space
            TS_LOCAL,
            /// Transform is relative to the space of the parent node
            TS_PARENT,
            /// Transform is relative to world space
            TS_WORLD
        };
        typedef DiVector<DiNode*> NodeVec;

        /** Listener which gets called back on DiNode events.
        */
        class DI_GFX_API Listener
        {
        public:
            Listener() {}
            virtual ~Listener() {}

            /** Called when a node gets updated.
            @remarks
            Note that this happens when the node's derived update happens,
            not every time a method altering it's state occurs. There may
            be several state-changing calls but only one of these calls,
            when the node graph is fully updated.
            */
            virtual void NodeUpdated(const DiNode*) {}

            virtual void NodeDestroyed(const DiNode*) {}

            virtual void NodeAttached(const DiNode*) {}

            virtual void NodeDetached(const DiNode*) {}
        };

    protected:
        /// Depth level in the hierarchy tree (0: Root node, 1: Child of root, etc)
        uint16 mDepthLevel;
        /// Pointer to parent node
        DiNode* mParent;
        /// Collection of pointers to direct children; hashmap for efficiency
        NodeVec mChildren;
        /// All the transform data needed in SoA form
        Transform mTransform;

        /// Friendly name of this node, can be empty
        DiString mName;

        /// Only available internally - notification of parent. Can't be null
        void SetParent(DiNode* parent);

        void UnsetParent(void);

        /// Notification from parent that we need to migrate to a different depth level
        void ParentDepthLevelChanged(void);

        /** Triggers the node to update it's combined transforms.
        @par
        This method is called internally by Ogre to ask the node
        to update it's complete transformation based on it's parents
        derived transform.
        */
        void UpdateFromParent(void);

        /** Class-specific implementation of _updateFromParent.
        @remarks
        Splitting the implementation of the update away from the update call
        itself allows the detail to be overridden without disrupting the
        general sequence of updateFromParent (e.g. raising events)
        */
        virtual void UpdateFromParentImpl(void);

        /** Internal method for creating a new child node - must be overridden per subclass. */
        virtual DiNode* CreateChildImpl(SceneMemoryMgrTypes sceneType) = 0;

#ifndef NDEBUG
        mutable bool mCachedTransformOutOfDate;
#endif

        /** DiNode listener - only one allowed (no list) for size & performance reasons. */
        Listener* mListener;

        /// The memory manager used to allocate the Transform.
        NodeMemoryManager *mNodeMemoryManager;

    public:
        /** Index in the vector holding this node reference (could be our parent node, or a global array
        tracking all created nodes to avoid memory leaks). Used for O(1) removals.
        @remarks
        It is the parent (or our creator) the one that sets this value, not ourselves. Do NOT modify
        it manually.
        */
        size_t mGlobalIndex;
        /// @copydoc mGlobalIndex
        size_t mParentIndex;

        /** Constructor, should only be called by parent, not directly.
        @remarks
        Parent pointer can be null.
        */
        DiNode(IdType id, NodeMemoryManager *nodeMemoryManager, DiNode *parent);

        /** Don't use this constructor unless you know what you're doing.
        @See NodeMemoryManager::mDummyNode
        */
        DiNode(const Transform &transformPtrs);

        virtual ~DiNode();

        /** Sets a custom name for this node. Doesn't have to be unique */
        void SetName(const DiString &name)  { mName = name; }

        /** Returns the name of the node. */
        const DiString& GetName(void) const  { return mName; }

        /** Gets this node's parent (NULL if this is the root). */
        DiNode* GetParent(void) const;

        /// Checks whether this node is static. @See setStatic
        bool IsStatic() const;

        /** Turns this DiNode into static or dynamic
        @remarks
        Switching between dynamic and static has some overhead and forces to update all
        static scene when converted to static. So don't do it frequently.
        Static objects are not updated every frame, only when requested explicitly. Use
        this feature if you plan to have this object unaltered for a very long times
        @par
        Changing this attribute to a node will cause to switch the attribute to all
        attached entities (but not children or parent nodes; it's perfectly valid
        and useful to have dynamic children of a static parent; although the opposite
        (static children, dynamic parent) is probably a bug.
        @return
        True if setStatic made an actual change. False otherwise. Can fail because the
        object was already static/dynamic, or because switching is not supported
        */
        virtual bool SetStatic(bool bStatic);

        /// Returns how deep in the hierarchy we are (eg. 0 -> root node, 1 -> child of root)
        uint16 GetDepthLevel() const  { return mDepthLevel; }

        /// Returns a direct access to the Transform state
        Transform& GetTransform()    { return mTransform; }

        /// Called by SceneManager when it is telling we're a static node being dirty
        virtual void NotifyStaticDirty(void) const {}

        /** Returns a quaternion representing the nodes orientation.
        @remarks
        Don't call this function too often, as we need to convert from SoA
        */
        DiQuat GetOrientation() const;

        void SetOrientation(DiQuat q);

        void SetOrientation(float w, float x, float y, float z);

        void ResetOrientation(void);

        /** Sets the position of the node relative to it's parent.
        @remarks
        Don't call this function too often, as we need to convert to SoA
        */
        void SetPosition(const DiVec3& pos);

        /** Sets the position of the node relative to it's parent.
        @remarks
        Don't call this function too often, as we need to convert to SoA
        */
        void SetPosition(float x, float y, float z);

        /** Gets the position of the node relative to it's parent.
        @remarks
        Don't call this function too often, as we need to convert from SoA
        */
        DiVec3 GetPosition(void) const;

        void SetScale(const DiVec3& scale);

        void SetScale(float x, float y, float z);

        DiVec3 GetScale(void) const;

        void SetInheritOrientation(bool inherit);

        bool GetInheritOrientation(void) const;

        void SetInheritScale(bool inherit);

        bool GetInheritScale(void) const;

        void Scale(const DiVec3& scale);

        void Scale(float x, float y, float z);

        void Translate(const DiVec3& d, TransformSpace relativeTo = TS_PARENT);

        void Translate(float x, float y, float z, TransformSpace relativeTo = TS_PARENT);

        void Translate(const DiMat3& axes, const DiVec3& move, TransformSpace relativeTo = TS_PARENT);

        void Translate(const DiMat3& axes, float x, float y, float z, TransformSpace relativeTo = TS_PARENT);

        /** Rotate the node around the Z-axis.
        */
        void Roll(const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        /** Rotate the node around the X-axis.
        */
        void Pitch(const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        /** Rotate the node around the Y-axis.
        */
        void Yaw(const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        /** Rotate the node around an arbitrary axis.
        */
        void Rotate(const DiVec3& axis, const DiRadian& angle, TransformSpace relativeTo = TS_LOCAL);

        /** Rotate the node around an aritrary axis using a Quarternion.
        */
        void Rotate(const DiQuat& q, TransformSpace relativeTo = TS_LOCAL);

        /** Gets a matrix whose columns are the local axes based on
        the nodes orientation relative to it's parent. */
        DiMat3 GetLocalAxes(void) const;

        virtual DiNode* CreateChild(
            SceneMemoryMgrTypes sceneType = SCENE_DYNAMIC,
            const DiVec3& translate = DiVec3::ZERO,
            const DiQuat& rotate = DiQuat::IDENTITY);

        void AddChild(DiNode* child);

        /** Reports the number of child nodes under this one.
        */
        size_t NumChildren(void) const      { return mChildren.size(); }

        /** Gets a pointer to a child node. */
        DiNode* GetChild(size_t index)                { return mChildren[index]; }

        const DiNode* GetChild(size_t index) const    { return mChildren[index]; }

        virtual void RemoveChild(DiNode* child);

        virtual void RemoveAllChildren(void);

        void SetDerivedPosition(const DiVec3& pos);

        void SetDerivedOrientation(const DiQuat& q);

        DiQuat GetDerivedOrientation(void) const;

        DiQuat GetDerivedOrientationUpdated(void);

        DiVec3 GetDerivedPosition(void) const;

        DiVec3 GetDerivedPositionUpdated(void);

        DiVec3 GetDerivedScale(void) const;

        DiVec3 GetDerivedScaleUpdated(void);

        FORCEINLINE const DiMat4& GetFullTransform(void) const
        {
            DI_ASSERT(!mCachedTransformOutOfDate);
            return mTransform.mDerivedTransform[mTransform.mIndex];
        }

        /** @See _getDerivedScaleUpdated remarks. @See _getFullTransform */
        const DiMat4& GetFullTransformUpdated(void);

        /** Sets a listener for this DiNode.
        @remarks
        Note for size and performance reasons only one listener per node is
        allowed.
        */
        virtual void SetListener(Listener* listener) { mListener = listener; }

        /** Gets the current listener for this DiNode.
        */
        Listener* GetListener(void) const { return mListener; }

        /** @See SceneManager::updateAllTransforms()
        @remarks
        We don't pass by reference on purpose (avoid implicit aliasing)
        */
        static void UpdateAllTransforms(const size_t numNodes, Transform t);

        /** Gets the local position, relative to this node, of the given world-space position */
        DiVec3 ConvertWorldToLocalPosition(const DiVec3 &worldPos);

        /** Gets the world position of a point in the node local space
        useful for simple transforms that don't require a child node.*/
        DiVec3 ConvertLocalToWorldPosition(const DiVec3 &localPos);

        /** Gets the local orientation, relative to this node, of the given world-space orientation */
        DiQuat ConvertWorldToLocalOrientation(const DiQuat &worldOrientation);

        /** Gets the world orientation of an orientation in the node local space
        useful for simple transforms that don't require a child node.*/
        DiQuat ConvertLocalToWorldOrientation(const DiQuat &localOrientation);

        /** Helper function, get the squared view depth.  */
        virtual float GetSquaredViewDepth(const DiCamera* cam) const;

        /** Manually set the mNodeMemoryManager to a null ptr.
        @remarks
        DiNode doesn't follow the rule of three. This function is useful when you make multiple
        hard copies but only the destructor must release the mTransform only slots once.
        */
        void SetNullNodeMemoryManager(void) { mNodeMemoryManager = nullptr; }

        /** Internal use, notifies all attached objects that our memory pointers
        (i.e. Transform) may have changed (e.g. during cleanups, change of parent, etc)
        */
        virtual void CallMemoryChangeListeners(void) = 0;

#ifndef NDEBUG
        virtual void SetCachedTransformOutOfDate(void);

        bool IsCachedTransformOutOfDate(void) const { return mCachedTransformOutOfDate; }
#endif
    };
}

#endif
