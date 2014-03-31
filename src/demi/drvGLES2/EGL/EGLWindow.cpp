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
#include "EGLWindow.h"

namespace Demi
{
    DiEGLWindow::DiEGLWindow(DiEGLUtil* util)
        : DiWindow()
        , mGLSupport(util)
        , mContext(0)
        , mEglConfig(0)
        , mEglSurface(0)
        , mWindow(0)
        , mNativeDisplay(0)
        , mEglDisplay(EGL_NO_DISPLAY)
    {
        mIsTopLevel = false;
        mClosed = false;
        mIsExternalGLControl = false;
        mVisible = false;
    }

    DiEGLWindow::~DiEGLWindow()
    {
    }

    ::EGLSurface DiEGLWindow::CreateSurfaceFromWindow(::EGLDisplay display, NativeWindowType win)
    {
        ::EGLSurface surface;

        surface = eglCreateWindowSurface(display, mEglConfig, (EGLNativeWindowType)win, NULL);
        EGL_CHECK_ERROR

        if (surface == EGL_NO_SURFACE)
        {
            DI_WARNING("Fail to create EGLSurface based on X NativeWindowType");
        }
        return surface;
    }

    void DiEGLWindow::SwapBuffers()
    {
        if (mClosed || mIsExternalGLControl)
        {
            return;
        }

        if (eglSwapBuffers(mEglDisplay, mEglSurface) == EGL_FALSE)
        {
            EGL_CHECK_ERROR
            DI_WARNING("Fail to swap buffer");
        }
    }

}