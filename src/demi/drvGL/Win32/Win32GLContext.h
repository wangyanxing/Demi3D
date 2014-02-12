


#ifndef DiWin32GLContext_h__
#define DiWin32GLContext_h__


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

    private:

        void            InitFromHwnd(HWND hwnd);

    private:

        DiWin32GLUtil*  mGLUtil;

        HDC             mHDC;

        HGLRC           mGLRc;
    };
}

#endif
