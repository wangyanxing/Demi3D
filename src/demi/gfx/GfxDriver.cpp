
#include "GfxDriver.h"
#include "RenderTarget.h"
#include "DepthBuffer.h"
#include "RenderWindow.h"
#include "RenderPipeline.h"
#include "ShaderManager.h"
#include "Window.h"
#include "Command.h"

namespace Demi
{
    DiGfxDriver::DiGfxDriver()
        :
        mWidth(0), 
        mHeight(0),
        mFullScreen(false),
        mDeviceLost(false),
        mMainWindow(nullptr),
        mCommandMgr(nullptr),
        mDeltaTime(0),
        mElapsedTime(0),
        mFrameNum(0),
        mClosing(false)
    {
        Driver = this;
    }

    DiGfxDriver::~DiGfxDriver()
    {
        Driver = nullptr;
        SAFE_DELETE(mMainWindow);
    }

    void DiGfxDriver::SwapBuffer(DiRenderTarget* renderwnd)
    {
        if (mDeviceLost || IsDeviceLost())
            return;

        if (!renderwnd->SwapBuffer())
            NotifyDeviceLost();
    }

    void DiGfxDriver::NotifyDeviceLost()
    {
        if (mDeviceLost)
            return;

        mDeviceLost = true;

        uint32 w = 0;
        uint32 h = 0;
        GetWindowDimension(mMainHwnd, w, h);

        if (w == 0 || h == 0)
            return;

        OnDeviceLost();

        if (ResetDevice(w, h))
            mDeviceLost = false;

        OnDeviceReset();
    }


    void DiGfxDriver::CleanUpDepthBuffers(bool bCleanManualBuffers)
    {
        auto itMap = mDepthBufferPool.begin();
        auto enMap = mDepthBufferPool.end();

        while (itMap != enMap)
        {
            auto itor = itMap->second.begin();
            auto end  = itMap->second.end();

            while (itor != end)
            {
                if (bCleanManualBuffers || !(*itor)->IsManual())
                {
                    DI_DELETE *itor;
                }
                ++itor;
            }

            itMap->second.clear();
            ++itMap;
        }

        mDepthBufferPool.clear();
    }

    void DiGfxDriver::SetDepthBufferFor(DiRenderTarget *renderTarget)
    {
        uint16 poolId = renderTarget->GetDepthBufferPool();
        if (poolId == DiDepthBuffer::POOL_NO_DEPTH)
            return;

        auto itor = mDepthBufferPool[poolId].begin();
        auto end  = mDepthBufferPool[poolId].end();

        bool bAttached = false;
        while (itor != end && !bAttached)
        {
            bAttached = renderTarget->AttachDepthBuffer(*itor++);
        }

        if (!bAttached)
        {
            DiDepthBuffer *newDepthBuffer = renderTarget->CreateDepthBuffer();
            if (newDepthBuffer)
            {
                newDepthBuffer->SetPoolId(poolId);
                mDepthBufferPool[poolId].push_back(newDepthBuffer);

                bAttached = renderTarget->AttachDepthBuffer(newDepthBuffer);

                DI_ASSERT(bAttached);
            }
            else
            {
                DI_WARNING("Null depth buffer!");
            }
        }
    }

    void DiGfxDriver::AddGpuResource(DiDeviceLostListener* res)
    {
#ifndef NDEBUG
        if (mGpuResources.contains(res))
        {
            DI_WARNING("The gpu resource : %x has already existed", res);
            return;
        }
#endif
        mGpuResources.push_back(res);
    }

    void DiGfxDriver::RemoveGpuResource(DiDeviceLostListener* res)
    {
        auto it = mGpuResources.find(res);
        if (it != mGpuResources.end())
        {
            mGpuResources.erase(it);
            return;
        }
        DI_WARNING("Cannot find the gpu resource");
    }

    void DiGfxDriver::OnDeviceLost()
    {
        DI_DEBUG("*******Losing device*******");

        for (auto it = mGpuResources.begin(); it != mGpuResources.end(); ++it)
            (*it)->OnDeviceLost();

        for (auto it = mRenderWindows.begin(); it != mRenderWindows.end(); ++it)
            it->second->OnDeviceLost();
    }

    void DiGfxDriver::OnDeviceReset()
    {
        DI_DEBUG("*******Resetting device*******");

        for (auto it = mGpuResources.begin(); it != mGpuResources.end(); ++it)
            (*it)->OnDeviceReset();

        for (auto it = mRenderWindows.begin(); it != mRenderWindows.end(); ++it)
            it->second->OnDeviceReset();
    }

    DiRenderWindow* DiGfxDriver::CreateRenderWindow(const DiString& name, DiWndHandle hwnd)
    {
        if (mRenderWindows.contains(hwnd))
        {
            DI_WARNING("The render window [%d] has already existed", (int)hwnd);
            return mRenderWindows[hwnd];
        }

        DiRenderWindow* wnd = DI_NEW DiRenderWindow(name);
        mRenderWindows[hwnd] = wnd;
        wnd->Create(hwnd);
        return wnd;
    }

    void DiGfxDriver::DestroyRenderWindow(DiWndHandle hwnd)
    {
        auto it = mRenderWindows.find(hwnd);
        if (it != mRenderWindows.end())
        {
            if (hwnd == mMainHwnd)
            {
                mMainHwnd = nullptr;
            }

            DI_DELETE it->second;
            mRenderWindows.erase(it);
        }

        if (!mMainHwnd)
        {
            DI_ASSERT(mRenderWindows.empty());
            Shutdown();
        }
    }

    DiRenderWindow* DiGfxDriver::FindRenderWindow(DiWndHandle hwnd)
    {
        auto it = mRenderWindows.find(hwnd);
        if (it != mRenderWindows.end())
            return it->second;

        return nullptr;
    }

    DiRenderWindow* DiGfxDriver::GetMainRenderWindow()
    {
        if (mRenderWindows.empty() || !mMainHwnd)
            return nullptr;
        
        return mRenderWindows[mMainHwnd];
    }

    void DiGfxDriver::DestroyRenderWindows()
    {
        // close other windows
        for (auto it = mRenderWindows.begin(); it != mRenderWindows.end(); ++it)
        {
            if (it->first != mMainHwnd)
                DI_DELETE it->second;
        }
        
        // close main window
        if (mMainHwnd)
            DestroyRenderWindow(mMainHwnd);

        mRenderWindows.clear();
    }

    bool DiGfxDriver::Init(DiWndHandle wnd)
    {
        if (!wnd)
        {
            Shutdown();
            return false;
        }

        mCommandMgr = DI_NEW DiCommandManager();

        mMainHwnd = (HWND)wnd;
        InitGfx(mMainHwnd);

        mPipeline = DI_NEW DiRenderPipeline();
        mShaderManager = DI_NEW DiShaderManager();

        DiRenderWindow* rw = CreateRenderWindow("main_wnd", wnd);

        if (mMainWindow)
            mMainWindow->SetParentRenderWnd(rw);

        NotifyDeviceLost();

        return true;
    }

    bool DiGfxDriver::Init(uint32 w, uint32 h, const char* title, bool fullscreen)
    {
        mMainWindow = DiWindow::CreateWnd();
        mMainWindow->Create(w, h, title, fullscreen);
        return Init(mMainWindow->GetWndHandle());
    }

    void DiGfxDriver::Shutdown(bool destroyWindows)
    {
        if (destroyWindows && !mRenderWindows.empty())
            DestroyRenderWindows();

        SAFE_DELETE(mShaderManager);
        SAFE_DELETE(mPipeline);

        ReleaseGfx();

        SAFE_DELETE(mCommandMgr);
    }

    void DiGfxDriver::Render(DiRenderWindow* render)
    {
        DI_PROFILE(Graphics_Update);

        mDeltaTime = (float)mTimer.GetElapse();
        mElapsedTime += mDeltaTime;
        mTimer.Start();
        mFrameNum++;

        if (!render)
            render = GetMainRenderWindow();

        if (!render)
            return;

        if (mMainWindow)
            mMainWindow->Update();

        if (mClosing)
            return;

        DiRenderWindow::ActiveWindow = render;
        render->Render();
        DiRenderWindow::ActiveWindow = nullptr;
    }

    DiSceneManager* DiGfxDriver::GetSceneManager()
    {
        if (mRenderWindows.empty())
            return nullptr;
        
        return mRenderWindows[mMainHwnd]->GetSceneManager();
    }

    DiShaderEnvironment* DiGfxDriver::GetShaderEnvironment() const
    {
        return mPipeline->GetShaderEnvironment();
    }

}