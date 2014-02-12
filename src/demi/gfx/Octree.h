


#ifndef DiOctree_h__
#define DiOctree_h__

#include "DList.h"

namespace Demi
{
    typedef shared_ptr<DiOctree> DiOctreePtr;

    class DI_GFX_API DiOctree : public DiBase, public enable_shared_from_this<DiOctree>
    {
    public:

        DiOctree(DiOctreePtr parent, DiOctreePtr root, uint16 index);

        ~DiOctree();

    public:

        void          AddNode(DiCullNode* node);
                      
        void          RemoveNode(DiCullNode* node);
                      
        void          DeleteChild(uint16 id)
        {
            mChildren[id].reset();
        }

        int           NumNodes()
        {
            return mNumNodes;
        }

        bool          IsTwiceSize( const DiAABB&box ) const;

        uint16        GetChildIndexes( const DiAABB& ) const;

        void          GetCullBounds( DiAABB* ) const;

        typedef DiDList<DiCullNode*> NodeList;
        NodeList      mNodes;

        DiAABB        mBox;

        DiVec3        mHalfSize;

        DiOctreePtr   mChildren[8];

        uint16        mIndex;

    protected:

        inline void IncreaseNodeCount()
        {
            mNumNodes++;

            if (mParent) 
                mParent->IncreaseNodeCount();
        }

        inline void DecreaseNodeCount()
        {
            mNumNodes--;
            if (mNumNodes == 0)
            {
                //if (mParent)
                   // mParent->DeleteChild(mIndex);
            }

            if (mParent)
                mParent->DecreaseNodeCount();
        }

        int mNumNodes;

        DiOctreePtr mParent;
        DiOctreePtr mRoot;
    };
}

#endif
