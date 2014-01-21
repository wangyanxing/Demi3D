
#include "GLRenderTarget.h"
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "Texture.h"

namespace Demi
{
    DiGLRenderTarget::DiGLRenderTarget()
    {
    }

    DiGLRenderTarget::~DiGLRenderTarget()
    {
    }

    bool DiGLRenderTarget::BindRenderTarget(uint8 mrtid)
    {
        return true;
    }

    bool DiGLRenderTarget::BindDepthStencil()
    {
        return true;
    }

    void DiGLRenderTarget::AttachSurface()
    {
    }

    void DiGLRenderTarget::DetachSurface()
    {
    }

    DiDepthBuffer* DiGLRenderTarget::CreateDepthBuffer()
    {
        return nullptr;
    }

    void DiGLRenderTarget::PreBind()
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

    bool DiGLRenderTarget::IsCompatibleWith(DiDepthBuffer* db)
    {
        return false;
    }
}