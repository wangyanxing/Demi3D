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

#ifndef DiNewBone_h__
#define DiNewBone_h__

#include "GfxPrerequisites.h"
#include "Math/Array/BoneTransform.h"

namespace Demi
{
    class DI_GFX_API DiNewBone : public DiBase
    {
    public:
        /** Index in the vector holding this node reference (could be our parent node, or a global array
        tracking all created nodes to avoid memory leaks). Used for O(1) removals.
        @remarks
        It is the parent (or our creator) the one that sets this value, not ourselves. Do NOT modify
        it manually.
        */
        size_t mGlobalIndex;

        size_t mParentIndex;

        DiNewBone();
        virtual ~DiNewBone();

        void _initialize(uint32 id, BoneMemoryManager *boneMemoryManager,
            DiNewBone *parent, ArrayMatrixAf4x3 const * RESTRICT_ALIAS reverseBind);
        void _deinitialize(void);

        /// Returns how deep in the hierarchy we are (eg. 0 -> root node, 1 -> child of root)
        uint16 getDepthLevel() const                                { return mDepthLevel; }

        /// Returns a direct access to the Transform state
        BoneTransform& _getTransform()                              { return mTransform; }

        /// Internal use. Called from BoneMemoryManager's rebases (i.e. cleanups, grows)
        void _memoryRebased(void);

        void _setReverseBindPtr(const ArrayMatrixAf4x3 *ptr)      { mReverseBind = ptr; }

        /// Sets a custom name for this node. Doesn't have to be unique
        void setName(const DiString &name)                          { mName = name; }

        /// Returns the name of the node.
        const DiString& getName(void) const                           { return mName; }

        /// Gets this Bones's parent (NULL if this is the root).
        DiNewBone* getParent(void) const                                 { return mParent; }

        /** Sets a regular Node to be parent of this bone
        @remarks
        1. Multiple calls to _setNodeParent with different arguments will
        silently override previous calls.
        2. By the time we update, we assume the Node has already been updated.
        (even when calling _getDerivedPositionUpdated and Co)
        3. Null pointers will "detach", causing derived updates to be in local space
        4. We must ensure that when a NodeMemoryManager performs a cleanup (or resizes),
        this function is called again (to update our pointers).
        */
        void _setNodeParent(DiNode *nodeParent);

        inline void setOrientation(DiQuat q);

        inline DiQuat getOrientation() const;

        inline void setPosition(const DiVec3& pos);

        inline DiVec3 getPosition(void) const;

        inline void setScale(const DiVec3& pos);

        inline DiVec3 getScale(void) const;

        void setInheritOrientation(bool inherit);

        bool getInheritOrientation(void) const;

        void setInheritScale(bool inherit);

        bool getInheritScale(void) const;

        FORCEINLINE const SimpleMatrixAf4x3& _getLocalSpaceTransform(void) const
        {
            return mTransform.mDerivedTransform[mTransform.mIndex];
        }

        /** Gets the full transformation matrix for this node.
        @remarks
        This method returns the full transformation matrix
        for this node, including the effect of any parent DiNewBone
        transformations.
        @par
        Assumes the caches are already updated.
        @par
        The transform is in "world bone" space, unless our root parent called
        _setNodeParent( nullptr ) in which case the transform will be in
        local bone space.
        */
        FORCEINLINE const SimpleMatrixAf4x3& _getFullTransform(void) const
        {
            return mTransform.mFinalTransform[mTransform.mIndex];
        }

        /** @See _getDerivedScaleUpdated remarks. @See _getFullTransform */
        const SimpleMatrixAf4x3& _getFullTransformUpdated(void);

        /** TODO
        */
        /*virtual SceneNode* createChildSceneNode(
        SceneMemoryMgrTypes sceneType = SCENE_DYNAMIC,
        const DiVec3& translate = DiVec3::ZERO,
        const DiQuat& rotate = DiQuat::IDENTITY );*/

        /// @See Node::updateAllTransforms
        static void updateAllTransforms(const size_t numNodes, BoneTransform t,
            ArrayMatrixAf4x3 const * RESTRICT_ALIAS reverseBind,
            size_t numBinds);

        typedef DiVector<DiNewBone*> BoneVec;
    protected:
        ArrayMatrixAf4x3 const * RESTRICT_ALIAS mReverseBind;
        BoneTransform                           mTransform;

        /// Depth level in the hierarchy tree (0: Root node, 1: Child of root, etc)
        uint16      mDepthLevel;
        /// Pointer to parent node
        DiNewBone*  mParent;
        /// Collection of pointers to direct children
        BoneVec     mChildren;

        DiString    mName;

        /// The memory manager used to allocate the Transform.
        BoneMemoryManager *mBoneMemoryManager;

        /// @copydoc Node::_updateFromParent
        void _updateFromParent(void);

        /// @copydoc Node::updateFromParentImpl.
        void updateFromParentImpl(void);

        void setCachedTransformOutOfDate(void) const;

        void resetParentTransformPtr(void);

        /** For debug use ONLY. Bones don't support dynamically changing their hierarchy structure.
        It can mess with the memory layout of neighbouring SkeletonInstances
        */
        void removeChild(DiNewBone* child);
    };

#define CACHED_TRANSFORM_OUT_OF_DATE() ((void)0)

    inline DiQuat DiNewBone::getOrientation() const
    {
        return mTransform.mOrientation->getAsQuaternion(mTransform.mIndex);
    }
    
    inline void DiNewBone::setOrientation(DiQuat q)
    {
        DI_ASSERT(!q.isNaN() && "Invalid orientation supplied as parameter");
        q.normalise();
        mTransform.mOrientation->setFromQuaternion(q, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    inline void DiNewBone::setPosition(const DiVec3& pos)
    {
        DI_ASSERT(!pos.isNaN() && "Invalid vector supplied as parameter");
        mTransform.mPosition->setFromVector3(pos, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    inline DiVec3 DiNewBone::getPosition(void) const
    {
        return mTransform.mPosition->getAsVector3(mTransform.mIndex);
    }
    
    inline void DiNewBone::setScale(const DiVec3& scale)
    {
        DI_ASSERT(!scale.isNaN() && "Invalid vector supplied as parameter");
        mTransform.mScale->setFromVector3(scale, mTransform.mIndex);
        CACHED_TRANSFORM_OUT_OF_DATE();
    }
    
    inline DiVec3 DiNewBone::getScale(void) const
    {
        return mTransform.mScale->getAsVector3(mTransform.mIndex);
    }
}

#endif
