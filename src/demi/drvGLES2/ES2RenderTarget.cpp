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

#include "DrvGLES2Pch.h"
#include "ES2RenderTarget.h"
#include "ES2DepthBuffer.h"
#include "GLES2Driver.h"
#include "ES2TypeMappings.h"
#include "ES2FrameBuffer.h"
#include "ES2RenderBuffer.h"
#include "ES2Context.h"

#include "Texture.h"
#include "Window.h"

namespace Demi
{
    DiGLES2RenderTarget::DiGLES2RenderTarget()
        :mGLFormat(0),
        mFrameBuffer(nullptr)
    {
    }

    DiGLES2RenderTarget::~DiGLES2RenderTarget()
    {
        if (mFrameBuffer)
        {
            DI_DELETE mFrameBuffer;
            mFrameBuffer = nullptr;
        }
    }

    bool DiGLES2RenderTarget::BindRenderTarget(uint8 mrtid)
    {
        if (mFrameBuffer)
            mFrameBuffer->Bind();
        else
        {
            // Old style context (window/pbuffer) or copying render texture
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
            // The screen buffer is 1 on iOS
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 1));
#else
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#endif
        }
        return true;
    }

    bool DiGLES2RenderTarget::BindDepthStencil()
    {
        DiGLES2DepthBuffer *depthBuffer = static_cast<DiGLES2DepthBuffer*>(GetDepthBuffer());

        if (GetDepthBufferPool() != DiDepthBuffer::POOL_NO_DEPTH &&
            (!depthBuffer || depthBuffer->GetGLContext() != static_cast<DiGLES2Driver*>(Driver)->GetCurrentContext()))
        {
            //Depth is automatically managed and there is no depth buffer attached to this RT
            //or the Current context doesn't match the one this Depth buffer was created with
            Driver->SetDepthBufferFor(this);
        }
        return true;
    }

    void DiGLES2RenderTarget::AttachSurface()
    {
        mGLFormat = DiGLTypeMappings::GetClosestGLInternalFormat(mParentTex->GetFormat());

        mFrameBuffer->AttachSurface(0, mParentTex);
    }

    void DiGLES2RenderTarget::DetachSurface()
    {
        mFrameBuffer->DetarchSurface(0);
    }

    DiDepthBuffer* DiGLES2RenderTarget::CreateDepthBuffer()
    {
        DiDepthBuffer *ret = nullptr;

        if (!mFrameBuffer) 
            return ret;

        GLuint depthFormat, stencilFormat;
        DiGLES2Driver::FBOManager->GetBestDepthStencil(mFrameBuffer->GetFormat(), &depthFormat, &stencilFormat);

        DiGLES2RenderBuffer* db = DI_NEW DiGLES2RenderBuffer(depthFormat, GetWidth(), GetHeight());
        DiGLES2RenderBuffer* sb = db;

        if (depthFormat != GL_DEPTH24_STENCIL8_OES && sb)
        {
            sb = DI_NEW DiGLES2RenderBuffer(stencilFormat, GetWidth(), GetHeight());
        }

        auto context = static_cast<DiGLES2Driver*>(Driver)->GetCurrentContext();
        return DI_NEW DiGLES2DepthBuffer(0, GetWidth(), GetHeight(), context, db, sb, 0, 0, false);
    }

    void DiGLES2RenderTarget::PreBind()
    {
        uint32 left   = uint32(mViewport.mLeft   * mWidth);
        uint32 top    = uint32(mViewport.mTop    * mHeight);
        uint32 width  = uint32(mViewport.mWidth  * mWidth);
        uint32 height = uint32(mViewport.mHeight * mHeight);

        if (!mFlippingUV)
        {
            // Convert "upper-left" corner to "lower-left"
            //top = GetHeight() - height - top;
        }

        Driver->SetViewport(left, top, width, height);
    }

    bool DiGLES2RenderTarget::IsCompatibleWith(DiDepthBuffer* db)
    {
        bool retVal = false;

        //Check standard stuff first.
        if (Driver->GetGfxCaps()->hasCapability(RSC_RTT_DEPTHBUFFER_RESOLUTION_LESSEQUAL))
        {
            if (!DiRenderTarget::IsCompatibleWith(db))
                return false;
        }
        else
        {
            if (mWidth != db->GetWidth() || mHeight != db->GetHeight())
                return retVal;
        }


        DiGLES2DepthBuffer* gldb = static_cast<DiGLES2DepthBuffer*>(db);
        DiGLES2RenderBuffer* glDepth = gldb->GetDepthBuffer();
        DiGLES2RenderBuffer* glStencil = gldb->GetStencilBuffer();

        if (!mFrameBuffer)
        {
            DiGLES2Context* dbContext = gldb->GetGLContext();
            if (!glDepth && !glStencil && dbContext == GetContext())
            {
                retVal = true;
            }
        }
        else
        {
            if (glDepth || glStencil)
            {
                GLenum depthFormat, stencilFormat;
                static_cast<DiGLES2Driver*>(Driver)->GetDepthStencilFormatFor(mFrameBuffer->GetFormat(),
                                                                           &depthFormat, &stencilFormat);

                bool bSameDepth = false;

                if (glDepth)
                    bSameDepth |= glDepth->GetGLFormat() == depthFormat;

                bool bSameStencil = false;

                if (!glStencil || glStencil == glDepth)
                    bSameStencil = stencilFormat == GL_NONE;
                else
                {
                    if (glStencil)
                        bSameStencil = stencilFormat == glStencil->GetGLFormat();
                }

                retVal = bSameDepth && bSameStencil;
            }
        }
        
        return retVal;
    }

    bool DiGLES2RenderTarget::AttachDepthBuffer(DiDepthBuffer *depthBuffer)
    {
        bool result;
        if ((result = DiRenderTarget::AttachDepthBuffer(depthBuffer)))
        {
            if (mFrameBuffer)
                mFrameBuffer->AttachDepthBuffer(depthBuffer);
        }

        return result;
    }

    void DiGLES2RenderTarget::Init()
    {
        mFrameBuffer = DI_NEW DiGLES2FrameBuffer();
    }

    void DiGLES2RenderTarget::DetachDepthBuffer()
    {
        if (mFrameBuffer)
            mFrameBuffer->DetachDepthBuffer();
        DiRenderTarget::DetachDepthBuffer();
    }

    DiGLWindowTarget::DiGLWindowTarget()
        : mWnd(nullptr)
        , mContext(nullptr)
    {
    }

    DiGLWindowTarget::~DiGLWindowTarget()
    {
    }

    void DiGLWindowTarget::Create(DiWndHandle wnd, DiGLES2Context* context)
    {
        mWnd = wnd;
        mContext = context;

        Driver->GetWindowSize(wnd, mWidth, mHeight);

        DiGLES2DepthBuffer *depthBuffer = DI_NEW DiGLES2DepthBuffer(DiDepthBuffer::POOL_DEFAULT, 
            mWidth, mHeight,
            static_cast<DiGLES2Driver*>(Driver)->GetCurrentContext(), nullptr, nullptr,
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