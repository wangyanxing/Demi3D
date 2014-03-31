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

#ifndef DiWin32EGLContext_h__
#define DiWin32EGLContext_h__

#include "EGLContext.h"

namespace Demi
{
    class DI_GLES2_API DiWin32EGLContext : public DiEGLContext
    {
    public:

        DiWin32EGLContext(EGLDisplay eglDisplay, DiEGLUtil* util,
            ::EGLConfig fbconfig, ::EGLSurface drawable);

        ~DiWin32EGLContext();

    public:

        void                Release();

        DiGLES2Context*     Clone() const;

    };
}

#endif
