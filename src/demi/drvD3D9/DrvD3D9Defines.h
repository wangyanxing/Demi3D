
/********************************************************************
    File:       DrvD3D9Defines.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DrvD3D9Defines_h__
#define DrvD3D9Defines_h__

#ifdef DEMI_STATIC_API
#    define DI_D3D9DRV_API
#else
#    ifdef DI_D3D9_EXPORT
#        define DI_D3D9DRV_API __declspec(dllexport)
#    else
#        define DI_D3D9DRV_API __declspec(dllimport)
#    endif
#endif

#endif // DrvD3D9Defines_h__
