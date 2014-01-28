
#include "DrvGLPch.h"
#include "Win32GLContext.h"
#include "Win32GLUtil.h"

namespace Demi
{
    DiWin32GLContext::DiWin32GLContext(DiWin32GLUtil* glutil, DiWndHandle wnd)
        : mGLUtil(glutil)
    {
        InitFromHwnd((HWND)wnd);
        if (!glutil->GetMainHDC())
            glutil->SetMainHDC(mHDC);
    }

    DiWin32GLContext::DiWin32GLContext(DiWin32GLUtil* glutil, HDC dc, HGLRC glrc)
        : mGLUtil(glutil),
        mHDC(dc),
        mGLRc(glrc)
    {
        if (!glutil->GetMainHDC())
            glutil->SetMainHDC(mHDC);
    }

    DiWin32GLContext::~DiWin32GLContext()
    {
    }

    void DiWin32GLContext::BeginContext()
    {
        wglMakeCurrent(mHDC, mGLRc);
    }

    void DiWin32GLContext::EndContext()
    {
        wglMakeCurrent(NULL, NULL);
    }

    void DiWin32GLContext::Release()
    {
        if (mGLRc)
        {
            wglDeleteContext(mGLRc);
            mGLRc = NULL;
            mHDC = NULL;
        }
    }

    DiGLContext* DiWin32GLContext::Clone() const
    {
        HGLRC newCtx = wglCreateContext(mHDC);

        if (!newCtx)
        {
            DI_WARNING("Error calling wglCreateContext");
            return nullptr;
        }

        HGLRC oldrc = wglGetCurrentContext();
        HDC oldhdc = wglGetCurrentDC();
        wglMakeCurrent(NULL, NULL);
        
        if (!wglShareLists(mGLRc, newCtx))
        {
            DiString errorMsg = DiWin32GLUtil::TranslateWGLError();
            wglDeleteContext(newCtx);
            DI_WARNING("Error calling wglShareLists : %s", errorMsg.c_str());
            return nullptr;
        }

        wglMakeCurrent(oldhdc, oldrc);
        return DI_NEW DiWin32GLContext(mGLUtil, mHDC, newCtx);
    }

    void DiWin32GLContext::InitFromHwnd(HWND hwnd)
    {
        HDC old_hdc = wglGetCurrentDC();
        HGLRC old_context = wglGetCurrentContext();

        mHDC = GetDC(hwnd);

        // select valid pixel format
        // todo
        bool formatOk = mGLUtil->SelectPixelFormat(mHDC, 32, 0, false); // TODO!
        if (!formatOk)
        {
            DI_ERROR("Invalid pixel format!");
            return;
        }

        mGLRc = wglCreateContext(mHDC);
        if (!mGLRc)
        {
            DI_LOG(DiWin32GLUtil::TranslateWGLError().c_str());
            DI_ERROR("Cannot create wgl context.");
            return;
        }

        if (old_context && old_context != mGLRc)
        {
            // Share lists with old context
            if (!wglShareLists(old_context, mGLRc))
            {
                DI_ERROR("Cannot share wgl lists");
            }
        }

        if (!wglMakeCurrent(mHDC, mGLRc))
        {
            DI_ERROR("Failed to call wglMakeCurrent");
        }

        PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

        // TODO: VSYNC
        //if (_wglSwapIntervalEXT)
            //_wglSwapIntervalEXT(mVSync ? mVSyncInterval : 0);

        if (old_context && old_context != mGLRc)
        {
            // Restore old context
            if (!wglMakeCurrent(mHDC, mGLRc))
            {
                DI_ERROR("Failed to call wglMakeCurrent");
            }
        }
    }

    void DiWin32GLContext::SwapBuffer()
    {
        SwapBuffers(mHDC);
    }

}