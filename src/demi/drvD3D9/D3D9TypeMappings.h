
/********************************************************************
    File:       D3D9TypeMappings.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DiD3D9Mappings
    {
    public:

        static D3DTEXTUREFILTERTYPE D3D9FilterMapping[FT_MAX];

        static D3DTEXTUREADDRESS    D3D9AddModeMapping[AM_MAX];

        static D3DFORMAT            D3D9FormatMapping[PIXEL_FORMAT_MAX];
        
        static D3DPRIMITIVETYPE     D3D9PrimTypeMapping[PT_MAX];

        static DiPixelFormat        ConvertPixFormat(D3DFORMAT d3dfmt);

        static DWORD                GetLockOption(DiLockFlag lock, DiResUsage usage);

    };
}