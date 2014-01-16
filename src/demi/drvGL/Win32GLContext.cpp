
#include "Win32GLContext.h"
#include "Win32GLSupport.h"

namespace Demi
{
    DiWin32GLContext::DiWin32GLContext(HDC hdc, HGLRC glrc)
        :mHDC(hdc), mGLRc(glrc)
    {
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
            DiString errorMsg = DiWin32GLSupport::TranslateWGLError();
            wglDeleteContext(newCtx);
            DI_WARNING("Error calling wglShareLists : %s", errorMsg.c_str());
            return nullptr;
        }

        wglMakeCurrent(oldhdc, oldrc);
        return DI_NEW DiWin32GLContext(mHDC, newCtx);
    }

}