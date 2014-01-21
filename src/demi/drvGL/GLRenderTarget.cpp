
#include "GLRenderTarget.h"
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "Texture.h"

namespace Demi
{
    DiGLRenderTarget::DiGLRenderTarget()
        :mRenderbufferID(0),
        mGLFormat(0)
    {
    }

    DiGLRenderTarget::~DiGLRenderTarget()
    {
    }

    bool DiGLRenderTarget::BindRenderTarget(uint8 mrtid)
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mRenderbufferID);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, mrtid,
            GL_RENDERBUFFER_EXT, mRenderbufferID);
        return true;
    }

    bool DiGLRenderTarget::BindDepthStencil()
    {
        return true;
    }

    void DiGLRenderTarget::AttachSurface()
    {
        mGLFormat = DiGLTypeMappings::GLFormatMapping[mParentTex->GetFormat()];

        glGenRenderbuffersEXT(1, &mRenderbufferID);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderbufferID);

        // no MSAA now
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, mGLFormat, mWidth, mHeight);
    }

    void DiGLRenderTarget::DetachSurface()
    {
        glDeleteRenderbuffersEXT(1, &mRenderbufferID);
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