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


#ifndef DiOctreeCuller_h__
#define DiOctreeCuller_h__

#include "SceneCuller.h"
#include "DList.h"

namespace Demi
{
    /** Octree culler
     */
    class DI_GFX_API DiOctreeCuller : public DiSceneCuller
    {
    public:
        
        DiOctreeCuller(DiSceneManager* sm);
        
        ~DiOctreeCuller();
        
    public:
        
        void            UpdateUnit(DiCullUnitPtr unit);
        
        void            Cull(DiCamera* camera);
        
        DiCullUnitPtr   CreateUnit(DiCullNode* node);
        
        void            RemoveUnit(DiCullUnitPtr unit);
        
        void            AddUnit(DiCullUnitPtr unit, DiOctreePtr octree, int depth = 0);

        void            LoadScene(const DiString& scene);
        
    protected:
        
        void            WalkTree(DiCamera* camera, DiOctreePtr octree, bool foundvisible);
        
    protected:
        
        DiOctreePtr     mOctree;
        
        int             mMaxDepth;
    };
    
    /** Octree culling unit
     */
    class DI_GFX_API DiOctreeCullUnit : public DiCullUnit,
                                        public DiDListBase<DiOctreeCullUnitPtr>
    {
    public:
        
        DiOctreeCullUnit(DiCullNode* parentNode, DiSceneCuller* culler);
        
        ~DiOctreeCullUnit();
        
    public:
        
        void            Update();
        
        DiOctreePtr     GetOctant() { return mOctant; }
        
        void            SetOctant(DiOctreePtr o) { mOctant = o; }
        
        bool            IsIn(const DiAABB& aabb);
        
    protected:
        
        DiOctreePtr     mOctant;
    };
}

#endif