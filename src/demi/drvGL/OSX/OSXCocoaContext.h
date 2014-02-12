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