


#ifndef GLPrerequisites_h__
#define GLPrerequisites_h__

namespace Demi
{
    class DiGLStateCache;
    class DiGLDriver;
    class DiGLContext;
    class DiGLUtil;
    class DiGLBufferManager;
    class DiGLTextureDrv;
    class DiGLFrameBuffer;
    class DiGLDepthBuffer;
    class DiGLRenderBuffer;
    class DiGLFBOManager;
    class DiGLWindowTarget;
    class DiGLGpuVariable;
    class DiGLShaderLinker;
    class DiGLShaderParam;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    class DiWin32GLUtil;
    class DiWin32GLContext;
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
    class DiOSXGLUtil;
    class DiOSXGLContext;
#endif
}

#include "DrvGLDefines.h"

#endif