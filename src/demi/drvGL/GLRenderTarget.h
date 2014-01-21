
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

        virtual         ~DiGLRenderTarget();

    public:

        void            AttachSurface();

        void            DetachSurface();

        bool            BindRenderTarget(uint8 mrtid);

        bool            BindDepthStencil();

        bool            IsCompatibleWith(DiDepthBuffer* db);

        DiDepthBuffer*  CreateDepthBuffer();

        void            PreBind();

    private:

        GLuint          mRenderbufferID;

        GLenum          mGLFormat;
    };
}