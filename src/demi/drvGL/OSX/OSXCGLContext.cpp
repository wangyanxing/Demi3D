
#include "DrvGLPch.h"
#include "OSX/OSXCGLContext.h"
#include "OSX/OSXGLUtil.h"

namespace Demi
{
	DiOSXCGLContext::DiOSXCGLContext(CGLContextObj cglContext,
                                     CGLPixelFormatObj pixelFormat) :
        mCGLContext(cglContext), mPixelFormat(pixelFormat)
    {
    }
    
    DiOSXCGLContext::~DiOSXCGLContext()
    {
        //_unregisterContext();
		
        if(mPixelFormat != NULL)
        {
            CGLDestroyPixelFormat(mPixelFormat);
            mPixelFormat = NULL;
        }
    }

    void DiOSXCGLContext::BeginContext()
    {
        CGLSetCurrentContext(mCGLContext);
    }
    
    void DiOSXCGLContext::EndContext()
    {
        CGLSetCurrentContext(nullptr);
    }
    
    void DiOSXCGLContext::Release()
    {
    }
    
    DiGLContext* DiOSXCGLContext::Clone() const
    {
        CGLContextObj cglCtxShare;
        CGLCreateContext(mPixelFormat, mCGLContext, &cglCtxShare);
        
		return DI_NEW DiOSXCGLContext(cglCtxShare, mPixelFormat);
    }
}