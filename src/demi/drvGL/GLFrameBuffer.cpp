
#include "GLFrameBuffer.h"
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLRenderTarget.h"
#include "GLTexture.h"
#include "GLTypeMappings.h"

namespace Demi
{
    DiFrameBuffer::DiFrameBuffer()
        :mFBOId(0)
    {
        glGenFramebuffersEXT(1, &mFBOId);
    }

    DiFrameBuffer::~DiFrameBuffer()
    {
        glDeleteFramebuffersEXT(1, &mFBOId);
    }

    void DiFrameBuffer::AttachSurface(uint32 attachment, DiTexturePtr surface)
    {
        DI_ASSERT(attachment < MAX_MRT);
        mColorBuffer[attachment] = surface;
        if (mColorBuffer[attachment])
            Init();
    }

    void DiFrameBuffer::DetarchSurface(uint32 attachment)
    {
        DI_ASSERT(attachment < MAX_MRT);
        mColorBuffer[attachment].reset();
        if (mColorBuffer[attachment])
            Init();
    }

    void DiFrameBuffer::Init()
    {
        size_t width = mColorBuffer[0]->GetWidth();
        size_t height = mColorBuffer[0]->GetHeight();
        DiPixelFormat origFmt = mColorBuffer[0]->GetFormat();
        GLuint format = DiGLTypeMappings::GLFormatMapping[origFmt];
        uint32 maxSupportedMRTs = MAX_MRT;  //TODO

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOId);

        // Bind all attachment points to frame buffer
        for (size_t x = 0; x < maxSupportedMRTs; ++x)
        {
            if (mColorBuffer[x])
            {
                if (mColorBuffer[x]->GetWidth() != width || mColorBuffer[x]->GetHeight() != height)
                {
                    DI_WARNING("All color buffers in a frame buffer object should exactly in same dimension");
                    return;
                }
                if (mColorBuffer[x]->GetFormat() != origFmt)
                {
                    DI_WARNING("All color buffers in a frame buffer object should exactly in same format");
                    return;
                }
                DiGLTextureDrv* td = static_cast<DiGLTextureDrv*>(mColorBuffer[x]->GetTextureDriver());
                td->BindToFrameBuffer(GL_COLOR_ATTACHMENT0_EXT + x, 0, 0);
            }
            else
            {
                glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + x,
                    GL_RENDERBUFFER_EXT, 0);
            }
        }

        GLenum bufs[MAX_MRT];
        GLsizei n = 0;
        for (size_t x = 0; x < MAX_MRT; ++x)
        {
            if (mColorBuffer[x])
            {
                bufs[x] = GL_COLOR_ATTACHMENT0_EXT + x;
                // Keep highest used buffer + 1
                n = x + 1;
            }
            else
            {
                bufs[x] = GL_NONE;
            }
        }

        if (glDrawBuffers)
            glDrawBuffers(n, bufs);
        else
            glDrawBuffer(bufs[0]);
       
        glReadBuffer(GL_NONE);

        GLuint result = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        switch (result)
        {
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            DI_WARNING("Unsupported frame buffer format.");
            break;
        default:
            DI_WARNING("Cannot init frame buffer object.");
        }
    }

}