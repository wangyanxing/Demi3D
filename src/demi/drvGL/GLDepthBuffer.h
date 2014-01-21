
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

        DiGLDepthBuffer();

        ~DiGLDepthBuffer();

    public:

        void        Init(uint16 poolId, uint32 width, uint32 height,
                        void* depthBufferSurf, DiPixelFormat fmt,
                        uint32 fsaa, uint32 multiSampleQuality, bool manual);


    };
}