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

#ifndef DiEGLContext_h__
#define DiEGLContext_h__

#include "ES2Context.h"

namespace Demi
{
    class DI_GLES2_API DiEGLContext : public DiGLES2Context
    {
    public:

        DiEGLContext(EGLDisplay eglDisplay, DiEGLUtil* util, ::EGLConfig fbconfig, ::EGLSurface drawable);

        ~DiEGLContext();

    public:

        void            BeginContext();

        void            EndContext();

        virtual DiGLES2Context* Clone() const = 0;

        virtual void    CreateInternalResources(EGLDisplay eglDisplay, ::EGLConfig glconfig, ::EGLSurface drawable, ::EGLContext shareContext);

        virtual void    DestroyInternalResources();

        EGLSurface      getDrawable() const { return mDrawable; }

    private:

        DiEGLUtil*      mGLUtil;

        ::EGLConfig     mConfig;

        ::EGLSurface    mDrawable;

        ::EGLContext    mContext;

        EGLDisplay      mEglDisplay;
    };
}

#endif
