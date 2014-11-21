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

#ifndef DiEAGL2Window_H__
#define DiEAGL2Window_H__

#include "ES2Prerequisites.h"
#include "Window.h"

#ifdef __OBJC__

#import "EAGL2View.h"
#import "EAGL2ViewController.h"

// Forward declarations
@class CAEAGLLayer;

// Define the native window type
typedef UIWindow *NativeWindowType;

#endif

namespace Demi
{
    class DiEAGL2Util;
    class DiEAGLES2Context;

    class DI_GLES2_API DiEAGL2Window : public DiWindow
    {
        protected:
        
            bool mIsFullScreen;
        
            bool mActive;
        
            bool mClosed;
        
            bool mVisible;
        
            uint32 mWidth;
        
            uint32 mHeight;
        
            /// Is this using an external window handle?
            bool mIsExternal;
            /// Is this using an external view handle?
            bool mUsingExternalView;
            /// Is this using an external view controller handle?
            bool mUsingExternalViewController;

            // iOS 4 content scaling
            bool mIsContentScalingSupported;
        
            float mContentScalingFactor;
        
            DiString mName;

            DiEAGL2Util* mGLSupport;
        
            DiEAGLES2Context* mContext{ nullptr };
#ifdef __OBJC__
            NativeWindowType mWindow{ nil };
            EAGL2View *mView{ nil };
            EAGL2ViewController *mViewController{ nil };
#endif

            void SwitchFullScreen(bool fullscreen) { }
			
            void GetLeftAndTopFromNativeWindow(int & left, int & top, uint width, uint height);
			
            void InitNativeCreatedWindow();
			
            void CreateNativeWindow(int &left, int &top, uint &width, uint &height, DiString &title);
			
            void Reposition(int left, int top);

	public:
            DiEAGL2Window(DiEAGL2Util* glsupport);
        
            virtual ~DiEAGL2Window();

            bool Create(uint32& width, uint32& height,
                                   const DiString& title, bool fullscreen);

			void SetFullscreen(bool fullscreen, uint width, uint height);
        
            DiEAGLES2Context* GetContext() {return mContext;}
        
            void Destroy(void);
        
            bool IsClosed(void) const { return mClosed; }
        
            bool IsVisible(void) const { return mVisible; }

            void SetVisible(bool visible) { mVisible = visible; }
        
            void SetClosed(bool closed) { mClosed = closed; }
        
            void SetWindowSize(uint32 width, uint32 height);
        
            void GetWindowSize(uint32& width, uint32& height);
        
            void SwapBuffers();
        
            void OnMoveOrResize();
        
            void Update();
        
            void Resize(uint32 width, uint32 height);
        
            void GetTitle(char *title, uint32 maxLength) const;
        
            void SetTitle(const char *title);
        
            bool Close();
        
            bool IsOpen();
        
            DiPixelFormat SuggestPixelFormat() const
            {
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
                return PF_R8G8B8A8; //TODO
#else
                return PF_A8B8G8R8;
#endif
            }
    };
}

#endif
