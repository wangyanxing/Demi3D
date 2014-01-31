
/********************************************************************
    File:       OSXCGLContext.h
    Creator:    demiwangya
*********************************************************************/

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