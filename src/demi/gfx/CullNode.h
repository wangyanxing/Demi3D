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

#ifndef CullNode_h__
#define CullNode_h__

#include "Node.h"
#include "SceneCuller.h"
#include <functional>

namespace Demi
{
    class DI_GFX_API DiCullNode : public DiNode
    {
    public:
        
        DiCullNode(DiSceneManager* sm);

        DiCullNode(DiSceneManager* sm,const DiString& name);

        ~DiCullNode(void);

    public:

        typedef DiVector<DiTransUnitPtr>         ObjectMap;
        typedef DiVectorIterator<ObjectMap>      ObjectIterator;
        typedef DiConstVectorIterator<ObjectMap> ConstObjectIterator;

        friend class DiSceneManager;

    public:

        static void             ProcessQueuedUpdates(void);

        DiCullNode*             CreateChild();

        DiCullNode*             CreateChild(const DiString& name);

        DiNode*                 CreateChild(const DiVec3& inTranslate, const DiQuat& inRotate);

        DiNode*                 CreateChild(const DiString& name,
                                     const DiVec3& inTranslate, const DiQuat& inRotate);

        DiNode*                 RemoveChild( uint32 index );

        DiNode*                 RemoveChild( const DiString & name );

        DiNode*                 RemoveChild( DiNode* child);

        void                    RemoveAllChildren(void);

        /** Attach the object and notify
         */
        void                    AttachObject(DiTransUnitPtr obj);

        /** Attach the object without sending notification
         */
        void                    AttachSilently(DiTransUnitPtr obj);

        uint32                  NumAttachedObjects(void) const;

        DiTransUnitPtr          GetAttachedObject(uint32 index);

        /** Shoud not be called frequently
         */
        DiTransUnitPtr          DetachObject(uint32 index);

        /** Shoud not be called frequently
         */
        void                    DetachObject(DiTransUnitPtr obj);
        
        void                    DetachObject(DiTransformUnit* obj);

        void                    DetachAllObjects(void);

        /** Get an external iterator wrapper
         */
        ObjectIterator          GetAttachedObjectIterator();

        ConstObjectIterator     GetAttachedObjectIterator() const;

        void                    ProcessVisibleObjects(std::function<void(DiTransUnitPtr)> func);

        /** Set the visibility for all attached objects
         */
        void                    SetVisible(bool vis, bool cascade = true);

        void                    FlipVisibility(bool cascade = true);

        DiSceneManager*         GetCreator() const { return mCreator; }

        /** Update the node tree
         */
        void                    _Update(bool updateChildren, bool parentHasChanged);

        DiAABB&                 GetLocalAABB() { return mLocalAABB; }

        const DiAABB&           GetWorldAABB(void) const;
        
        DiCullUnitPtr           GetCullUnit() {return mCullUnit;}

        /** Test if this node is in a bounding box
         */
        bool                    IsIn( DiAABB &box );

        bool                    IsCulled() const { return mIsCulled; }

        void                    SetCulled(bool val) { mIsCulled = val; }

    protected:

        void                    UpdateBounds();

        void                    RemoveNodeAndChildren();

    protected:

        ObjectMap               mObjects;

        DiSceneManager*         mCreator;
        
        DiAABB                  mWorldAABB;

        DiAABB                  mLocalAABB;
        
        DiCullUnitPtr           mCullUnit;

        bool                    mIsCulled;
    };
}

#endif // CullNode_h__
