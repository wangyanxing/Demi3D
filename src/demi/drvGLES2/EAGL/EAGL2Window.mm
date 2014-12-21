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

#include "DrvGLES2Pch.h"
#include "EAGL2Window.h"
#include "EAGL2Util.h"
#include "EAGLES2Context.h"
#include "GLES2Driver.h"

#include <iomanip>

#import <UIKit/UIWindow.h>
#import <UIKit/UIGraphics.h>
#import <UIKit/UIScreen.h>

namespace Demi
{
    DiEAGL2Window::DiEAGL2Window(DiEAGL2Util *glsupport)
        :   mClosed(false),
            mVisible(false),
            mIsExternal(false),
            mUsingExternalView(false),
            mUsingExternalViewController(false),
            mIsContentScalingSupported(false),
            mContentScalingFactor(1.0),
            mGLSupport(glsupport),
            mActive(true),
            mIsFullScreen(true),
            mWidth(0),
            mHeight(0)
    {
        // Check for content scaling.  iOS 4 or later
        if(mGLSupport->GetCurrentOSVersion() >= 4.0)
            mIsContentScalingSupported = true;
    }

    DiEAGL2Window::~DiEAGL2Window()
    {
        Destroy();

        if (mContext != NULL)
        {
            DI_DELETE mContext;
        }

        mContext = NULL;
    }
    
    void DiEAGL2Window::Update()
    {
        
    }

    void DiEAGL2Window::Destroy(void)
    {
        if (mClosed)
        {
            return;
        }

        mClosed = true;
        mActive = false;

        if (!mIsExternal)
        {
            [mWindow release];
            mWindow = nil;
        }

        if (mIsFullScreen)
        {
            SwitchFullScreen(false);
        }

        if(!mUsingExternalViewController)
            [mViewController release];
    }

    void DiEAGL2Window::SetFullscreen(bool fullscreen, uint width, uint height)
    {
    }

    void DiEAGL2Window::Reposition(int left, int top)
	{
	}
    
	void DiEAGL2Window::Resize(uint32 width, uint32 height)
	{
        if(!mWindow) return;

        float w = mContentScalingFactor, h = mContentScalingFactor;

        // Check the orientation of the view controller and adjust dimensions
        if (UIInterfaceOrientationIsPortrait(mViewController.interfaceOrientation))
        {
            h *= std::max(width, height);
            w *= std::min(width, height);
        }
        else
        {
            w *= std::max(width, height);
            h *= std::min(width, height);
        }

        // Check if the window size really changed
        if(mWidth == w && mHeight == h)
            return;
        
        // Destroy and recreate the framebuffer with new dimensions 
        mContext->DestroyFramebuffer();
        
        mWidth = w;
        mHeight = h;
        
        mContext->CreateFramebuffer();
	}
    
    bool DiEAGL2Window::IsOpen()
    {
        return mWindow ? true : false;
    }
    
    void DiEAGL2Window::SetWindowSize(uint32 width, uint32 height)
    {
        Resize(width, height);
    }
    
    void DiEAGL2Window::GetWindowSize(uint32& width, uint32& height)
    {
        width = mWidth;
        height = mHeight;
    }
    
	void DiEAGL2Window::OnMoveOrResize()
	{
		CGRect frame = [mView frame];
		mWidth = (unsigned int)frame.size.width;
		mHeight = (unsigned int)frame.size.height;
	}

    void DiEAGL2Window::InitNativeCreatedWindow()
    {
        // This method is called from within create() and after parameters have been parsed.
        // If the window, view or view controller objects are nil at this point, it is safe
        // to assume that external handles are either not being used or are invalid and
        // we can create our own.
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
        CGRect screenBounds = [[UIScreen mainScreen] bounds];
        mContentScalingFactor = [[UIScreen mainScreen] scale];
        
        mWidth = screenBounds.size.width;
        mHeight = screenBounds.size.height;
        
        // Set us up with an external window, or create our own.
        if(!mIsExternal)
        {
            mWindow = [[[UIWindow alloc] initWithFrame : screenBounds] retain];
        }
        
        DI_ASSERT(mWindow);
        mWndHandle = mWindow;
        
        // Set up the view
        if(!mUsingExternalView)
        {
            mView = [[EAGL2View alloc] initWithFrame : screenBounds];
            mView.opaque = YES;

            // Use the default scale factor of the screen
            // See Apple's documentation on supporting high resolution devices for more info
            mView.contentScaleFactor = mContentScalingFactor;
        }
    
        DI_ASSERT(mView);
        
        [mView setMWindow : this];

        DI_ASSERT([mView.layer isKindOfClass:[CAEAGLLayer class]]);
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)mView.layer;
        DI_ASSERT(eaglLayer != nil);

        BOOL retainedBacking = NO;

        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:retainedBacking], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        // Set up the view controller
        if(!mUsingExternalViewController)
        {
            mViewController = [[EAGL2ViewController alloc] init];
        }
        
        DI_ASSERT(mViewController);
        
        if(mViewController.view != mView)
            mViewController.view = mView;
        
        mWndViewHandle = mViewController.view;
        mWndViewCtrlHandle = mViewController;

        CFDictionaryRef dict;   // TODO: Dummy dictionary for now
        if(eaglLayer)
        {
            EAGLSharegroup *group = nil;
            
#if 0
            if ((option = miscParams->find("externalSharegroup")) != miscParams->end())
            {
                group = (EAGLSharegroup *)StringConverter::parseUnsignedLong(option->second);
                DI_LOG("iOS: Using an external EAGLSharegroup");
            }
#endif
            
            mContext = mGLSupport->CreateNewContext(dict, eaglLayer, group);

            mContext->mIsMultiSampleSupported = true;
            mContext->mNumSamples = 0; // TODO
        }
        
        DI_ASSERT(mContext);

        if(!mUsingExternalViewController)
            [mWindow addSubview:mViewController.view];
        
        mViewController.mGLSupport = mGLSupport;
        
        if(!mUsingExternalViewController)
            mWindow.rootViewController = mViewController;
        
        if(!mUsingExternalView)
            [mView release];
    
        if(!mUsingExternalViewController)
            [mWindow makeKeyAndVisible];

        mContext->CreateFramebuffer();
        
        
        DI_LOG("iOS Window created: %d x %d", mWidth,mHeight);
        DI_LOG("Backbuffer size: %d x %d, scale factor: %g", mContext->mBackingWidth, mContext->mBackingHeight,
               mContentScalingFactor);
        
        [pool release];
    }
    
    void DiEAGL2Window::GetTitle(char *title, uint32 maxLength) const
    {
        DI_ASSERT_FAIL; //todo
    }
    
    void DiEAGL2Window::SetTitle(const char *title)
    {
        DI_ASSERT_FAIL; //todo
    }
    
    bool DiEAGL2Window::Close()
    {
        return true;
    }
    
    bool DiEAGL2Window::Create(uint32& width, uint32& height,
                               const DiString& title, bool fullscreen)
    {
        //short frequency = 0;
        //bool vsync = false;
        
        mIsFullScreen = false;
        mWidth = width;
        mHeight = height;
        mName = title;

        // Check the configuration. This may be overridden later by the value sent via miscParams
        // mContentScalingFactor
        
        InitNativeCreatedWindow();

        // Resize, taking content scaling factor into account
        Resize(mWidth, mHeight);

		mActive = true;
		mVisible = true;
		mClosed = false;
        
        return true;
    }

    void DiEAGL2Window::SwapBuffers()
    {
        if (mClosed)
            return;

        unsigned int attachmentCount = 0;
        GLenum attachments[3];
        unsigned int buffers = DiGLES2Driver::StateCache->getDiscardBuffers();
        
        if(buffers & CLEAR_COLOR)
            attachments[attachmentCount++] = GL_COLOR_ATTACHMENT0;
        
        if(buffers & CLEAR_DEPTH)
            attachments[attachmentCount++] = GL_DEPTH_ATTACHMENT;
        
        if(buffers & CLEAR_STENCIL)
            attachments[attachmentCount++] = GL_STENCIL_ATTACHMENT;
        
#if 0
        if(mContext->mIsMultiSampleSupported && mContext->mNumSamples > 0)
        {
            CHECK_GL_ERROR(glDisable(GL_SCISSOR_TEST));
            CHECK_GL_ERROR(glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, mContext->mFSAAFramebuffer));
            CHECK_GL_ERROR(glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, mContext->mViewFramebuffer));
            CHECK_GL_ERROR(glResolveMultisampleFramebufferAPPLE());
            CHECK_GL_ERROR(glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, attachmentCount, attachments));
        }
        else
#endif
        {
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mContext->mViewFramebuffer));
            CHECK_GL_ERROR(glDiscardFramebufferEXT(GL_FRAMEBUFFER, attachmentCount, attachments));
        }

        CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mContext->mViewRenderbuffer));
        if ([mContext->GetContext() presentRenderbuffer:GL_RENDERBUFFER] == NO)
        {
            glGetError();
            DI_WARNING("Failed to swap buffers in ");
        }
    }
}
