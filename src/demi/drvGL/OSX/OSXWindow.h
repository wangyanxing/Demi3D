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

#ifndef OSXWindow_h__
#define OSXWindow_h__

#include "Window.h"

#include <Cocoa/Cocoa.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLTypes.h>

#include "OSXCocoaContext.h"
#include "OSXWindowView.h"
#include "OSXWindowDelegate.h"

@class OSXWindowDelegate;

@interface DemiWindow : NSWindow

@end

namespace Demi
{
    class DI_GFX_API DiOSXWindow : public DiWindow
    {
    public:

        DiOSXWindow();

        ~DiOSXWindow();

    public:

        bool                Create(uint32& width, uint32& height,
                                   const DiString& title, bool fullscreen);

        void                Update();

        bool                Close();

        bool                IsOpen();
        
        bool                IsActive() {return mActive;}
        
        void                SetActive(bool act);
        
        void                SetHidden(bool hidden);

        void                SetWindowSize(uint32 width, uint32 height);

        void                GetWindowSize(uint32& width, uint32& height);

        void                GetTitle(char *title, uint32 maxLength) const;

        void                SetTitle(const char *title);
        
        void                Destroy();
        
        void                SetVSync(bool val);
        
        void                OnMoveOrResize();
        
        void                SwapBuffers();
        
    private:
        
        void                _CreateWindow(unsigned int width, unsigned int height, const char* title);
        
        void                _SetWindowParams();
        
    private:
        
        bool                    mActive;
        
        bool                    mClosed;
        
        bool                    mIsFullScreen;
        
        bool                    mHasResized;
        
        bool                    mIsExternal;
        
        bool                    mHidden;
        
        DiRect                  mRect;
        
        DiString                mWindowTitle;
        
        bool                    mUseNSView;
        
        NSWindow*               mWindow;

        NSView*                 mView;
        
        NSOpenGLContext*        mGLContext;
        
        NSOpenGLPixelFormat*    mGLPixelFormat;
        
        NSPoint                 mWindowOrigin;
        
        OSXWindowDelegate*      mWindowDelegate;

        DiOSXCocoaContext*      mContext;
        
        CGLContextObj           mCGLContextObj;
        
#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_6
        CGDisplayModeRef        mOriginalDisplayMode;
#else
        CFDictionaryRef         mOriginalDisplayMode;
#endif
        
    };
}

#endif