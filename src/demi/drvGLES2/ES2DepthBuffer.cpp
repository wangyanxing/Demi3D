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
#include "ES2DepthBuffer.h"
#include "GLES2Driver.h"
#include "ES2RenderTarget.h"
#include "ES2TypeMappings.h"
#include "ES2RenderBuffer.h"

namespace Demi
{

    DiGLES2DepthBuffer::DiGLES2DepthBuffer(uint16 poolId, uint32 width, uint32 height, 
        DiGLES2Context* context, DiGLES2RenderBuffer* depth,
        DiGLES2RenderBuffer* stencil, uint32 fsaa, uint32 multiSampleQuality, bool manual)
        : mDepthBuffer(depth)
        , mStencilBuffer(stencil)
        , mContext(context)
    {
        mPoolId = poolId;
        mBitDepth = 0;
        mWidth = width;
        mHeight = height;
        mManual = manual;
        mMultiSampleQuality = multiSampleQuality;

        if (mDepthBuffer)
        {
            switch (mDepthBuffer->GetGLFormat())
            {
            case GL_DEPTH_COMPONENT16:
                mBitDepth = 16;
                break;
            case GL_DEPTH_COMPONENT24_OES:
            case GL_DEPTH_COMPONENT32_OES:
            case GL_DEPTH24_STENCIL8_OES:  // Packed depth / stencil
                mBitDepth = 32;
                break;
            }
        }
    }

    DiGLES2DepthBuffer::~DiGLES2DepthBuffer()
    {
        if (mStencilBuffer && mStencilBuffer != mDepthBuffer)
        {
            DI_DELETE mStencilBuffer;
            mStencilBuffer = nullptr;
        }

        if (mDepthBuffer)
        {
            DI_DELETE mDepthBuffer;
            mDepthBuffer = nullptr;
        }
    }


}