


#ifndef OSXCocoaContext_h__
#define OSXCocoaContext_h__

#include "GLContext.h"
#include <Cocoa/Cocoa.h>

namespace Demi
{
    class DI_GLDRV_API DiOSXCocoaContext : public DiGLContext
    {
    public:

        DiOSXCocoaContext(NSOpenGLContext *context, NSOpenGLPixelFormat *pixelFormat);

        ~DiOSXCocoaContext();

    public:

        void                    BeginContext();

        void                    EndContext();

        void                    Release();

        DiGLContext*            Clone() const;
        
        NSOpenGLContext*        GetContext();
		
		NSOpenGLPixelFormat*    GetPixelFormat();

    private:

        DiOSXGLUtil*            mGLUtil;
        
		NSOpenGLContext*        mNSGLContext;
        
		NSOpenGLPixelFormat*    mNSGLPixelFormat;
    };
}

#endif