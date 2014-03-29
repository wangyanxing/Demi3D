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

#ifndef ES2Prerequisites_h__
#define ES2Prerequisites_h__

namespace Demi
{
    class DiGLES2StateCache;
    class DiGLES2UniformCache;
    class DiGLES2Driver;
    class DiGLES2Context;
    class DiGLES2Util;
    class DiGLES2BufferManager;
    class DiGLES2TextureDrv;
    class DiGLES2FrameBuffer;
    class DiGLES2DepthBuffer;
    class DiGLES2RenderBuffer;
    class DiGLES2FBOManager;
    class DiGLES2WindowTarget;
    class DiGLES2GpuVariable;
    class DiGLES2ShaderLinker;
    class DiGLES2ShaderParam;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    class DiWin32GLUtil;
    class DiWin32GLContext;
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
    class DiOSXGLUtil;
    class DiOSXGLContext;
#endif
}

#include "DrvGLES2Defines.h"

#endif