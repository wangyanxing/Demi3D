
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

#ifndef DiFxDefines_h__
#define DiFxDefines_h__

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DEMI_FX_API
#   else
#       ifdef DI_FX_EXPORT
#           define DEMI_FX_API __declspec(dllexport)
#       else
#           define DEMI_FX_API __declspec(dllimport)
#       endif
#   endif
#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
#   ifdef DEMI_STATIC_API
#       define DEMI_FX_API
#   else
#       if defined( DEMI_GCC_VISIBILITY )
#           define DEMI_FX_API  __attribute__ ((visibility("default")))
#       else
#           define DEMI_FX_API
#       endif
#   endif
#endif

#endif


