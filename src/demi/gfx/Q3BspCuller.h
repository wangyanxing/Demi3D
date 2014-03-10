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

#ifndef DiQ3BspCuller_h__
#define DiQ3BspCuller_h__

#include "SceneCuller.h"

namespace Demi
{
    class DiBspScene;
    struct Q3BspFaceRenderer;

    /** Bsp scene culler
     */
    class DiBspSceneCuller : public DiSceneCuller
    {
    public:

        DiBspSceneCuller(DiSceneManager* sm);

        ~DiBspSceneCuller();

    public:

        DiCullUnitPtr   CreateUnit(DiCullNode* node);

        void            Cull(DiCamera* camera);

        void            ProcessVisibilities();

        void            UpdateUnit(DiCullUnitPtr unit);

        void            RemoveUnit(DiCullUnitPtr unit);
        
        void            ProcessVisibleLeaf(DiBspNode* nd, DiCamera* camera);

        void            LoadScene(const DiString& scene);

        struct materialLess
        {
            bool operator()(const DiMaterial* x, const DiMaterial* y) const;
        };

    protected:

        // State variables for rendering WIP
        // Set of face groups (by index) already included
        typedef DiSet<int> FaceGroupSet;
        FaceGroupSet            mFaceGroupSet;

        // Material -> face group hashmap
        typedef std::map<DiMaterial*, DiVector<Q3BspFaceRenderer*>, materialLess > MaterialFaceGroupMap;
        MaterialFaceGroupMap    mMatFaceGroupMap;

        DiBspScene*             mLevel;

        DiMaterialPtr           mTestMat;
    };

    /** Bsp scene node
     */
    class DiBspCullUnit : public DiCullUnit
    {
    public:

        DiBspCullUnit(DiCullNode* parentNode, DiSceneCuller* culler);

        ~DiBspCullUnit();

    public:

        void            Update();

        bool            IsIn(const DiAABB& aabb);

    protected:

    };
}

#endif
