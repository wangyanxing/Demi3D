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
/********************************************************************
    File:     EngineDefines.h
*********************************************************************/

#ifndef GfxDefines_h__
#define GfxDefines_h__

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DI_GFX_API
#   else
#       ifdef DI_GFX_EXPORT
#           define DI_GFX_API __declspec(dllexport)
#       else
#           define DI_GFX_API __declspec(dllimport)
#       endif
#   endif
#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
#   ifdef DEMI_STATIC_API
#       define DI_GFX_API
#   else
#       if defined( DEMI_GCC_VISIBILITY )
#           define DI_GFX_API  __attribute__ ((visibility("default")))
#       else
#           define DI_GFX_API
#       endif
#   endif
#endif

#ifndef MAKEFOURCC
#   define MAKEFOURCC(ch0, ch1, ch2, ch3)    ((unsigned long)(uint8)(ch0) | ((unsigned long)(uint8)(ch1) << 8) \
    | ((unsigned long)(uint8)(ch2) << 16) | ((unsigned long)(uint8)(ch3) << 24 ))
#endif

#ifndef MAKEFOURCCSTR
#   define MAKEFOURCCSTR(str) MAKEFOURCC(str[0],str[1],str[2],str[3])
#endif

typedef void*   DiWndHandle;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   ifdef max
#       undef max
#   endif

#   ifdef min
#       undef min
#   endif
#endif

#endif // GfxDefines_h__
