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

#ifndef DrvGLES2Defines_h__
#define DrvGLES2Defines_h__

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DI_GLES2_API
#   else
#       ifdef DI_GLES2_EXPORT
#           define DI_GLES2_API  __declspec(dllexport)
#       else
#           define DI_GLES2_API __declspec(dllimport)
#       endif
#   endif
#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
#   ifdef DEMI_STATIC_API
#       define DI_GLES2_API
#   else
#       if defined( DEMI_GCC_VISIBILITY )
#           define DI_GLES2_API  __attribute__ ((visibility("default")))
#       else
#           define DI_GLES2_API
#       endif
#   endif
#endif

#ifndef GL_GLEXT_PROTOTYPES
#  define  GL_GLEXT_PROTOTYPES
#endif

#include <GLES2/gles2w.h>

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   if !defined( __MINGW32__ )
#      define __PRETTY_FUNCTION__ __FUNCTION__
#      ifndef WIN32_LEAN_AND_MEAN
#          define WIN32_LEAN_AND_MEAN 1
#      endif
#      ifndef NOMINMAX
#	       define NOMINMAX
#      endif
#   endif

#	undef GL_GLEXT_PROTOTYPES
#   undef GL_OES_get_program_binary

#   include <windows.h>
#   include <wingdi.h>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#   include <EGL/egl.h>

#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS

#elif DEMI_PLATFORM == DEMI_PLATFORM_ANDROID

#endif

// Apple doesn't define this in their extension.  We'll do it just for convenience.
// Using the value from desktop GL
#ifndef GL_SAMPLER_2D_SHADOW_EXT
#   define GL_SAMPLER_2D_SHADOW_EXT             0x8B62
#endif

#ifndef GL_EXT_texture_filter_anisotropic
#   define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#   define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF
#endif

// Copy this definition from desktop GL.  Used for polygon modes.
#ifndef GL_FILL
#   define GL_FILL    0x1B02
#endif

/// Lots of generated code in here which triggers the new VC CRT security warnings
#if !defined( _CRT_SECURE_NO_DEPRECATE )
#define _CRT_SECURE_NO_DEPRECATE
#endif

#if (DEMI_PLATFORM == DEMI_PLATFORM_IOS)
#   define IF_IOS_VERSION_IS_GREATER_THAN(vers) \
        if(static_cast<DiEAGL2Util*>(DiGLES2Driver::GLUtil)->GetCurrentOSVersion() >= vers)
#else
#   define IF_IOS_VERSION_IS_GREATER_THAN(vers)
#endif

#define ENABLE_GL_CHECK 0

#if ENABLE_GL_CHECK
#define CHECK_GL_ERROR(glFunc) \
    { \
        glFunc; \
        int e = glGetError(); \
        if (e != 0) \
        { \
            const char * errorString = ""; \
            switch (e) \
            { \
            case GL_INVALID_ENUM:       errorString = "GL_INVALID_ENUM";        break; \
            case GL_INVALID_VALUE:      errorString = "GL_INVALID_VALUE";       break; \
            case GL_INVALID_OPERATION:  errorString = "GL_INVALID_OPERATION";   break; \
            case GL_OUT_OF_MEMORY:      errorString = "GL_OUT_OF_MEMORY";       break; \
            default:                                                            break; \
            } \
            DI_WARNING("OpenGL error 0x%04X %s in %s at line %i for %s\n", e, errorString, __PRETTY_FUNCTION__, __LINE__, #glFunc); \
        } \
    }
#else
#   define CHECK_GL_ERROR(glFunc) { glFunc; }
#endif

#if ENABLE_GL_CHECK
#define EGL_CHECK_ERROR \
    { \
        int e = eglGetError(); \
        if ((e != 0) && (e != EGL_SUCCESS))\
        { \
        DI_WARNING("EGL error 0x%04X in %s at line %i\n", e, __PRETTY_FUNCTION__, __LINE__); \
        } \
    }
#else
#define EGL_CHECK_ERROR {}
#endif

#endif // DrvGLDefines_h__