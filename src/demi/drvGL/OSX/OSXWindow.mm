
#include "GfxPch.h"
#include "OSXWindow.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "GfxDriver.h"

@implementation DemiWindow

- (BOOL)canBecomeKeyWindow
{
    return YES;
}

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

@end

namespace Demi
{
    DiOSXWindow::DiOSXWindow() : mWindow(nil), mView(nil), mGLContext(nil), mWindowOrigin(NSZeroPoint),
        mWindowDelegate(NULL), mActive(false), mClosed(false), mIsFullScreen(false),
        mHasResized(false), mIsExternal(false), mUseNSView(false), mHidden(false)
    {
        mRect.SetNull();
        mDestroyWindow = false;
    }

    DiOSXWindow::~DiOSXWindow()
    {
        [mGLContext clearDrawable];
        
        Destroy();
        
        if(mWindow)
        {
            [mWindow release];
            mWindow = nil;
        }
        
        if(mWindowDelegate)
        {
            [mWindowDelegate release];
            mWindowDelegate = nil;
        }
    }

    bool DiOSXWindow::Create(uint32& width, uint32& height,
        const char* title, bool fullscreen)
    {
        bool ok = false;
        return ok;
    }

    void DiOSXWindow::Update()
    {
    }

    bool DiOSXWindow::Close()
    {
        return true;
    }

    bool DiOSXWindow::IsOpen()
    {
        return mWndHandle ? true : false;
    }

    void DiOSXWindow::SetWindowSize(uint32 width, uint32 height)
    {
    }

    void DiOSXWindow::GetWindowSize(uint32& width, uint32& height)
    {
    }

    void DiOSXWindow::GetTitle(char *title, uint32 maxLength) const
    {
    }

    void DiOSXWindow::SetTitle(const char *title)
    {
    }
    
    void DiOSXWindow::SetActive(bool act)
    {
        mActive = act;
    }
    
    void DiOSXWindow::OnMoveOrResize()
    {
        if(!mIsFullScreen)
        {
            NSRect frame = [mWindow frame];
            NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
            
            mRect.left = (long)frame.origin.x;
            mRect.top = (int)(screenFrame.size.height - frame.size.height);
            mRect.right = (long)frame.size.width + mRect.left;
            mRect.bottom = (long)frame.size.height + mRect.top;
            
            mWindowOrigin = NSMakePoint(mRect.left, mRect.top);
        }
        
		[mGLContext update];
    }
    
    void DiOSXWindow::SetVSync(bool val)
    {
        mVSync = val;
        mContext->BeginContext();
        
        GLint vsyncInterval = mVSync ? 1 : 0;
        [mGLContext setValues:&vsyncInterval forParameter:NSOpenGLCPSwapInterval];
        
        mContext->EndContext();
        
        if(mGLContext != [NSOpenGLContext currentContext])
            [mGLContext makeCurrentContext];

    }
    
    void DiOSXWindow::SetHidden(bool hidden)
    {
        mHidden = hidden;
        if (!mIsExternal)
        {
            if (hidden)
                [mWindow orderOut:nil];
            else
                [mWindow makeKeyAndOrderFront:nil];
        }
    }
    
    void DiOSXWindow::Destroy()
    {
        if(!mIsFullScreen)
        {
            // Unregister and destroy the GLContext
            DI_DELETE mContext;
            
            if(!mIsExternal)
            {
                // Remove the window from the Window listener
                //WindowEventUtilities::_removeRenderWindow(this);
            }
            
            if(mGLContext)
            {
                [mGLContext release];
                mGLContext = nil;
            }
            
            if(mWindow)
            {
                [mWindow performClose:nil];
                
                if(mGLPixelFormat)
                {
                    [mGLPixelFormat release];
                    mGLPixelFormat = nil;
                }
                
            }
		}
		
        mActive = false;
        mClosed = true;
    }
}
