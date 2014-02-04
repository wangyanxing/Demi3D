
#include "DrvD3D9Pch.h"
#include"D3D9TypeMappings.h"

namespace Demi
{
    D3DPRIMITIVETYPE DiD3D9Mappings::D3D9PrimTypeMapping[PT_MAX] =
    {
        D3DPT_FORCE_DWORD,
        D3DPT_POINTLIST,
        D3DPT_LINELIST,
        D3DPT_LINESTRIP,
        D3DPT_TRIANGLELIST,
        D3DPT_TRIANGLESTRIP,
        D3DPT_TRIANGLEFAN
    };

    D3DTEXTUREFILTERTYPE DiD3D9Mappings::D3D9MinMagFilter[] =
    {
        D3DTEXF_POINT,
        D3DTEXF_LINEAR,
        D3DTEXF_LINEAR,
        D3DTEXF_ANISOTROPIC
    };

    D3DTEXTUREFILTERTYPE DiD3D9Mappings::D3D9MipFilter[] =
    {
        D3DTEXF_POINT,
        D3DTEXF_POINT,
        D3DTEXF_LINEAR,
        D3DTEXF_ANISOTROPIC
    };

    D3DTEXTUREADDRESS DiD3D9Mappings::D3D9AddModeMapping[AM_MAX] =
    {
        D3DTADDRESS_WRAP,
        D3DTADDRESS_MIRROR,
        D3DTADDRESS_CLAMP,
        D3DTADDRESS_BORDER,
        D3DTADDRESS_MIRRORONCE
    };

    D3DFORMAT DiD3D9Mappings::D3D9FormatMapping[PIXEL_FORMAT_MAX] =
    {
        D3DFMT_R8G8B8,
        D3DFMT_A8R8G8B8,
        D3DFMT_A8B8G8R8,
        D3DFMT_X8R8G8B8,
        D3DFMT_A8,
        D3DFMT_L8,
        D3DFMT_DXT1,
        D3DFMT_DXT2,
        D3DFMT_DXT3,
        D3DFMT_DXT4,
        D3DFMT_DXT5,
        D3DFMT_R16F,
        D3DFMT_G16R16F,
        D3DFMT_A16B16G16R16F,
        D3DFMT_R32F,
        D3DFMT_G32R32F,
        D3DFMT_A32B32G32R32F,
    };

    DiPixelFormat DiD3D9Mappings::ConvertPixFormat(D3DFORMAT d3dfmt)
    {
        static DiMap<D3DFORMAT, DiPixelFormat> mapping;
        if (mapping.empty())
        {
            mapping[D3DFMT_R8G8B8]   =    PF_R8G8B8;
            mapping[D3DFMT_A8R8G8B8] =    PF_A8R8G8B8;
            mapping[D3DFMT_A8B8G8R8] =    PF_A8B8G8R8;
            mapping[D3DFMT_X8R8G8B8] =    PF_X8R8G8B8;
            mapping[D3DFMT_A8]       =    PF_A8;
            mapping[D3DFMT_L8]       =    PF_L8;
            mapping[D3DFMT_DXT1]     =    PF_DXT1;
            mapping[D3DFMT_DXT2]     =    PF_DXT2;
            mapping[D3DFMT_DXT3]     =    PF_DXT3;
            mapping[D3DFMT_DXT4]     =    PF_DXT4;
            mapping[D3DFMT_DXT5]     =    PF_DXT5;
            mapping[D3DFMT_R16F]     =    PF_R16F;
            mapping[D3DFMT_G16R16F]  =    PF_G16R16F;
            mapping[D3DFMT_A16B16G16R16F] = PF_A16B16G16R16F;
            mapping[D3DFMT_R32F]     =    PF_R32F;
            mapping[D3DFMT_G32R32F]  =    PF_G32R32F;
            mapping[D3DFMT_A32B32G32R32F]=PF_A32B32G32R32F;
        }

#ifdef _DEBUG
        if (!mapping.contains(d3dfmt))
        {
            DI_WARNING("Cannot convert the D3D9 format to DiPixelFormat!");
        }
#endif
        return mapping[d3dfmt];
    }

    DWORD DiD3D9Mappings::GetLockOption(DiLockFlag lock, DiResUsage usage)
    {
        DWORD ret = 0;
        if (lock == LOCK_DISCARD)
        {
            if (usage & RU_DYNAMIC)
                ret |= D3DLOCK_DISCARD;
        }
        if (lock == LOCK_READ_ONLY)
        {
            if (!(usage & RU_WRITE_ONLY))
                ret |= D3DLOCK_READONLY;
        }
        if (lock == LOCK_NO_OVERWRITE)
        {
            if (usage & RU_DYNAMIC)
                ret |= D3DLOCK_NOOVERWRITE;
        }
        return ret;
    }
}