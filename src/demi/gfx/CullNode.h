
/********************************************************************
    File:       CullNode.h
    Creator:    demiwangya
*********************************************************************/

#ifndef CullNode_h__
#define CullNode_h__

#include "Node.h"
#include "DList.h"
#include "Octree.h"
#include <functional>

namespace Demi
{
    class DI_GFX_API DiCullNode : public DiNode, public DiDListBase<DiCullNode*>
    {
    public:
        
        DiCullNode(DiSceneManager* sm);

        DiCullNode(DiSceneManager* sm,const DiString& name);

        ~DiCullNode(void);

    public:

        typedef DiStrHash<DiTransUnitPtr>     ObjectMap;
        typedef DiMapIterator<ObjectMap>      ObjectIterator;
        typedef DiConstMapIterator<ObjectMap> ConstObjectIterator;

        friend class DiSceneManager;

    public:

        static void             ProcessQueuedUpdates(void);

        DiCullNode*             CreateChild();

        DiCullNode*             CreateChild(const DiString& name);

        DiNode*                 CreateChild(const DiVec3& inTranslate, const DiQuat& inRotate);

        DiNode*                 CreateChild(const DiString& name,
                                     const DiVec3& inTranslate, const DiQuat& inRotate);

        DiNode*                 RemoveChild( unsigned short index );

        DiNode*                 RemoveChild( const DiString & name );

        DiNode*                 RemoveChild( DiNode* child);

        void                    RemoveAllChildren(void);

        void                    AttachObject(DiTransUnitPtr obj);

        void                    AttachSilently(DiTransUnitPtr obj);

        unsigned short          NumAttachedObjects(void) const;

        DiTransUnitPtr          GetAttachedObject(unsigned short index);

        DiTransUnitPtr          GetAttachedObject(const DiString& name);

        DiTransUnitPtr          DetachObject(unsigned short index);

        void                    DetachObject(DiTransUnitPtr obj);

        DiTransUnitPtr          DetachObject(const DiString& name);

        void                    DetachAllObjects(void);

        ObjectIterator          GetAttachedObjectIterator();

        ConstObjectIterator     GetAttachedObjectIterator() const;

        void                    ProcessBatchGroup(DiCamera* camera,DiRenderPipeline* pipeline, 
                                    bool onlyShadowCaster, DiVisibleObjectsBoundsInfo* visbleBounds);

        void                    ProcessVisibleObjects(std::function<void(DiTransUnitPtr)> func);

        void                    SetVisible(bool vis, bool cascade = true);

        void                    FlipVisibility(bool cascade = true);

        DiSceneManager*         GetCreator() const { return mCreator; }

        void                    _Update(bool updateChildren, bool parentHasChanged);

        DiOctreePtr             GetOctant()
        {
            return mOctant;
        }

        void                    SetOctant( DiOctreePtr o )
        {
            mOctant = o;
        }

        DiAABB&                 GetLocalAABB()
        {
            return mLocalAABB;
        }

        const DiAABB&           GetWorldAABB(void) const;


        bool                    IsIn( DiAABB &box );

    protected:

        void                    UpdateBounds();

        void                    RemoveNodeAndChildren( );

    protected:

        ObjectMap               mObjectsByName;

        DiSceneManager*         mCreator;
        
        DiAABB                  mWorldAABB;

        DiAABB                  mLocalAABB;

        DiOctreePtr             mOctant;
    };
}

#endif // CullNode_h__
