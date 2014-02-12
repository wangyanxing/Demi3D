


#ifndef DiD3D9DepthBuffer_h__
#define DiD3D9DepthBuffer_h__


#include "DepthBuffer.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9DepthBuffer : public DiDepthBuffer
    {
    public:

        DiD3D9DepthBuffer(uint16 poolId, uint32 width, uint32 height,
            void* depthBufferSurf, D3DFORMAT fmt,
            uint32 fsaa, uint32 multiSampleQuality, bool manual);

        ~DiD3D9DepthBuffer();

    public:

        IDirect3DSurface9*  GetDepthBufferSurface() const
        {
            return mDepthBuffer;
        }

        D3DFORMAT           GetFormat()
        {
            return mD3DFormat;
        }

        void                Release();

    private:

        IDirect3DSurface9*  mDepthBuffer;

        D3DFORMAT           mD3DFormat;
    };
}

#endif
