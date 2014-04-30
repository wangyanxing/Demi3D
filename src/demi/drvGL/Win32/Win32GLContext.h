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

        void            SetVSyncEnabled(bool vsync);

        DiGLContext*    Clone() const;

    private:

        void            InitFromHwnd(HWND hwnd);

    private:

        DiWin32GLUtil*  mGLUtil;

        HDC             mHDC;

        HGLRC           mGLRc;

        bool            mVSync;
    };
}

#endif
