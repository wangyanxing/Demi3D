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

#ifndef DiGLES2RenderTarget_h__
#define DiGLES2RenderTarget_h__

#include "ES2Prerequisites.h"
#include "RenderTarget.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2RenderTarget : public DiRenderTarget
    {
    public:

        DiGLES2RenderTarget();

        virtual                 ~DiGLES2RenderTarget();

    public:

        void                    AttachSurface();

        void                    DetachSurface();

        bool                    BindRenderTarget(uint8 mrtid);

        void                    DetachDepthBuffer();

        bool                    AttachDepthBuffer(DiDepthBuffer *depthBuffer);

        bool                    BindDepthStencil();

        bool                    IsCompatibleWith(DiDepthBuffer* db);

        DiDepthBuffer*          CreateDepthBuffer();

        void                    PreBind();

        virtual DiGLES2Context* GetContext() { return nullptr; }

        virtual void            Init();

    protected:

        GLenum                  mGLFormat;

        DiGLES2FrameBuffer*     mFrameBuffer;
    };


    class DI_GLES2_API DiGLWindowTarget : public DiGLES2RenderTarget
    {
    public:

        DiGLWindowTarget();

        ~DiGLWindowTarget();

    public:

        void                    Create(DiWndHandle wnd, DiGLES2Context* context);

        DiGLES2Context*         GetContext() { return mContext; }

        bool                    SwapBuffer();

        void                    Init();

    private:

        DiWndHandle             mWnd;

        DiGLES2Context*         mContext;
    };
}

#endif
