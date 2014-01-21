
#include "D3D9DepthBuffer.h"
#include "D3D9Texture.h"
#include "D3D9TypeMappings.h"

namespace Demi
{
    DiD3D9DepthBuffer::DiD3D9DepthBuffer(uint16 poolId, uint32 width, uint32 height,
        void* depthBufferSurf, DiPixelFormat fmt,
        uint32 fsaa, uint32 multiSampleQuality, bool manual)
    {
        mPoolId = poolId;
        mWidth = width;
        mHeight = height;
        mManual = manual;
        mMultiSampleQuality = multiSampleQuality;
        mD3DFormat = DiD3D9Mappings::D3D9FormatMapping[fmt];
        mDepthBuffer = (IDirect3DSurface9*)depthBufferSurf;

        switch (mD3DFormat)
        {
        case D3DFMT_D16_LOCKABLE:
        case D3DFMT_D15S1:
        case D3DFMT_D16:
            mBitDepth = 16;
            break;
        case D3DFMT_D32:
        case D3DFMT_D24S8:
        case D3DFMT_D24X8:
        case D3DFMT_D24X4S4:
        case D3DFMT_D32F_LOCKABLE:
        case D3DFMT_D24FS8:
            mBitDepth = 32;
            break;
        }
    }

    DiD3D9DepthBuffer::~DiD3D9DepthBuffer()
    {
        Release();
    }

    void DiD3D9DepthBuffer::Release()
    {
        DI_DEBUG("D3D9 depth/stencil buffer released: %x", (void*)mDepthBuffer);
        SAFE_RELEASE(mDepthBuffer);
    }
}