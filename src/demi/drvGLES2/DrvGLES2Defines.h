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


// Defines for extensions that were made core in OpenGL ES 3
#if OGRE_NO_GLES3_SUPPORT == 0
#   ifndef GL_OES_mapbuffer
#       define GL_WRITE_ONLY_OES GL_MAP_WRITE_BIT
#       define glUnmapBufferOES glUnmapBuffer
#   endif

#   ifndef GL_OES_texture_half_float
#       define GL_HALF_FLOAT_OES GL_HALF_FLOAT
#   endif

#   ifndef GL_OES_rgb8_rgba8
#       define GL_RGB8_OES GL_RGB8
#       define GL_RGBA8_OES GL_RGBA8
#   endif

#   ifndef GL_EXT_texture_rg
#       define GL_RG8_EXT GL_RG8
#       define GL_RED_EXT GL_RED
#       define GL_RG_EXT GL_RG
#       define GL_R8_EXT GL_R8
#   endif

#   ifndef GL_EXT_texture_storage
#       define GL_R16F_EXT GL_R16F
#       define GL_R32F_EXT GL_R32F
#       define GL_RG16F_EXT GL_RG16F
#       define GL_RG32F_EXT GL_RG32F
#       define GL_RGB16F_EXT GL_RGB16F
#       define GL_RGB32F_EXT GL_RGB32F
#       define GL_RGBA16F_EXT GL_RGBA16F
#       define GL_RGBA32F_EXT GL_RGBA32F
#       define GL_DEPTH_COMPONENT32_OES GL_DEPTH_COMPONENT32F
#       endif

#ifndef GL_EXT_blend_minmax
#define GL_MIN_EXT GL_MIN
#define GL_MAX_EXT GL_MAX
#endif

#ifndef GL_OES_depth24
#define GL_DEPTH_COMPONENT24_OES GL_DEPTH_COMPONENT24
#endif

#ifndef GL_OES_packed_depth_stencil
#define GL_DEPTH24_STENCIL8_OES GL_DEPTH24_STENCIL8
#endif

#ifndef GL_APPLE_texture_max_level
#define GL_TEXTURE_MAX_LEVEL_APPLE GL_TEXTURE_MAX_LEVEL
#endif

#ifndef GL_APPLE_framebuffer_multisample
#define GL_MAX_SAMPLES_APPLE GL_MAX_SAMPLES
#define glRenderbufferStorageMultisampleAPPLE glRenderbufferStorageMultisample
#endif

#ifndef GL_EXT_occlusion_query_boolean
#define GL_ANY_SAMPLES_PASSED_EXT GL_ANY_SAMPLES_PASSED
#define GL_QUERY_RESULT_EXT GL_QUERY_RESULT
#define GL_QUERY_RESULT_AVAILABLE_EXT GL_QUERY_RESULT_AVAILABLE
#define glGenQueriesEXT glGenQueries
#define glDeleteQueriesEXT glDeleteQueries
#define glBeginQueryEXT glBeginQuery
#define glEndQueryEXT glEndQuery
#define glGetQueryObjectuivEXT glGetQueryObjectuiv
#endif

#ifndef GL_EXT_map_buffer_range
#define GL_MAP_WRITE_BIT_EXT GL_MAP_WRITE_BIT
#define GL_MAP_FLUSH_EXPLICIT_BIT_EXT GL_MAP_FLUSH_EXPLICIT_BIT
#define GL_MAP_INVALIDATE_RANGE_BIT_EXT GL_MAP_INVALIDATE_RANGE_BIT
#define GL_MAP_UNSYNCHRONIZED_BIT_EXT GL_MAP_UNSYNCHRONIZED_BIT
#define GL_MAP_READ_BIT_EXT GL_MAP_READ_BIT
#define glMapBufferRangeEXT glMapBufferRange
#define glFlushMappedBufferRangeEXT glFlushMappedBufferRange
#endif

#ifndef GL_APPLE_sync
#define GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE GL_SYNC_GPU_COMMANDS_COMPLETE
#define GL_SYNC_FLUSH_COMMANDS_BIT_APPLE GL_SYNC_FLUSH_COMMANDS_BIT
#define GL_TIMEOUT_IGNORED_APPLE GL_TIMEOUT_IGNORED
#define GL_WAIT_FAILED_APPLE GL_WAIT_FAILED
#define glFenceSyncAPPLE glFenceSync
#define glClientWaitSyncAPPLE glClientWaitSync
#define glDeleteSyncAPPLE glDeleteSync
#endif

#define GL_PROGRAM_BINARY_LENGTH_OES GL_PROGRAM_BINARY_LENGTH
#define glProgramBinaryOES glProgramBinary
#define glGetProgramBinaryOES glGetProgramBinary

#define glDrawElementsInstancedEXT glDrawElementsInstanced
#define glDrawArraysInstancedEXT glDrawArraysInstanced
#define glVertexAttribDivisorEXT glVertexAttribDivisor
#define glBindVertexArrayOES glBindVertexArray
#define glGenVertexArraysOES glGenVertexArrays
#define glDeleteVertexArraysOES glDeleteVertexArrays
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