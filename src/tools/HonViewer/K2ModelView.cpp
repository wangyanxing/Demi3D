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
#include "ToolsControl.h"
#include "FilesView.h"
#include "AnimationView.h"
#include "KeyFrameBar.h"
#include "SettingsManager.h"
#include "Grid.h"

namespace Demi
{
    K2ModelViewer::K2ModelViewer()
        : mSceneNode(nullptr)
        , mPauseClip(false)
        , mLoopAnimation(true)
        , mShowBoneNames(false)
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

        mGridPlane = make_shared<DiGridPlane>(30, 10, DiColor(0.1f, 0.1f, 0.1f), DiColor(0.5f, 0.5f, 0.5f));
        mGridPlane->SetMaterial(dbgHelperMat);
        mGridPlane->SetVisible(false);
        sm->GetRootNode()->AttachObject(mGridPlane);

        mSkeleton->SetVisible(false);
        mBounds->SetVisible(false);

        MyGUI::Colour c = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColorBackground");
        DiColor backgroundColor(c.red,c.green,c.blue,c.alpha);
        SetBackgroundColor(backgroundColor);
        SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &K2ModelViewer::NotifySettingsChanged);
    }

    void K2ModelViewer::Release()
    {
        SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &K2ModelViewer::NotifySettingsChanged);
        mModel.reset();
    }

    void K2ModelViewer::Update(float dt)
    {
        if (mModel)
        {
            mModel->UpdateAnimation(mPauseClip ? -1 : dt);

            RenderWindowControl* renderWnd = HonViewerApp::GetViewerApp(
                )->getMainPane()->getMainWorkspaceControl()->getRenderWndControl();
            renderWnd->updateBoneNames(mShowBoneNames);

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
        mModel->GetAnimation()->Play(K2PrefabClip::ANIM_IDLE);

        InitHelpers();

        //notify keyframe bar to update the scale
        RenderWindowControl* renderWnd = HonViewerApp::GetViewerApp(
            )->getMainPane()->getMainWorkspaceControl()->getRenderWndControl();
        renderWnd->getKeyFrameBar()->updateScales();
        renderWnd->updateInfo();

        ToolsControl* toolsCtrl = HonViewerApp::GetViewerApp(
            )->getMainPane()->getMainWorkspaceControl()->getToolsControl();
        toolsCtrl->GetAnimationView()->UpdateClips();
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

    DiVector<DiK2Clip*> K2ModelViewer::GetClipList()
    {
        DiVector<DiK2Clip*> ret;

        if (!mModel)
            return ret;

        DiK2Animation* animation = mModel->GetAnimation();
        if (!animation)
            return ret;

        for (int i = 0; i < K2PrefabClip::MAX_PREFAB_ANIM; ++i)
        {
            DiK2Clip* clip = animation->mClips[i];
            if (clip)
                ret.push_back(clip);
        }

        for (auto i = animation->mExtraClips.begin(); i != animation->mExtraClips.end(); ++i)
            ret.push_back(i->second);

        return ret; 
    }

    void K2ModelViewer::PlayClip(DiK2Clip* clip)
    {
        if (!mModel)
            return;

        clip->mLoop = mLoopAnimation;
        mModel->GetAnimation()->Play(clip);

        //notify keyframe bar to update the scale
        RenderWindowControl* renderWnd = HonViewerApp::GetViewerApp(
            )->getMainPane()->getMainWorkspaceControl()->getRenderWndControl();
        renderWnd->getKeyFrameBar()->updateScales();
        mPauseClip = false;
    }

    int K2ModelViewer::GetNumFaces()
    {
        if (!mModel)
            return 0;

        int ret = 0;
        uint32 submodels = mModel->GetNumSubModels();
        for (uint32 i = 0; i < submodels; ++i)
        {
            ret += mModel->GetSubModel(i)->mVerticesNum;
        }
        return ret;
    }

    int K2ModelViewer::GetNumVertices()
    {
        if (!mModel)
            return 0;

        int ret = 0;
        uint32 submodels = mModel->GetNumSubModels();
        for (uint32 i = 0; i < submodels; ++i)
        {
            ret += mModel->GetSubModel(i)->mPrimitiveCount;
        }
        return ret;
    }

    int K2ModelViewer::GetNumSubModels()
    {
        if (!mModel)
            return 0;

        return (int)mModel->GetNumSubModels();
    }

    bool K2ModelViewer::ShowGrids(bool var)
    {
        mGridPlane->SetVisible(var);
        return true;
    }

    bool K2ModelViewer::SetLoopAnim(bool var)
    {
        if (!mModel)
            return false;

        if (mModel->GetAnimation()->mSource)
        {
            mModel->GetAnimation()->mSource->mLoop = var;
        }
        if (mModel->GetAnimation()->mTarget)
        {
            mModel->GetAnimation()->mTarget->mLoop = var;
        }
        mLoopAnimation = var;
        return true;
    }

    bool K2ModelViewer::ShowBoneNames(bool var)
    {
        if (!mModel)
            return false;

        mShowBoneNames = var;

        return true;
    }

    int K2ModelViewer::GetNumBones()
    {
        if (!mModel || !mModel->GetSkeleton())
            return 0;
        return mModel->GetSkeleton()->GetNumBones();
    }

    void K2ModelViewer::SetK2ResourcePack(const DiString& resPack, const DiString& texturePack)
    {
        //DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER = "00000000/";
        if (texturePack.empty())
            DiK2Configs::SetK2ResourcePack(resPack);
        else
            DiK2Configs::SetK2ResourcePack(resPack, texturePack);

        FilesView* fv = HonViewerApp::GetViewerApp(
            )->getMainPane()->getMainWorkspaceControl()->getToolsControl()->GetFilesView();
        fv->scanFiles();
    }

    void K2ModelViewer::NotifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
    {
        if (_sectorName == "Settings")
        {
            if (_propertyName == "ColorBackground")
            {
                MyGUI::Colour c = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColorBackground");
                DiColor backgroundColor(c.red, c.green, c.blue, c.alpha);
                SetBackgroundColor(backgroundColor);
            }
        }
    }

    void K2ModelViewer::SetBackgroundColor(const DiColor& color)
    {
        DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(color);
    }

}