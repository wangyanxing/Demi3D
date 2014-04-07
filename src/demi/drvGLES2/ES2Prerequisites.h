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

#include "MiscPrerequisites.h"
#include "GfxPrerequisites.h"

namespace Demi
{
    class DiGLES2StateCache;
    class DiGLES2UniformCache;
    class DiGLES2Driver;
    class DiGLES2Context;
    class DiGLES2Util;
    class DiGLES2TextureDrv;
    class DiGLES2FrameBuffer;
    class DiGLES2DepthBuffer;
    class DiGLES2RenderBuffer;
    class DiGLES2FBOManager;
    class DiGLES2WindowTarget;
    class DiGLES2GpuVariable;
    class DiGLES2ShaderLinker;
    class DiGLES2ShaderParam;
    class DiGLES2ShaderInstance;
    class DiGLES2ShaderPipeline;
    class DiGLES2VertexDeclaration;
    struct DiGLES2ShaderConstant;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    class DiEGLUtil;
    class DiEGLContext;
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
    class DiEAGL2Util;
    class DiEAGLES2Context;
    class DiEAGL2Window;
#endif
}

#include "DrvGLES2Defines.h"

#endif