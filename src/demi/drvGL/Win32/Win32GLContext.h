
/********************************************************************
    File:       Win32GLContext.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GLContext.h"

namespace Demi
{
    class DI_GLDRV_API DiWin32GLContext : public DiGLContext
    {
    public:

        DiWin32GLContext(DiWin32GLUtil* util, DiWndHandle wnd);

        DiWin32GLContext(DiWin32GLUtil* util, HDC dc, HGLRC glrc);

        ~DiWin32GLContext();

    public:

        void            BeginContext();

        void            EndContext();

        void            Release();

        DiGLContext*    Clone() const;

        void            SwapBuffer();

    private:

        void            InitFromHwnd(HWND hwnd);

    private:

        DiWin32GLUtil*  mGLUtil;

        HDC             mHDC;

        HGLRC           mGLRc;
    };
}