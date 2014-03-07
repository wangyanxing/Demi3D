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

#ifndef DiQ3BspNode_h__
#define DiQ3BspNode_h__

#include "SceneQuery.h"

namespace Demi
{
    class DiBspNode
    {
    public:

        DiBspNode(DiBspScene* scene, bool isLeaf);

        DiBspNode();

        ~DiBspNode();

        friend class DiBspScene;

    public:

        bool            IsLeaf(void) const { return mIsLeaf; }

        DiBspNode*      GetFront(void) const;

        DiBspNode*      GetBack(void) const;

        DiPlane::Side   GetSide(const DiVec3& point) const;

        DiBspNode*      GetNextNode(const DiVec3& point) const;

        const DiPlane&  GetSplitPlane(void) const;

        const DiAABB&   GetBoundingBox(void) const;

        int             GetNumFaceGroups(void) const;

        int             GetFaceGroupStart(void) const;

        bool            IsLeafVisible(const DiBspNode* leaf) const;

        float           GetDistance(const DiVec3& pos) const;
        
        void            AddTransUnit(const DiTransUnitPtr tu);
        
        void            RemoveTransUnit(const DiTransUnitPtr tu);

        typedef DiSet<DiTransUnitPtr> IntersectingObjectSet;
        
        struct Brush
        {
            DiList<DiPlane> planes;
            DiSceneQuery::WorldFragment fragment; /// For query reporting
        };
        typedef DiVector<Brush*> NodeBrushList;

        const NodeBrushList&            GetSolidBrushes(void) const;
        
        const IntersectingObjectSet&    GetObjects(void) const { return mMovables; }
        
    private:
        
        DiBspScene*     mParent;
        
        bool            mIsLeaf;
        
        DiPlane         mSplitPlane;
        
        DiBspNode*      mFront;

        DiBspNode*      mBack;
        
        int             mVisCluster;
        
        DiAABB          mBounds;
        
        int             mNumFaceGroups;
        
        int             mFaceGroupStart;
        
        IntersectingObjectSet mMovables;
        
        NodeBrushList   mSolidBrushes;
    };
}

#endif
