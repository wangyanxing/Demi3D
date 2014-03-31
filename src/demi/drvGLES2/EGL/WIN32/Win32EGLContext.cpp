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
#include "Win32EGLContext.h"
#include "Win32EGLUtil.h"

namespace Demi
{
    DiWin32EGLContext::DiWin32EGLContext(EGLDisplay eglDisplay,
        DiEGLUtil* util, ::EGLConfig fbconfig, ::EGLSurface drawable)
        : DiEGLContext(eglDisplay, util, fbconfig, drawable)
    {

    }

    DiWin32EGLContext::~DiWin32EGLContext()
    {

    }

    DiGLES2Context* DiWin32EGLContext::Clone() const
    {
        return DI_NEW DiWin32EGLContext(mEglDisplay, mGLUtil, mConfig, mDrawable);
    }

    void DiWin32EGLContext::Release()
    {

    }

}