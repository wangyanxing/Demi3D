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
#include "TerrainNode.h"
#include "TerrainMap.h"
#include "TerrainChunk.h"
#include "Camera.h"

namespace Demi
{
    DiQuadNode::DiQuadNode(DiTerrainMap* terrain,DiQuadNode* parent,uint16 size,uint16 xoff, uint16 yoff)
        : mChunk(nullptr),
        mTerrain(terrain),
        mParent(parent),
        mChunkOffset(xoff,yoff)
    {
        if (size == 1)
        {
            memset(mChildren, 0, sizeof(DiQuadNode*) * 4);

            DiIntVec2 sz = terrain->GetChunkSize();
            if (xoff < sz.x && yoff < sz.y)
            {
                mChunk = terrain->GetChunk(xoff,yoff);
                mChunk->SetQuadNode(this);
            }
        }
        else
        {
            uint16 childSize = size / 2;
            uint16 childOff     = childSize;

            mChildren[0] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff, yoff);
            mChildren[1] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff + childOff, yoff);
            mChildren[2] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff, yoff + childOff );
            mChildren[3] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff + childOff, yoff + childOff );
        }

        mPosition = DiVec3::ZERO;
        mAABB.SetNull();
    }

    DiQuadNode::~DiQuadNode()
    {
        for (int i = 0; i < 4; ++i)
        {
            SAFE_DELETE(mChildren[i]);
        }
    }

    void DiQuadNode::MergeBoundsToParents()
    {
        DiQuadNode* parent = mParent;
        while(parent)
        {
            parent->mAABB.Merge(mAABB);
            parent = parent->mParent;
        }
    }

    void DiQuadNode::ResetBounds()
    {
        mAABB.SetNull();

        if (!IsLeaf())
        {
            for (int i = 0; i < 4; ++i)
            {
                mChildren[i]->ResetBounds();
            }
        }
    }

    bool DiQuadNode::IsLeaf()
    {
        return mChildren[0] == NULL;
    }

    void DiQuadNode::WalkQuadTree( DiCamera* camera, DiVector<int>& visibleChunk )
    {
        if (camera->IsVisible(mAABB))
        {
            if (!IsLeaf())
            {
                for (int i = 0; i < 4; ++i)
                {
                    mChildren[i]->WalkQuadTree(camera,visibleChunk);
                }
            }
            else
            {
                if (mChunk)
                {
                    visibleChunk.push_back(mChunk->GetChunkID());
                }
            }
        }
    }
}