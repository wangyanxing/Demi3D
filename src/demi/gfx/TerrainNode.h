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
