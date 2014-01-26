
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLRenderTarget.h"
#include "GLTypeMappings.h"
#include "GLRenderBuffer.h"

namespace Demi
{

    DiGLDepthBuffer::DiGLDepthBuffer(uint16 poolId, uint32 width, uint32 height, 
        DiGLContext* context, DiGLRenderBuffer* depth,
        DiGLRenderBuffer* stencil, uint32 fsaa, uint32 multiSampleQuality, bool manual)
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
            case GL_DEPTH_COMPONENT24:
            case GL_DEPTH_COMPONENT32:
            case GL_DEPTH24_STENCIL8_EXT:
                mBitDepth = 32;
                break;
            }
        }
    }

    DiGLDepthBuffer::~DiGLDepthBuffer()
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