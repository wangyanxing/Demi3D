
/********************************************************************
    File:       GLDepthBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once
#include "DepthBuffer.h"

namespace Demi
{
    class DI_GLDRV_API DiGLDepthBuffer : public DiDepthBuffer
    {
    public:

        DiGLDepthBuffer(uint16 poolId, uint32 width, uint32 height, DiGLRenderBuffer* depth,
            DiGLRenderBuffer* stencil, uint32 fsaa, uint32 multiSampleQuality, bool manual);

        ~DiGLDepthBuffer();

    public:

        DiGLRenderBuffer*   GetDepthBuffer() { return mDepthBuffer; }

        DiGLRenderBuffer*   GetStencilBuffer() { return mStencilBuffer; }

    private:

        DiGLRenderBuffer*   mDepthBuffer;

        DiGLRenderBuffer*   mStencilBuffer;
    };
}