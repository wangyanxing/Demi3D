


#ifndef DiTerrainNode_h__
#define DiTerrainNode_h__


namespace Demi
{
    class DI_GFX_API DiQuadNode
    {
    public:

        DiQuadNode(DiTerrainMap* terrain,DiQuadNode* parent,uint16 size,uint16 xoff, uint16 yoff);

        virtual         ~DiQuadNode();

    public:

        DiAABB&         GetBounds() {return mAABB;}

        void            ResetBounds();

        bool            IsLeaf();

        void            MergeBoundsToParents();

        void            WalkQuadTree(DiCamera* camera, DiVector<int>& visibleChunk);

    protected:

        DiTerrainMap*   mTerrain;

        DiQuadNode*     mParent;

        DiQuadNode*     mChildren[4];

        DiAABB          mAABB;

        DiVec3          mPosition;

        DiTerrainChunk* mChunk;

        DiIntVec2       mChunkOffset;
    };
}

#endif
