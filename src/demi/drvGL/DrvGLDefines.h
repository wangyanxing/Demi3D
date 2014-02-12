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

#ifndef DrvGLDefines_h__
#define DrvGLDefines_h__

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DI_GLDRV_API
#   else
#       ifdef DI_GL_EXPORT
#           define DI_GLDRV_API  __declspec(dllexport)
#       else
#           define DI_GLDRV_API __declspec(dllimport)
#       endif
#   endif
#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
#   ifdef DEMI_STATIC_API
#       define DI_GLDRV_API
#   else
#       if defined( DEMI_GCC_VISIBILITY )
#           define DI_GLDRV_API  __attribute__ ((visibility("default")))
#       else
#           define DI_GLDRV_API
#       endif
#   endif
#endif


#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   if !defined( __MINGW32__ )
#      define WIN32_LEAN_AND_MEAN
#      ifndef NOMINMAX
#	       define NOMINMAX
#      endif
#   endif
#   include <windows.h>
#   include <wingdi.h>
#   include <GL/glew.h>
#   include <GL/wglew.h>
#   include <GL/glu.h>
#elif DEMI_PLATFORM == DEMI_PLATFORM_LINUX
#   include <GL/glew.h>
#   include <GL/glu.h>
#   define GL_GLEXT_PROTOTYPES
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include <GL/glew.h>
#   include <OpenGL/glu.h>
#endif

/// Lots of generated code in here which triggers the new VC CRT security warnings
#if !defined( _CRT_SECURE_NO_DEPRECATE )
#define _CRT_SECURE_NO_DEPRECATE
#endif

#endif // DrvGLDefines_h__