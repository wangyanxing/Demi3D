
/********************************************************************
    File:       GLRenderTarget.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "RenderTarget.h"

#define GL_DEPTH24_STENCIL8_EXT 0x88F0

namespace Demi
{
    class DI_GLDRV_API DiGLRenderTarget : public DiRenderTarget
    {
    public:

        DiGLRenderTarget();

        virtual             ~DiGLRenderTarget();

    public:

        void                AttachSurface();

        void                DetachSurface();

        bool                BindRenderTarget(uint8 mrtid);

        bool                AttachDepthBuffer(DiDepthBuffer *depthBuffer);

        bool                BindDepthStencil();

        bool                IsCompatibleWith(DiDepthBuffer* db);

        DiDepthBuffer*      CreateDepthBuffer();

        void                PreBind();

    private:

        GLenum              mGLFormat;

        DiGLFrameBuffer*    mFrameBuffer;

        DiGLRenderBuffer*   mDepthBuffer;

        DiGLRenderBuffer*   mStencilBuffer;
    };


    class DI_GLDRV_API DiGLWindowTarget : public DiGLRenderTarget
    {
    public:

        DiGLWindowTarget();

        ~DiGLWindowTarget();

    public:

        void                Create(DiWndHandle wnd);
    };
}