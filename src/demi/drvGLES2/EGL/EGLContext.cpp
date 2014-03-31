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
#include "EGLContext.h"
#include "EGLUtil.h"
#include "GLES2Driver.h"

namespace Demi
{
    DiEGLContext::DiEGLContext(EGLDisplay eglDisplay, DiEGLUtil* util, ::EGLConfig glconfig, ::EGLSurface drawable)
        :mGLUtil(util),
        mContext(nullptr)
    {
        DI_ASSERT(drawable);

        DiEGLContext* mainContext = static_cast<DiEGLContext*>(static_cast<DiGLES2Driver*>(Driver)->GetMainContext());
        ::EGLContext shareContext = (::EGLContext) 0;

        if (mainContext)
            shareContext = mainContext->mContext;

        CreateInternalResources(eglDisplay, glconfig, drawable, shareContext);
    }

    DiEGLContext::~DiEGLContext()
    {
        DestroyInternalResources();

        static_cast<DiGLES2Driver*>(Driver);
    }

    void DiEGLContext::BeginContext()
    {
        EGLBoolean ret = eglMakeCurrent(mEglDisplay,
            mDrawable, mDrawable, mContext);

        EGL_CHECK_ERROR
     
        if (!ret)
            DI_WARNING("Fail to make context current");
    }

    void DiEGLContext::EndContext()
    {
        eglMakeCurrent(mEglDisplay, 0, 0, 0);
        EGL_CHECK_ERROR
    }

    void DiEGLContext::CreateInternalResources(EGLDisplay eglDisplay, ::EGLConfig glconfig, ::EGLSurface drawable, ::EGLContext shareContext)
    {
        mDrawable = drawable;
        mConfig = glconfig;
        mEglDisplay = eglDisplay;

        mContext = mGLUtil->CreateNewContext(mEglDisplay, mConfig, shareContext);

        if (!mContext)
        {
            DI_WARNING("Unable to create a suitable EGLContext");
        }

        BeginContext();

        // Initialise GL3W
        if (gleswInit())
            DI_WARNING("Failed to initialize GL3W");
    }

    void DiEGLContext::DestroyInternalResources()
    {
        EndContext();

        eglDestroyContext(mEglDisplay, mContext);
        EGL_CHECK_ERROR

        mContext = NULL;
    }

}