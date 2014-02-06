
#include "GfxPch.h"
#include "OctreeCuller.h"
#include "TransformUnit.h"
#include "SceneManager.h"
#include "RenderPipeline.h"
#include "Camera.h"

namespace Demi
{
    const float fObjectToNodeSizeRatio = 1.f / 8.f;
    DiSet<DiOctreeNode*> DiOctreeNode::sDummyOcNodes;
    DiObjectPool<DiOctreeNode> DiOctreeNode::sOcNodePool;

    DiOctreeNode::DiOctreeNode()
        :mParent(nullptr),
        mSceneNodeNum(0)
    {
            for (int i = 0; i < 8; i++)
                mChildren[i] = nullptr;
    }

    void DiOctreeNode::Init(const DiAABB& box, DiOctreeNode* parent)
    {
        for (int i = 0; i < 8; i++)
            mChildren[i] = nullptr;

        mSceneNodeNum = 0;
        mNodeBox    = box;
        mObjectsBox = box;
        mNodeCenter = box.GetCenter();
        mNodeRadius = box.GetRadius();
        mObjectsBox.mMaximum = box.mMinimum;
        mObjectsBox.mMinimum = box.mMaximum;
    }

    DiOctreeNode::~DiOctreeNode()
    {
        //for (DiSceneNode *nd = mSceneNodes.mHead; nd; nd = nd->mNext)
        //{
        //    
        //}
        //DI_ASSERT(!mSceneNodes.mHead);

        for (int i = 0; i<8; i++)
            if (mChildren[i])
                DiOctreeNode::ReleaseOctreeNode(mChildren[i]);
    }

    void DiOctreeNode::RenderNodes(DiCamera* camera, DiRenderPipeline* pipeline)
    {
        if (!camera->IsVisible(mObjectsBox))
            return;

        DiVec3 cameraPos = camera->GetDerivedPosition();

        for (DiSceneNode* nd = mSceneNodes.mHead; nd; nd = nd->mNext)
            nd->ProcessBatchGroup(camera, pipeline);

        int nFirst =
            ((cameraPos.x > mNodeCenter.x) ? 4 : 0) |
            ((cameraPos.y > mNodeCenter.y) ? 2 : 0) |
            ((cameraPos.z > mNodeCenter.z) ? 1 : 0);

        if (mChildren[nFirst])
            mChildren[nFirst]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 1])
            mChildren[nFirst ^ 1]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 2])
            mChildren[nFirst ^ 2]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 4])
            mChildren[nFirst ^ 4]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 3])
            mChildren[nFirst ^ 3]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 5])
            mChildren[nFirst ^ 5]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 6])
            mChildren[nFirst ^ 6]->RenderNodes(camera, pipeline);

        if (mChildren[nFirst ^ 7])
            mChildren[nFirst ^ 7]->RenderNodes(camera, pipeline);
    }

    void DiOctreeNode::CleanDummyOctreeNodes()
    {
        while (!sDummyOcNodes.empty())
        {
            //DI_DEBUG("empty nodes size:%d",sDummyOcNodes.size());

            DiOctreeNode* node = *sDummyOcNodes.begin();
            sDummyOcNodes.erase(sDummyOcNodes.begin());
            if (node->IsEmpty())
            {
                DiOctreeNode* parent = node->mParent;
                for (int n = 0; n < 8; n++)
                {
                    if (parent->mChildren[n] == node)
                        parent->mChildren[n] = nullptr;
                }
                DiOctreeNode::ReleaseOctreeNode(node);

                if (parent && parent->IsEmpty() && !sDummyOcNodes.contains(parent))
                    sDummyOcNodes.insert(parent);
            }
        }
    }

    bool DiOctreeNode::IsEmpty()
    {
        if (mParent)
        if (!mChildren[0] && !mChildren[1] && !mChildren[2] && !mChildren[3])
        if (!mChildren[4] && !mChildren[5] && !mChildren[6] && !mChildren[7])
        if (!HasNodes())
            return true;
        return false;
    }

    void DiOctreeNode::InsertSceneNode(DiSceneNode* node, const DiAABB& aabb, float objRadiusSqr, const DiVec3& objCenter)
    {
        DiOctreeNode* currentOct = this;
        DiVec3 objCt = objCenter;
        int depth = 0;

        while (true)
        {
            currentOct->mObjectsBox.Merge(aabb);
            //currentOct->mObjectsMaxViewDist = DiMath::Max(currentOct->mObjectsMaxViewDist, fWSMaxViewDist);
            if (depth < 5)
            {
                if (objRadiusSqr < DiMath::Sqr(currentOct->mNodeRadius * fObjectToNodeSizeRatio))
                {
                    int nChildId =
                        ((objCt.x > currentOct->mNodeCenter.x) ? 4 : 0) |
                        ((objCt.y > currentOct->mNodeCenter.y) ? 2 : 0) |
                        ((objCt.z > currentOct->mNodeCenter.z) ? 1 : 0);

                    if (!currentOct->mChildren[nChildId])
                    {
                        currentOct->mChildren[nChildId] = DiOctreeNode::GetOctreeNode();
                        currentOct->mChildren[nChildId]->Init(currentOct->GetChildAABB(nChildId), currentOct);
                    }

                    currentOct = currentOct->mChildren[nChildId];    
                    depth++;
                    continue;
                } 
            }
            break;
        }

        currentOct->Link(node);
        node->mOctreeNode = currentOct;
    }

    DiOctreeNode* DiOctreeNode::GetOctreeNode()
    {
        return sOcNodePool.GetFreeObject();
    }

    void DiOctreeNode::ReleaseOctreeNode(DiOctreeNode* node)
    {
        sOcNodePool.ReleaseObject(node);
    }

    void DiOctreeNode::ReleaseAll()
    {
        sOcNodePool.ReleaseAll();
    }

    DiAABB DiOctreeNode::GetChildAABB(int ID)
    {
        int x = (ID / 4);
        int y = (ID - x * 4) / 2;
        int z = (ID - x * 4 - y * 2);
        DiVec3 vSize = mNodeBox.GetSize() * 0.5f;
        DiVec3 vOffset = vSize;
        vOffset.x *= x;
        vOffset.y *= y;
        vOffset.z *= z;
        DiVec3 min = mNodeBox.mMinimum + vOffset;
        DiAABB childBox(min, min + vSize);
        return childBox;
    }

    void DiOctreeNode::DeleteSceneNode(DiSceneNode* node)
    {
        if (node->mOctreeNode && node->mOctreeNode != this)
            return node->mOctreeNode->DeleteSceneNode(node);

        Unlink(node);

        node->mOctreeNode = nullptr;

        if (IsEmpty())
        {
            printf("empty node\n");
            sDummyOcNodes.insert(this);
        }
    }

    void DiOctreeNode::Link(DiSceneNode* node)
    {
        mSceneNodeNum++;
        mSceneNodes.push_back(node);
    }

    void DiOctreeNode::Unlink(DiSceneNode* node)
    {
        if (node->mNext || node->mPrev || node == mSceneNodes.mTail || node == mSceneNodes.mHead)
        {
            mSceneNodes.erase(node);
            mSceneNodeNum--;
        }
    }

    bool DiOctreeNode::HasNodes() const
    {
        if (mSceneNodes.mHead)
            return true;
        return false;
    }

    bool DiOctreeNode::Cleanup()
    {
        bool hasChildren = false;
        for (int i = 0; i < 8; i++)
        {
            if (mChildren[i])
            {
                if (!mChildren[i]->Cleanup())
                {
                    DiOctreeNode::ReleaseOctreeNode(mChildren[i]);
                    mChildren[i] = nullptr;
                }
                else
                    hasChildren = true;
            }
        }

        mObjectsMaxViewDist = 0;
        mObjectsBox = mNodeBox;

        for (DiSceneNode* nd = mSceneNodes.mHead; nd; nd = nd->mNext)
        {
            //nd->mWSMaxViewDist = nd->GetMaxViewDist();
            mObjectsMaxViewDist = 1000000;//DiMath::Max(mObjectsMaxViewDist, nd->mWSMaxViewDist);
            mObjectsBox.Merge(nd->GetWorldAABB());
        }

        for (int i = 0; i<8; i++)
        {
            if (mChildren[i])
            {
                mObjectsMaxViewDist = DiMath::Max(mObjectsMaxViewDist, mChildren[i]->mObjectsMaxViewDist);
                mObjectsBox.Merge(mChildren[i]->mObjectsBox);
            }
        }

        return hasChildren || HasNodes();
    }

}