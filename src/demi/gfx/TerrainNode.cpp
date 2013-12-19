
#include "TerrainNode.h"
#include "TerrainMap.h"
#include "TerrainChunk.h"
#include "Camera.h"

namespace Demi
{
    DiQuadNode::DiQuadNode(DiTerrainMap* terrain,DiQuadNode* parent,uint16 size,uint16 xoff, uint16 yoff)
        : mChunk(NULL),
        mTerrain(terrain),
        mParent(parent),
        mChunkOffset(xoff,yoff)
    {
        // 叶子节点
        if (size == 1)
        {
            memset(mChildren, 0, sizeof(DiQuadNode*) * 4);

            // 是否有对应chunk
            DiIntVec2 size = terrain->GetChunkSize();
            if (xoff < size.x && yoff < size.y)
            {
                mChunk = terrain->GetChunk(xoff,yoff);
                mChunk->SetQuadNode(this);
            }
        }
        else
        {
            uint16 childSize = size / 2;
            uint16 childOff     = childSize;

            mChildren[0] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff,            yoff             );
            mChildren[1] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff + childOff,    yoff             );
            mChildren[2] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff,            yoff + childOff );
            mChildren[3] = DI_NEW(DiQuadNode)(terrain, this, childSize, xoff + childOff,    yoff + childOff );
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