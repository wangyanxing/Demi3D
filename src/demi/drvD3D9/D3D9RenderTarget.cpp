
#include "D3D9RenderTarget.h"
#include "D3D9DepthBuffer.h"
#include "D3D9Driver.h"
#include "D3D9TypeMappings.h"
#include "Texture.h"

namespace Demi
{
    DiD3D9RenderTarget::DiD3D9RenderTarget()
        :mSurface(nullptr)
    {
    }

    DiD3D9RenderTarget::~DiD3D9RenderTarget()
    {
    }

    bool DiD3D9RenderTarget::BindRenderTarget(uint8 mrtid)
    {
        if (!mSurface)
            return false;
        HRESULT hr = DiD3D9Driver::Device->SetRenderTarget(mrtid, mSurface);
        DX9_CHKERR(hr);
        return true;
    }

    bool DiD3D9RenderTarget::BindDepthStencil()
    {
        DiDepthBuffer* depthBuffer = GetDepthBuffer();
        if (GetDepthBufferPool() != DiDepthBuffer::POOL_NO_DEPTH && (!depthBuffer))
        {
            Driver->SetDepthBufferFor(this);
            depthBuffer = GetDepthBuffer();
        }

        IDirect3DSurface9 *depthSurface = depthBuffer ? static_cast<DiD3D9DepthBuffer*>(depthBuffer)->GetDepthBufferSurface() : nullptr;
        HRESULT hRlt = DiD3D9Driver::Device->SetDepthStencilSurface(depthSurface);
        DX9_CHKERR(hRlt);
        return true;
    }

    void DiD3D9RenderTarget::AttachSurface()
    {
        mSurface = static_cast<IDirect3DSurface9*>(mParentTex->GetSurfaceHandle());
    }

    void DiD3D9RenderTarget::DetachSurface()
    {
        SAFE_RELEASE(mSurface);
    }

    DiDepthBuffer* DiD3D9RenderTarget::CreateDepthBuffer()
    {
        if (!mSurface)
            return nullptr;

        D3DSURFACE_DESC srfDesc;
        if (FAILED(mSurface->GetDesc(&srfDesc)))
        {
            DI_WARNING("Failed to get the DESC of the RT surface");
            return nullptr;
        }

        D3DFORMAT dsfmt = static_cast<DiD3D9Driver*>(Driver)->GetDepthStencilFormatFor(srfDesc.Format);

        IDirect3DSurface9 *depthBufferSurface = NULL;
        HRESULT hr = DiD3D9Driver::Device->CreateDepthStencilSurface(
            srfDesc.Width, srfDesc.Height, dsfmt,
            srfDesc.MultiSampleType, srfDesc.MultiSampleQuality,
            TRUE, &depthBufferSurface, NULL);
        DI_DEBUG("D3D9 Depth stencil buffer created: (%d,%d), ptr:%x", mWidth, mHeight, depthBufferSurface);

        DX9_CHKERR(hr);

        DiDepthBuffer* newDepthBuffer = DI_NEW DiD3D9DepthBuffer(
            DiDepthBuffer::POOL_DEFAULT, srfDesc.Width, srfDesc.Height,
            depthBufferSurface, dsfmt, 
            srfDesc.MultiSampleType, srfDesc.MultiSampleQuality, false);

        return newDepthBuffer;
    }

    void DiD3D9RenderTarget::PreBind()
    {
        if (mViewportDirty)
        {
            uint32 left   = uint32(mViewport.mLeft   * mWidth);
            uint32 top    = uint32(mViewport.mTop    * mHeight);
            uint32 width  = uint32(mViewport.mWidth  * mWidth);
            uint32 height = uint32(mViewport.mHeight * mHeight);

            DI_DEBUG("Viewport updated : %d,%d,%d,%d", left, top, width, height);
            Driver->SetViewport(left, top, width, height);

            mViewportDirty = false;
        }
    }

    bool DiD3D9RenderTarget::IsCompatibleWith(DiDepthBuffer* db)
    {
        if (!mSurface)
            return false;

        D3DSURFACE_DESC srfDesc;
        if (SUCCEEDED(mSurface->GetDesc(&srfDesc)))
        {
            D3DFORMAT fmt = static_cast<DiD3D9Driver*>(Driver)->GetDepthStencilFormatFor(srfDesc.Format);

            if (fmt == static_cast<DiD3D9DepthBuffer*>(db)->GetFormat() &&
                db->GetMSAAQuality() == srfDesc.MultiSampleQuality &&
                db->GetWidth() >= GetWidth() &&
                db->GetHeight() >= GetHeight())
            {
                return true;
            }
        }

        return false;
    }
}