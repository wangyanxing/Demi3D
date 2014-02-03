
#include "GfxPch.h"
#include "OSXWindow.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "GLDriver.h"

#include "OSXCocoaContext.h"
#include "OSXWindowDelegate.h"

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
        const DiString& title, bool fullscreen)
    {
        bool ok = false;
        
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSApplicationLoad();
        
        BOOL hasDepthBuffer = YES;
		int fsaa_samples = 0;
        bool hidden = false;
        NSString *windowTitle = [NSString stringWithCString:title.c_str() encoding:NSUTF8StringEncoding];
		int winx = 0, winy = 0;
		int depth = 32;
        
        mIsFullScreen = false;

        NSOpenGLPixelFormatAttribute attribs[30];
        int i = 0;
        
        // Specify the display ID to associate the GL context with (main display for now)
        // Useful if there is ambiguity
        attribs[i++] = NSOpenGLPFAScreenMask;
        attribs[i++] = (NSOpenGLPixelFormatAttribute)CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID());
        
        // Specifying "NoRecovery" gives us a context that cannot fall back to the software renderer.
        // This makes the View-based context a compatible with the fullscreen context, enabling us to use
        // the "shareContext" feature to share textures, display lists, and other OpenGL objects between the two.
        attribs[i++] = NSOpenGLPFANoRecovery;
        
        attribs[i++] = NSOpenGLPFAAccelerated;
        attribs[i++] = NSOpenGLPFADoubleBuffer;
        
        attribs[i++] = NSOpenGLPFAColorSize;
        attribs[i++] = (NSOpenGLPixelFormatAttribute) depth;
        
        attribs[i++] = NSOpenGLPFAAlphaSize;
        attribs[i++] = (NSOpenGLPixelFormatAttribute) 8;
        
        attribs[i++] = NSOpenGLPFAStencilSize;
        attribs[i++] = (NSOpenGLPixelFormatAttribute) 8;
        
        attribs[i++] = NSOpenGLPFADepthSize;
        attribs[i++] = (NSOpenGLPixelFormatAttribute) (hasDepthBuffer? 16 : 0);
        
        if(fsaa_samples > 0)
        {
            attribs[i++] = NSOpenGLPFAMultisample;
            attribs[i++] = NSOpenGLPFASampleBuffers;
            attribs[i++] = (NSOpenGLPixelFormatAttribute) 1;
            
            attribs[i++] = NSOpenGLPFASamples;
            attribs[i++] = (NSOpenGLPixelFormatAttribute) fsaa_samples;
        }
        
        attribs[i++] = (NSOpenGLPixelFormatAttribute) 0;
        mGLPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attribs];

        DiGLDriver* drv = static_cast<DiGLDriver*>(Driver);
        
        DiOSXCocoaContext *mainContext = (DiOSXCocoaContext*)drv->GetMainContext();
        NSOpenGLContext *shareContext = mainContext == 0 ? nil : mainContext->GetContext();
        
        mGLContext = [[NSOpenGLContext alloc] initWithFormat:mGLPixelFormat shareContext:shareContext];
        
        GLint swapInterval = 1;
        [mGLContext setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

        SetHidden(hidden);
        mActive = true;
        mClosed = false;
        //mName = [windowTitle cStringUsingEncoding:NSUTF8StringEncoding];
        //mWidth = width;
        //mHeight = height;
        //mColourDepth = depth;
        //mFSAA = fsaa_samples;
        mRect = DiRect(0,0,width,height);
        
        if(true)
        {
            // create a new window
            _CreateWindow(width, height, [windowTitle cStringUsingEncoding:NSUTF8StringEncoding]);
        }
        else
        {
            // TODO: use an external window
        }
        
        mWndHandle = (void*)mWindow;
        
        mContext = DI_NEW DiOSXCocoaContext(mGLContext, mGLPixelFormat);
        mWindowDelegate = [[OSXWindowDelegate alloc] initWithNSWindow:mWindow demiWindow:this];
        
        CGLLockContext((CGLContextObj)[mGLContext CGLContextObj]);
        
        [mView setNeedsDisplay:YES];
        
        if([mGLContext view] != mView)
            [mGLContext setView:mView];
        [mGLContext makeCurrentContext];
        [mGLContext update];
        
        drv->Clear(CLEAR_COLOR, DiColor::Black, 1.0f);
        
        [mGLContext flushBuffer];
        CGLUnlockContext((CGLContextObj)[mGLContext CGLContextObj]);
        
        [pool drain];
        
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
        width = mRect.Width();
        height = mRect.Height();
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
    
    void DiOSXWindow::SwapBuffers()
    {
        CGLLockContext((CGLContextObj)[mGLContext CGLContextObj]);
        [mGLContext makeCurrentContext];
        
        if([mGLContext view] != mView)
            [mGLContext setView:mView];
        
        [mGLContext flushBuffer];
        CGLUnlockContext((CGLContextObj)[mGLContext CGLContextObj]);
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
    
    void DiOSXWindow::_SetWindowParams()
    {
        if(mWindow)
        {
            if(mIsFullScreen)
            {
                // Set the backing store size to the viewport dimensions
                // This ensures that it will scale to the full screen size
                GLint backingStoreDimensions[2] = { (GLint)mRect.Width(), (GLint)mRect.Height() };
                CGLSetParameter((CGLContextObj)[mGLContext CGLContextObj], kCGLCPSurfaceBackingSize, backingStoreDimensions);
                CGLEnable((CGLContextObj)[mGLContext CGLContextObj], kCGLCESurfaceBackingSize);
                
                NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
                NSRect windowRect = NSMakeRect(0.0, 0.0, mainDisplayRect.size.width, mainDisplayRect.size.height);
                [mWindow setFrame:windowRect display:YES];
                [mView setFrame:windowRect];
                
                // Set window properties for full screen and save the origin in case the window has been moved
                [mWindow setStyleMask:NSBorderlessWindowMask];
                [mWindow setOpaque:YES];
                [mWindow setHidesOnDeactivate:YES];
                [mWindow setContentView:mView];
                [mWindow setFrameOrigin:NSZeroPoint];
                [mWindow setLevel:NSMainMenuWindowLevel+1];
                [NSApp activateIgnoringOtherApps:YES];
                
                mWindowOrigin = mWindow.frame.origin;
                mRect.left = mRect.top = 0;
            }
            else
            {
                // Reset and disable the backing store in windowed mode
                GLint backingStoreDimensions[2] = { 0, 0 };
                CGLSetParameter((CGLContextObj)[mGLContext CGLContextObj], kCGLCPSurfaceBackingSize, backingStoreDimensions);
                CGLDisable((CGLContextObj)[mGLContext CGLContextObj], kCGLCESurfaceBackingSize);
                
                NSRect viewRect = NSMakeRect(mWindowOrigin.x, mWindowOrigin.y, mRect.Width(), mRect.Height());
                [mWindow setFrame:viewRect display:YES];
                [mView setFrame:viewRect];
                [mWindow setStyleMask:NSResizableWindowMask|NSTitledWindowMask];
                [mWindow setOpaque:YES];
                [mWindow setHidesOnDeactivate:NO];
                [mWindow setContentView:mView];
                [mWindow setLevel:NSNormalWindowLevel];
                [mWindow center];
                
                // Set the drawable, and current context
                // If you do this last, there is a moment before the rendering window pops-up
                [mGLContext makeCurrentContext];
            }
            
            [mGLContext update];
            
            // Even though OgreCocoaView doesn't accept first responder, it will get passed onto the next in the chain
            [mWindow makeFirstResponder:mView];
        }

    }
    
    void DiOSXWindow::_CreateWindow(unsigned int width, unsigned int height, const char* title)
    {
        NSRect windowRect = NSZeroRect;
        if(mIsFullScreen)
        {
            NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
            windowRect = NSMakeRect(0.0, 0.0, mainDisplayRect.size.width, mainDisplayRect.size.height);
        }
        else
            windowRect = NSMakeRect(0.0, 0.0, width, height);
        
        mWindow = [[DemiWindow alloc] initWithContentRect:windowRect
                                                styleMask:mIsFullScreen ? NSBorderlessWindowMask : NSResizableWindowMask|NSTitledWindowMask
                                                  backing:NSBackingStoreBuffered
                                                    defer:YES];
        [mWindow setTitle:[NSString stringWithCString:title encoding:NSUTF8StringEncoding]];
        mWindowTitle = title;
        
        mView = [[DemiView alloc] initWithGLOSXWindow:this];
        
        _SetWindowParams();

        //DiGLDriver* drv = static_cast<DiGLDriver*>(Driver);
        //drv->Clear(CLEAR_COLOR, DiColor::Black, 1.0f);
        
        // Show window
        if(mWindow)
            [mWindow makeKeyAndOrderFront:nil];
    }
}
