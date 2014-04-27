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
#include "RenderWindow.h"
#include "GfxDriver.h"
#include "RenderTarget.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "GBuffer.h"
#include "RenderPipeline.h"
#include "PostEffectManager.h"
#include "Window.h"
#include "Command.h"
#include "ConsoleVariable.h"

//#define DISABLE_POST

namespace Demi 
{
    DiRenderWindow* DiRenderWindow::ActiveWindow = nullptr;

    DiRenderWindow::DiRenderWindow(const DiString& name)
        : mSceneManager(nullptr)
        , mSceneCanvas(nullptr)
        , mRenderBuffer(nullptr)
        , mMainCamera(nullptr)
        , mPostEffectMgr(nullptr)
        , mName(name)
        , mEnable(true)
        , mCurrentBatchesNum(0)
        , mCurrentPrimitivesNum(0)
        , mGBuffer(nullptr)
        , mWidth(0)
        , mHeight(0)
        , mWndHandle(nullptr)
        , mWindow(nullptr)
        , mFrameNumber(0)
    {
    }

    DiRenderWindow::~DiRenderWindow()
    {
        Shutdown();
    }

    void DiRenderWindow::AddUpdateRenderTarget( DiRenderTarget* rt )
    {
        if (mUpdateRTs.contains(rt))
            return;
        mUpdateRTs.push_back(rt);
    }

    void DiRenderWindow::RemoveUpdateRenderTarget( DiRenderTarget* rt )
    {
        UpdateRTs::iterator it = mUpdateRTs.find(rt);
        if (it != mUpdateRTs.end())
            mUpdateRTs.erase(it);
    }

    void DiRenderWindow::Render()
    {
        if (!mEnable)
            return;

        ActiveRenderWindow = this;

        DI_PROFILE(RenderWindow_Render);

        if (mWindow)
            mWindow->Update();

        mCurrentPrimitivesNum = 0;
        mCurrentBatchesNum = 0;

        DiRenderPipeline* rp = Driver->GetPipeline();

        mSceneManager->PreUpdate();

        DiCamera* mainCam = mSceneManager->GetCamera();

        // Cull first
        mSceneManager->SetCurrentPass(GEOMETRY_PASS);
        mSceneManager->Cull(mainCam);
        mSceneManager->GetMainVisibleObjects().UpdateAll(mainCam);

        if (mUpdateCb)
            mUpdateCb();

        // Shadow mapping pass
        rp->SetCurrentPass(DiRenderPipeline::P_SHADOW_PASS);
        auto& visLights = mSceneManager->GetVisibleLights();
        visLights.SetupShaodwCamera(mSceneManager);
        for (auto i = visLights.dirLights.begin(); i != visLights.dirLights.end(); ++i)
        {
            DiLight* light = *i;
            if (light->GetShadowCastEnable())
            {
                light->ApplyGeneralShaderConfigs();
                for (int cascadeId = 0; cascadeId < MAX_CASCADE_SPLITS; ++cascadeId)
                {
                    DiCamera* cam = light->mShadowCameras[cascadeId];
                    rp->ClearGroup();
                    mSceneManager->SetCurrentPass(SHADOW_PASS);
                    mSceneManager->Cull(cam);
                    mSceneManager->GetVisibleObjects().AddToBatch(rp);
                    rp->Render(mSceneManager, cam, light->mShadowTextures[cascadeId]->GetRenderTarget());
                }
            }
         }

        // Normal geometry pass
        rp->ClearGroup();
        mSceneManager->GetMainVisibleObjects().AddToBatch(rp);
        rp->SetCurrentPass(DiRenderPipeline::P_LIGHTING_PASS);
        
#ifndef DISABLE_POST
        rp->Render(mSceneManager, mainCam, mSceneCanvas);
#else
        rp->Render(mSceneManager, mainCam, mRenderBuffer);
#endif

        // Process the extra render targets
        rp->SetCurrentPass(DiRenderPipeline::P_CUSTOM_RTT_PASS);
        auto rts = mSceneManager->GetExtraRenderTargets();
        for (auto it = rts.begin(); it != rts.end(); ++it) 
        {
            if (it->preUpdateCallback)
                it->preUpdateCallback(it->rt);

            rp->ClearGroup();
            mSceneManager->SetCurrentPass(RTT_PASS);
            mSceneManager->Cull(it->camera);
            mSceneManager->GetVisibleObjects().AddToBatch(rp);
            rp->Render(mSceneManager, it->camera, it->rt);

            if (it->postUpdateCallback)
                it->postUpdateCallback(it->rt);
        }
        
#ifndef DISABLE_POST
        // Post filters
        rp->RenderPost(mSceneManager, mainCam);
#endif

        mFrameNumber++;
        ActiveRenderWindow = nullptr;

        SwapBuffer();
    }

    void DiRenderWindow::Create(uint32 width, uint32 height, const DiString& title, bool fullscreen)
    {
        mWindow = Driver->CreateWnd();
        mWindow->SetParentRenderWnd(this);
        mWindow->Create(width, height, title.c_str(), fullscreen);
        mWndHandle = mWindow->GetWndHandle();

        Init();
    }

    void DiRenderWindow::Create(DiWndHandle hwnd)
    {
        DI_ASSERT(hwnd);
        mWndHandle = hwnd;
        mWindow = Driver->GetWindowManager()->GetWindow(hwnd);

        Init();
    }

    void DiRenderWindow::Init()
    {
        DI_INFO("Intializing render window : %s", mName.c_str());

        Driver->GetWindowSize(mWndHandle, mWidth, mHeight);

        mSceneManager = DI_NEW DiSceneManager(this);
        Driver->CreateWindowTarget(mRenderBuffer, mWndHandle);
        mMainCamera = mSceneManager->GetCamera();

        DiString canvasName = "_canvas_" + mName;
        mCanvasTexture = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(canvasName);
        mCanvasTexture->SetDimensions(mWidth, mHeight);
        mCanvasTexture->SetFormat(PF_A8R8G8B8);
        mCanvasTexture->SetResourceUsage(RU_NONE);
        mCanvasTexture->SetUsage(TU_RENDER_TARGET);
        mCanvasTexture->SetAutoMipmap(false);
        mCanvasTexture->CreateTexture();
        mCanvasTexture->SetAddressing(AM_CLAMP);
        mSceneCanvas = mCanvasTexture->GetRenderTarget();
        mCanvasTexture->SetAdaptedRT(mRenderBuffer);
        mCanvasTexture->SetViewportScale(DiVec2::UNIT_SCALE);
        mSceneCanvas->SetFlippingUV(true);
        
        float clearColor = CommandMgr->GetConsoleVar("clear_color")->GetAsFloat();
        mSceneCanvas->SetClearColor(DiColor(clearColor,clearColor,clearColor,clearColor));

        mPostEffectMgr = DI_NEW DiPostEffectManager(this);
        //mGBuffer = DI_NEW DiGBuffer(this);
    }

    void DiRenderWindow::Shutdown()
    {
        DI_INFO("Closing render window : %s", mName.c_str());

        DiAssetManager::GetInstance().DestroyAsset(mCanvasTexture->GetName());

        SAFE_DELETE(mGBuffer);
        SAFE_DELETE(mPostEffectMgr);
        SAFE_DELETE(mSceneManager);
        SAFE_DELETE(mRenderBuffer);
        SAFE_DELETE(mWindow);
    }

    void DiRenderWindow::OnDeviceLost()
    {
        Driver->GetWindowSize(mWndHandle, mWidth, mHeight);
    }

    void DiRenderWindow::OnDeviceReset()
    {
        if (mSceneManager)
            mSceneManager->GetCamera()->SetAspectRatio(float(GetWidth())/float(GetHeight()));
    }

    uint32 DiRenderWindow::GetWidth()
    {
        return mWidth;
    }

    uint32 DiRenderWindow::GetHeight()
    {
        return mHeight;
    }

    void DiRenderWindow::SwapBuffer()
    {
        DI_PROFILE(RenderWindow_SwapBuffer);
        if (mRenderBuffer && mRenderBuffer->IsActive())
            Driver->SwapBuffer(mRenderBuffer);
    }

    void DiRenderWindow::Closing()
    {
        Driver->DestroyRenderWindow(mWndHandle);
        mWndHandle = nullptr;
    }

    void DiRenderWindow::SetCustomizedCanvasSize(uint32 width, uint32 height)
    {
        DI_DEBUG("Resizing render canvas texture to (%d,%d)",width,height);
        mCanvasTexture->SetAdaptedRT(nullptr);
        mCanvasTexture->ResizeTexture(width,height);

        mSceneManager->GetCamera()->SetAspectRatio(float(width) / float(height));

        GetPostEffectManager()->NotifyInvalidatePasses();
    }
}