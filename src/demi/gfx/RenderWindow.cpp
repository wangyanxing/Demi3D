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

#include "Image.h"
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

        DI_PROFILE(RenderWindow_Render);

        if (mWindow)
            mWindow->Update();

        mCurrentPrimitivesNum = 0;
        mCurrentBatchesNum = 0;

        DiRenderPipeline* rp = Driver->GetPipeline();

        mSceneManager->PreUpdate();

        Driver->BeginFrame();

        // Normal geometry pass
        DiCamera* mainCam = mSceneManager->GetCamera();
        mSceneManager->SetCurrentPass(GEOMETRY_PASS);
        mSceneManager->Cull(mainCam);
        mSceneManager->GetVisibleObjects().UpdateAll(mainCam);

        rp->ClearGroup();
        mSceneManager->GetVisibleObjects().AddToBatch(rp);
        rp->Render(mSceneManager, mainCam, mSceneCanvas);
        
        static int i = 0;
        i++;

        // Process the extra render targets
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

            if (i == -1000){
                auto tex = it->rt->GetParentTexture();
                DiString file = tex->GetName() + ".png";
                DiImage::SaveTextureAsPng(tex, file);
            }

            if (it->postUpdateCallback)
                it->postUpdateCallback(it->rt);
        }
        
        rp->RenderPost(mSceneManager, mainCam);

        Driver->EndFrame();
        
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
        mCanvasTexture->SetFormat(PF_X8R8G8B8);
        mCanvasTexture->SetResourceUsage(RU_NONE);
        mCanvasTexture->SetUsage(TU_RENDER_TARGET);
        mCanvasTexture->CreateTexture();
        mSceneCanvas = mCanvasTexture->GetRenderTarget();
        mCanvasTexture->SetAdaptedRT(mRenderBuffer);
        mCanvasTexture->SetViewportScale(DiVec2::UNIT_SCALE);

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
}