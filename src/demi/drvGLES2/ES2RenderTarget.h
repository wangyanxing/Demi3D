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

#ifndef DiGLRenderTarget_h__
#define DiGLRenderTarget_h__


#include "RenderTarget.h"

#define GL_DEPTH24_STENCIL8_EXT 0x88F0

namespace Demi
{
    class DI_GLES2_API DiGLRenderTarget : public DiRenderTarget
    {
    public:

        DiGLRenderTarget();

        virtual                 ~DiGLRenderTarget();

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

        virtual DiGLContext*    GetContext() { return nullptr; }

        virtual void            Init();

    protected:

        GLenum                  mGLFormat;

        DiGLFrameBuffer*        mFrameBuffer;
    };


    class DI_GLES2_API DiGLWindowTarget : public DiGLRenderTarget
    {
    public:

        DiGLWindowTarget();

        ~DiGLWindowTarget();

    public:

        void                    Create(DiWndHandle wnd, DiGLContext* context);

        DiGLContext*            GetContext() { return mContext; }

        bool                    SwapBuffer();

        void                    Init();

    private:

        DiWndHandle             mWnd;

        DiGLContext*            mContext;
    };
}

#endif
