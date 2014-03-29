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

#ifndef DiGLDepthBuffer_h__
#define DiGLDepthBuffer_h__

#include "DepthBuffer.h"

namespace Demi
{
    class DI_GLES2_API DiGLDepthBuffer : public DiDepthBuffer
    {
    public:

        DiGLDepthBuffer(uint16 poolId, uint32 width, uint32 height, DiGLContext* context, DiGLRenderBuffer* depth,
            DiGLRenderBuffer* stencil, uint32 fsaa, uint32 multiSampleQuality, bool manual);

        ~DiGLDepthBuffer();

    public:

        DiGLRenderBuffer*   GetDepthBuffer() { return mDepthBuffer; }

        DiGLRenderBuffer*   GetStencilBuffer() { return mStencilBuffer; }

        DiGLContext*        GetGLContext() { return mContext; }

    private:

        DiGLRenderBuffer*   mDepthBuffer;

        DiGLRenderBuffer*   mStencilBuffer;

        DiGLContext*        mContext;
    };
}

#endif
