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
#include "SceneCuller.h"
#include "Q3BspCuller.h"
#include "Q3BspScene.h"
#include "Q3BspNode.h"
#include "Q3BspPatch.h"
#include "Material.h"

namespace Demi
{
    DiBspCullUnit::DiBspCullUnit(DiCullNode* parentNode, DiSceneCuller* culler)
        :DiCullUnit(parentNode, culler)
    {
    }

    DiBspCullUnit::~DiBspCullUnit()
    {

    }

    void DiBspCullUnit::Update()
    {

    }

    bool DiBspCullUnit::IsIn(const DiAABB& aabb)
    {
        return false;
    }

    DiBspSceneCuller::DiBspSceneCuller(DiSceneManager* sm)
        : DiSceneCuller(sm)
        , mLevel(nullptr)
    {
        mTestMat = DiMaterial::QuickCreate("basic_v", "basic_p");
    }

    DiBspSceneCuller::~DiBspSceneCuller()
    {

    }

    Demi::DiCullUnitPtr DiBspSceneCuller::CreateUnit(DiCullNode* node)
    {
        return nullptr;
    }

    void DiBspSceneCuller::Cull(DiCamera* camera)
    {
        DI_ASSERT(mLevel);

        mMatFaceGroupMap.clear();
        mFaceGroupSet.clear();

        DiBspNode* cameraNode = mLevel->findLeaf(camera->GetDerivedPosition());

        // Scan through all the other leaf nodes looking for visibilities
        int i = mLevel->mNumNodes - mLevel->mLeafStart;
        DiBspNode* nd = mLevel->mRootNode + mLevel->mLeafStart;

        while (i--)
        {
            if (mLevel->isLeafVisible(cameraNode, nd))
            {
                // Visible according to PVS, check bounding box against frustum
                DiFrustumPlane plane;
                if (camera->IsVisible(nd->GetBoundingBox(), &plane))
                {
                    ProcessVisibleLeaf(nd, camera);
                }
            }
            nd++;
        }

        ProcessVisibilities();
    }

    void DiBspSceneCuller::ProcessVisibleLeaf(DiBspNode* leaf, DiCamera* camera)
    {
        int numGroups = leaf->GetNumFaceGroups();
        int idx = leaf->GetFaceGroupStart();

        while (numGroups--)
        {
            int realIndex = mLevel->mLeafFaceGroups[idx++];
            // Check not already included
            if (mFaceGroupSet.find(realIndex) != mFaceGroupSet.end())
                continue;

            Q3BspFaceRenderer* faceGroup = mLevel->mBspFaces + realIndex;

            std::pair<MaterialFaceGroupMap::iterator, bool> matgrpi;
            matgrpi = mMatFaceGroupMap.insert(
                MaterialFaceGroupMap::value_type(mTestMat.get(), DiVector<Q3BspFaceRenderer*>()));
            matgrpi.first->second.push_back(faceGroup);

            mFaceGroupSet.insert(realIndex);
        }
    }

    void DiBspSceneCuller::UpdateUnit(DiCullUnitPtr unit)
    {
    }

    void DiBspSceneCuller::RemoveUnit(DiCullUnitPtr unit)
    {
    }

    void DiBspSceneCuller::LoadScene(const DiString& scene)
    {
        if (mLevel)
            DI_DELETE mLevel;

        mLevel = DI_NEW DiBspScene(scene);
        mLevel->Load();


    }

    void DiBspSceneCuller::ProcessVisibilities()
    {
        mSceneManager->GetMainVisibleObjects().objs.push_back(DiTransUnitPtr(mLevel->mRenderUnit));
        
        mLevel->mRenderUnit->ClearUnits();

        DiRenderUnit* ru = DI_NEW DiRenderUnit();
        mLevel->mRenderUnit->mRenderUnits.push_back(ru);
        ru->mIndexBuffer = mLevel->mRenderUnit->mIndexBuffer;
        ru->mVertexDecl = mLevel->mRenderUnit->mVertexDecl;
        ru->mSourceData.push_back(mLevel->mRenderUnit->mVertexBuffer);
        ru->mPrimitiveType = PT_TRIANGLELIST;
        ru->mIndexOffset = 0;

    }

    bool DiBspSceneCuller::materialLess::operator()(const DiMaterial* x, const DiMaterial* y) const
    {
        if (x->IsTransparent() && !y->IsTransparent())
            return false;
        else if (!x->IsTransparent() && y->IsTransparent())
            return true;
        else
            return x < y;
    }
}