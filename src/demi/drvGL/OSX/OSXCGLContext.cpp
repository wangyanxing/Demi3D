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