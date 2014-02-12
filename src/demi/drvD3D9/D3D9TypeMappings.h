


#ifndef DiD3D9TypeMappings_h__
#define DiD3D9TypeMappings_h__


namespace Demi
{
    class DiD3D9Mappings
    {
    public:

        static D3DTEXTUREFILTERTYPE D3D9MinMagFilter[FILTER_MAX-1];

        static D3DTEXTUREFILTERTYPE D3D9MipFilter[FILTER_MAX-1];

        static D3DTEXTUREADDRESS    D3D9AddModeMapping[AM_MAX];

        static D3DFORMAT            D3D9FormatMapping[PIXEL_FORMAT_MAX];
        
        static D3DPRIMITIVETYPE     D3D9PrimTypeMapping[PT_MAX];

        static DiPixelFormat        ConvertPixFormat(D3DFORMAT d3dfmt);

        static DWORD                GetLockOption(DiLockFlag lock, DiResUsage usage);
    };
}

#endif
