
#include "DrvD3D9Pch.h"
#include "D3D9WindowTarget.h"
#include "RenderWindow.h"
#include "D3D9Driver.h"

namespace Demi
{
    DiD3D9WindowTarget::DiD3D9WindowTarget(void) :
        mWndHandle(nullptr)
        , mSwapChain(nullptr)
        , mD3D9DepthBuffer(nullptr)
    {
        SetShouldUpdate(false);
    }

    DiD3D9WindowTarget::~DiD3D9WindowTarget(void)
    {
        ReleaseResources();
    }

    void DiD3D9WindowTarget::Create(HWND hwnd)
    {
        mWndHandle = hwnd;

        RECT kRect;
        GetClientRect(hwnd, &kRect);
        mWidth  = kRect.right - kRect.left;
        mHeight = kRect.bottom - kRect.top;

        DiD3D9Driver* d3d9Drv = static_cast<DiD3D9Driver*>(Driver);

        if (IsSwapChainWindow())
            mSwapChain = d3d9Drv->CreateSwapChain(hwnd);
        else
            mSwapChain = d3d9Drv->GetSwapChain(0);

        HRESULT h = mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mSurface);
        DX9_CHKERR(h);

        mSwapChain->GetPresentParameters(&mParameters);

        h = DiD3D9Driver::Device->CreateDepthStencilSurface(
            mWidth, mHeight,
            mParameters.AutoDepthStencilFormat,
            mParameters.MultiSampleType,
            mParameters.MultiSampleQuality,
            (mParameters.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL),
            &mD3D9DepthBuffer, NULL
            );
        DX9_CHKERR(h);
        DI_DEBUG("D3D9 Depth stencil buffer created: (%d,%d), ptr:%x", mWidth, mHeight, mD3D9DepthBuffer);

        if (mD3D9DepthBuffer)
        {
            DiDepthBuffer *depthBuf = d3d9Drv->AddManualDepthBuffer(mD3D9DepthBuffer);
            AttachDepthBuffer(depthBuf);
        }
    }

    void DiD3D9WindowTarget::OnDeviceLost()
    {
        SAFE_RELEASE(mSurface);
        SAFE_RELEASE(mSwapChain);
    }

    void DiD3D9WindowTarget::OnDeviceReset()
    {
        Driver->GetWindowSize(mWndHandle, mWidth, mHeight);

        DiD3D9Driver* d3d9Drv = static_cast<DiD3D9Driver*>(Driver);

        if (IsSwapChainWindow())
            mSwapChain = d3d9Drv->CreateSwapChain(mWndHandle);
        else
            mSwapChain = d3d9Drv->GetSwapChain(0);

        HRESULT h = mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mSurface);
        DX9_CHKERR(h);

        mSwapChain->GetPresentParameters(&mParameters);
    }

    bool DiD3D9WindowTarget::IsSwapChainWindow()
    {
        if (!Driver->GetMainRenderWindow())
            return false;

        return Driver->GetMainRenderWindow()->GetRenderBuffer() != this;
    }

    HRESULT DiD3D9WindowTarget::Present()
    {
        if (!IsSwapChainWindow())
            return DiD3D9Driver::Device->Present(NULL, NULL, NULL, NULL);
        else
            return mSwapChain->Present(NULL, NULL, NULL, NULL, 0);
    }

    void DiD3D9WindowTarget::ReleaseResources()
    {
        SAFE_RELEASE(mSwapChain);
    }

    bool DiD3D9WindowTarget::CheckSizeChanged()
    {
        RECT kRect;
        GetClientRect(GetWindowHandle(), &kRect);

        int wd = kRect.right - kRect.left;
        int he = kRect.bottom - kRect.top;

        if (wd != (int)mWidth || he != (int)mHeight)
            return true;

        return false;
    }

    uint32 DiD3D9WindowTarget::GetWidth() const
    {
        RECT kRect;
        GetClientRect(GetWindowHandle(), &kRect);
        return kRect.right - kRect.left;
    }

    uint32 DiD3D9WindowTarget::GetHeight() const
    {
        RECT kRect;
        GetClientRect(GetWindowHandle(), &kRect);
        return kRect.bottom - kRect.top;
    }

    bool DiD3D9WindowTarget::SwapBuffer()
    {
        bool resized = CheckSizeChanged();
        HRESULT hr = Present();

        if (D3DERR_DEVICELOST == hr || resized)
            return false;
        else
        {
            if (FAILED(hr))
            {
                DI_ERROR("Swaping buffer failed!");
            }
            return true;
        }
    }
}