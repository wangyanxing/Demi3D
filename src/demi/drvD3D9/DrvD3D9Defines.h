/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

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
