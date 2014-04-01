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

#ifndef DiGLES2DepthBuffer_h__
#define DiGLES2DepthBuffer_h__

#include "ES2Prerequisites.h"
#include "DepthBuffer.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2DepthBuffer : public DiDepthBuffer
    {
    public:

        DiGLES2DepthBuffer(uint16 poolId, uint32 width, uint32 height, 
            DiGLES2Context* context, DiGLES2RenderBuffer* depth,
            DiGLES2RenderBuffer* stencil, uint32 fsaa, uint32 multiSampleQuality, bool manual);

        ~DiGLES2DepthBuffer();

    public:

        DiGLES2RenderBuffer*   GetDepthBuffer() { return mDepthBuffer; }

        DiGLES2RenderBuffer*   GetStencilBuffer() { return mStencilBuffer; }

        DiGLES2Context*        GetGLContext() { return mContext; }

    private:

        DiGLES2RenderBuffer*   mDepthBuffer;

        DiGLES2RenderBuffer*   mStencilBuffer;

        DiGLES2Context*        mContext;
    };
}

#endif
