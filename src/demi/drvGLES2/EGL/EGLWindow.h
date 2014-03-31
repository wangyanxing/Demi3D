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

#ifndef DiEGLWindow_h__
#define DiEGLWindow_h__

#include "Window.h"

namespace Demi
{
    class DI_GLES2_API DiEGLWindow : public DiWindow
    {
    public:

        DiEGLWindow(DiEGLUtil* util);

        virtual ~DiEGLWindow();

    public:

        virtual bool        Create(uint32& width, uint32& height,
            const DiString& title, bool fullscreen) = 0;

        virtual void        Update() = 0;

        virtual bool        Close() = 0;

        virtual bool        IsOpen() = 0;

        virtual void        SetVSync(bool vsync) {}

        bool                IsVSync() const { return mVSync; }

        bool                DestroyingWindow() const { return mDestroyWindow; }

        void                SetParentRenderWnd(DiRenderWindow* parent) { mParentWnd = parent; }

        DiRenderWindow*     GetParentRenderWnd() { return mParentWnd; }

        DiWndHandle         GetWndHandle() { return mWndHandle; }

        DiEGLContext*       GetContext() { return mContext; }

        virtual void        SetWindowSize(uint32 width, uint32 height) = 0;

        virtual void        GetWindowSize(uint32& width, uint32& height) = 0;

        virtual void        GetTitle(char *title, uint32 maxLength) const = 0;

        virtual void        SetTitle(const char *title) = 0;

        virtual void        OnMoveOrResize() {}

        virtual void        SwapBuffers();

    protected:

        ::EGLSurface        CreateSurfaceFromWindow(::EGLDisplay display, NativeWindowType win);

    protected:

        bool                mClosed;

        bool                mVisible;
        
        bool                mIsTopLevel;
        
        bool                mIsExternal;
        
        bool                mIsExternalGLControl;

        DiEGLUtil*          mGLSupport;

        DiEGLContext*       mContext;
        
        NativeWindowType    mWindow;
        
        NativeDisplayType   mNativeDisplay;
        
        ::EGLDisplay        mEglDisplay;

        ::EGLConfig         mEglConfig;
        
        ::EGLSurface        mEglSurface;
    };
}
#endif 