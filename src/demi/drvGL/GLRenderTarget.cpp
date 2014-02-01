
#include "DrvGLPch.h"
#include "GLRenderTarget.h"
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "GLFrameBuffer.h"
#include "GLRenderBuffer.h"
#include "GLContext.h"

#include "Texture.h"
#include "Window.h"

namespace Demi
{
    DiGLRenderTarget::DiGLRenderTarget()
        :mGLFormat(0),
        mFrameBuffer(nullptr),
        mDepthBuffer(nullptr),
        mStencilBuffer(nullptr)
    {
    }

    DiGLRenderTarget::~DiGLRenderTarget()
    {
        if (mFrameBuffer)
        {
            DI_DELETE mFrameBuffer;
            mFrameBuffer = nullptr;
        }
    }

    bool DiGLRenderTarget::BindRenderTarget(uint8 mrtid)
    {
        if (mFrameBuffer)
            mFrameBuffer->Bind();
        return true;
    }

    bool DiGLRenderTarget::BindDepthStencil()
    {
        DiGLDepthBuffer *depthBuffer = static_cast<DiGLDepthBuffer*>(GetDepthBuffer());

        if (GetDepthBufferPool() != DiDepthBuffer::POOL_NO_DEPTH &&
            (!depthBuffer || depthBuffer->GetGLContext() != static_cast<DiGLDriver*>(Driver)->GetCurrentContext()))
        {
            //Depth is automatically managed and there is no depth buffer attached to this RT
            //or the Current context doesn't match the one this Depth buffer was created with
            Driver->SetDepthBufferFor(this);
        }
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

        ret = DI_NEW DiGLDepthBuffer(0, GetWidth(), GetHeight(),
            static_cast<DiGLDriver*>(Driver)->GetCurrentContext(), db, sb, 0, 0, false);

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

        if (mWidth != db->GetWidth() || mHeight != db->GetHeight() )
            return retVal;

        if (!mFrameBuffer)
        {
            DiGLContext* dbContext = static_cast<DiGLDepthBuffer*>(db)->GetGLContext();
            if (!mDepthBuffer && !mStencilBuffer && dbContext == GetContext())
            {
                retVal = true;
            }
        }
        else
        {
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
        }
        
        return retVal;
    }

    bool DiGLRenderTarget::AttachDepthBuffer(DiDepthBuffer *depthBuffer)
    {
        bool result;
        if ((result = DiRenderTarget::AttachDepthBuffer(depthBuffer)))
        {
            if (mFrameBuffer)
                mFrameBuffer->AttachDepthBuffer(depthBuffer);
        }

        return result;
    }

    void DiGLRenderTarget::Init()
    {
        mFrameBuffer = DI_NEW DiGLFrameBuffer();
    }

    DiGLWindowTarget::DiGLWindowTarget()
        : mWnd(nullptr)
        , mContext(nullptr)
    {
    }

    DiGLWindowTarget::~DiGLWindowTarget()
    {
    }

    void DiGLWindowTarget::Create(DiWndHandle wnd, DiGLContext* context)
    {
        mWnd = wnd;
        mContext = context;

        Driver->GetWindowSize(wnd, mWidth, mHeight);

        DiGLDepthBuffer *depthBuffer = DI_NEW DiGLDepthBuffer(DiDepthBuffer::POOL_DEFAULT, 
            mWidth, mHeight,
            static_cast<DiGLDriver*>(Driver)->GetCurrentContext(), nullptr, nullptr,
            0, 0, true);

        AttachDepthBuffer(depthBuffer);
    }

    bool DiGLWindowTarget::SwapBuffer()
    {
        DiWindow* wnd = Driver->GetWindowManager()->GetWindow(mWnd);
        if(wnd)
        {
            wnd->SwapBuffers();
            return true;
        }
        else
            return false;
    }

    void DiGLWindowTarget::Init()
    {
    }
}