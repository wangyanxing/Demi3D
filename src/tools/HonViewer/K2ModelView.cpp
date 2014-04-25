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

#include "ViewerPch.h"
#include "K2ModelView.h"
#include "SubModel.h"
#include "DebugHelper.h"
#include "HonViewerApp.h"
#include "MainPaneControl.h"
#include "MainWorkspaceControl.h"
#include "RenderWindowControl.h"
#include "KeyFrameBar.h"

namespace Demi
{
    K2ModelViewer::K2ModelViewer()
        : mSceneNode(nullptr)
        , mPauseClip(false)
    {
        Init();
    }

    K2ModelViewer::~K2ModelViewer()
    {
        Release();
    }

    void K2ModelViewer::Init()
    {
        DiSceneManager* sm = Driver->GetSceneManager();
        mSceneNode = sm->GetRootNode()->CreateChild();

        DiMaterialPtr dbgHelperMat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        dbgHelperMat->SetDepthCheck(false);

        mBounds = make_shared<DiDebugHelper>();
        mBounds->SetMaterial(dbgHelperMat);

        mSkeleton = make_shared<DiDebugHelper>();
        mSkeleton->SetMaterial(dbgHelperMat);

        mSkeleton->SetVisible(false);
        mBounds->SetVisible(false);
    }

    void K2ModelViewer::Release()
    {
        mModel.reset();
    }

    void K2ModelViewer::Update(float dt)
    {
        if (mModel)
        {
            mModel->UpdateAnimation(mPauseClip ? -1 : dt);
            UpdateSkeletonHelper();
            UpdateBoundsHelper(); 
        }
    }

    void K2ModelViewer::LoadModel(const DiString& file)
    {
        if (mCurrentFile == file)
            return;

        mCurrentFile = file;

        mSceneNode->DetachAllObjects();
        mModel = make_shared<DiK2Model>(file);
        mSceneNode->AttachObject(mModel);
        mSceneNode->SetPosition(0, -50, 0);
        mModel->GetAnimation()->Play(K2PrefabClip::ANIM_IDLE);

        InitHelpers();

        //notify keyframe bar to update the scale
        RenderWindowControl* renderWnd = HonViewerApp::GetViewerApp()->getMainPane()->getMainWorkspaceControl()->getRenderWndControl();
        renderWnd->getKeyFrameBar()->updateScales();
    }

    bool K2ModelViewer::SetWireframe(bool var)
    {
        if (!mModel)
            return false;

        uint32 numSubModels = mModel->GetNumSubModels();
        for (uint32 i = 0; i < numSubModels; ++i)
        {
            DiSubModel* sm = mModel->GetSubModel(i);
            if (sm->mMaterial)
            {
                sm->mMaterial->SetWireframe(var);
            }
        }

        return true;
    }

    bool K2ModelViewer::ShowBones(bool var)
    {
        if (!mModel || !mSkeleton)
            return false;
        mSkeleton->SetVisible(var);
        return true;
    }

    bool K2ModelViewer::ShowBounds(bool var)
    {
        if (!mModel || !mBounds)
            return false;
        mBounds->SetVisible(var);
        return true;
    }

    bool K2ModelViewer::ShowModel(bool var)
    {
        if (!mModel)
            return false;

        mModel->SetVisible(var);

        return true;
    }

    void K2ModelViewer::InitHelpers()
    {
        if (!mModel)
            return;

        mBounds->Clear();
        mSceneNode->AttachObject(mBounds);

        mSkeleton->Clear();
        mSceneNode->AttachObject(mSkeleton);
    }

    void K2ModelViewer::UpdateSkeletonHelper()
    {
        if (!mSkeleton || !mSkeleton->GetVisible())
            return;

        mSkeleton->Clear();
        auto skel = mModel->GetSkeleton();

        uint32 numBons = skel->GetNumBones();
        for (uint32 i = 0; i < numBons; ++i)
        {
            const DiNode* b = skel->GetBone(i);
            DiNode* p = b->GetParent();
            DiVec3 pos = b->GetDerivedPosition();
            DiVec3 posParent = p ? (p->GetDerivedPosition()) : pos;
            mSkeleton->AddLine(pos, posParent, DiColor::Red);
            mSkeleton->AddBoundingBox(pos, 0.2f, DiColor::Yellow);
        }
    }

    void K2ModelViewer::UpdateBoundsHelper()
    {
        if (!mBounds || !mBounds->GetVisible())
            return;
        mBounds->Clear();
        mBounds->AddBoundingBox(mSceneNode->GetLocalAABB(), DiColor::Blue);
    }

    DiK2Clip* K2ModelViewer::GetCurrentClip()
    {
        return mModel ? mModel->GetAnimation()->mSource : nullptr;
    }

}