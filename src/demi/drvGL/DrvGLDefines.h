
/********************************************************************
    File:       DrvGLDefines.h
    Creator:    demiwangya
*********************************************************************/

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

#endif // DrvGLDefines_h__