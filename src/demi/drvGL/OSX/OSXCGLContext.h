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

#ifndef OSXCGLContext_h__
#define OSXCGLContext_h__

#include "GLContext.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLTypes.h>

namespace Demi
{
    class DI_GLDRV_API DiOSXCGLContext : public DiGLContext
    {
    public:

        DiOSXCGLContext(CGLContextObj cglContext,
                        CGLPixelFormatObj pixelFormat);

        ~DiOSXCGLContext();

    public:

        void            BeginContext();

        void            EndContext();

        void            Release();

        DiGLContext*    Clone() const;

        void            SwapBuffer(){}
        
        CGLContextObj   GetContext() { return mCGLContext; }

    private:
        
        CGLContextObj       mCGLContext;
        
        CGLPixelFormatObj   mPixelFormat;
    };
}

#endif