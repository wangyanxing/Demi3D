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
    class DI_GLES2_API DiEGLContext : public DiGLContext
    {
    public:

        DiEGLContext(DiEGLUtil* util, DiWndHandle wnd);

        DiEGLContext(DiEGLUtil* util, HDC dc, HGLRC glrc);

        ~DiEGLContext();

    public:

        void            BeginContext();

        void            EndContext();

        void            Release();

        DiGLContext*    Clone() const;

    private:

        void            InitFromHwnd(HWND hwnd);

    private:

        DiEGLUtil*  mGLUtil;

        HDC             mHDC;

        HGLRC           mGLRc;
    };
}

#endif
