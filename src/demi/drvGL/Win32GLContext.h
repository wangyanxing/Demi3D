
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

        DiWin32GLContext(HDC hdc, HGLRC glrc);

        ~DiWin32GLContext();

    public:

        void            BeginContext();

        void            EndContext();

        void            Release();

        DiGLContext*    Clone() const;

    private:

        HDC             mHDC;

        HGLRC           mGLRc;
    };
}