
#include "GLRenderTarget.h"
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "GLFrameBuffer.h"
#include "GLRenderBuffer.h"

#include "Texture.h"

namespace Demi
{
    DiGLRenderTarget::DiGLRenderTarget()
        :mGLFormat(0),
        mFrameBuffer(nullptr),
        mDepthBuffer(nullptr),
        mStencilBuffer(nullptr)
    {
        mFrameBuffer = DI_NEW DiGLFrameBuffer();
    }

    DiGLRenderTarget::~DiGLRenderTarget()
    {
        DI_DELETE mFrameBuffer;
        mFrameBuffer = nullptr;
    }

    bool DiGLRenderTarget::BindRenderTarget(uint8 mrtid)
    {
        mFrameBuffer->Bind();
        return true;
    }

    bool DiGLRenderTarget::BindDepthStencil()
    {
        return true;
    }

    void DiGLRenderTarget::AttachSurface()
    {
        mGLFormat = DiGLTypeMappings::GLFormatMapping[mParentTex->GetFormat()];

        mFrameBuffer->AttachSurface(0, mParentTex);
    }

    void DiGLRenderTarget::DetachSurface()
    {
        mFrameBuffer->DetarchSurface(0);
    }

    DiDepthBuffer* DiGLRenderTarget::CreateDepthBuffer()
    {
        DiDepthBuffer *ret = nullptr;

        if (!mFrameBuffer) return ret;

        GLenum glfmt = DiGLTypeMappings::GLFormatMapping[mFrameBuffer->GetFormat()];
        GLuint depthFormat, stencilFormat;
        DiGLDriver::FBOManager->GetBestDepthStencil(glfmt, &depthFormat, &stencilFormat);

        DiGLRenderBuffer* db = DI_NEW DiGLRenderBuffer(depthFormat, GetWidth(), GetHeight());
        DiGLRenderBuffer* sb = db;

        if (depthFormat != GL_DEPTH24_STENCIL8_EXT && sb)
        {
            sb = DI_NEW DiGLRenderBuffer(stencilFormat, GetWidth(), GetHeight());
        }

        ret = DI_NEW DiGLDepthBuffer(0, GetWidth(), GetHeight(), db, sb, 0, 0, false);

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
        bool retVal = false;

        if (mWidth != db->GetWidth() ||
            mHeight != db->GetHeight() )
            return retVal;

        if (mDepthBuffer || mStencilBuffer)
        {
            GLenum internalFormat = DiGLTypeMappings::GLFormatMapping[mFrameBuffer->GetFormat()];
            GLenum depthFormat, stencilFormat;

            static_cast<DiGLDriver*>(Driver)->GetDepthStencilFormatFor(internalFormat, &depthFormat, &stencilFormat);

            bool bSameDepth = false;

            if (mDepthBuffer)
                bSameDepth |= mDepthBuffer->GetGLFormat() == depthFormat;

            bool bSameStencil = false;

            if (!mStencilBuffer || mStencilBuffer == mDepthBuffer)
                bSameStencil = stencilFormat == GL_NONE;
            else
            {
                if (mStencilBuffer)
                    bSameStencil = stencilFormat == mStencilBuffer->GetGLFormat();
            }

            retVal = bSameDepth && bSameStencil;
        }

        return retVal;
    }

    bool DiGLRenderTarget::AttachDepthBuffer(DiDepthBuffer *depthBuffer)
    {
        bool result;
        if ((result = DiRenderTarget::AttachDepthBuffer(depthBuffer)))
        {
            mFrameBuffer->AttachDepthBuffer(depthBuffer);
        }

        return result;
    }


    DiGLWindowTarget::DiGLWindowTarget()
    {
       
    }

    DiGLWindowTarget::~DiGLWindowTarget()
    {

    }

    void DiGLWindowTarget::Create(DiWndHandle wnd)
    {
        Driver->GetWindowDimension(wnd, mWidth, mHeight);

        DiGLDepthBuffer *depthBuffer = DI_NEW DiGLDepthBuffer(DiDepthBuffer::POOL_DEFAULT, 
            mWidth, mHeight,
            nullptr, nullptr,
            0, 0, true);

        AttachDepthBuffer(depthBuffer);
    }

}