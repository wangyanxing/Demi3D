
/********************************************************************
    File:     OctreeCuller.h
    Creator: demiwangya
*********************************************************************/

#pragma once

#include "SceneNode.h"
#include "Pool.h"

namespace Demi
{
    static const int NUM_OCTANTS = 8;
    static const unsigned ROOT_INDEX = 0xffffffff;
    
    class DiOctantRoot;

    class DiOctreeNode
    {
    public:

        DiOctreeNode();

        virtual         ~DiOctreeNode();

        friend class DiSceneManager;

    public:

        void            Init(const DiAABB& box, DiOctreeNode* parent);

        bool            Cleanup();

        bool            HasNodes() const;

        void            InsertSceneNode(DiSceneNode* node, const DiAABB& aabb, float objRadiusSqr, const DiVec3& objCenter);

        void            DeleteSceneNode(DiSceneNode* node);

        void            RenderNodes(DiCamera* camera, DiRenderPipeline* pipeline);

        bool             IsEmpty();

        static void     CleanDummyOctreeNodes();

        static void     ReleaseAll();

        static DiOctreeNode* GetOctreeNode();

        static void     ReleaseOctreeNode(DiOctreeNode* node);

    protected:

        void            Link(DiSceneNode* node);

        void            Unlink(DiSceneNode* node);

        DiAABB          GetChildAABB(int ID);

    protected:

        typedef DiDList<DiSceneNode*> SceneNodes;

        SceneNodes      mSceneNodes;

        uint32          mSceneNodeNum;

        DiAABB          mNodeBox;

        DiVec3          mNodeCenter;

        float           mNodeRadius;

        DiOctreeNode*   mChildren[8];

        DiOctreeNode*   mParent;

        float           mNodeDistance;

        float           mObjectsMaxViewDist;

        DiAABB          mObjectsBox;

        static DiSet<DiOctreeNode*> sDummyOcNodes;

        static DiObjectPool<DiOctreeNode> sOcNodePool;
    };
}