
#include "DrvGLPch.h"
#include "GLFrameBuffer.h"
#include "GLDepthBuffer.h"
#include "GLDriver.h"
#include "GLRenderTarget.h"
#include "GLTexture.h"
#include "GLDepthBuffer.h"
#include "GLTypeMappings.h"
#include "GLRenderBuffer.h"

#include "Image.h"

/// Size of probe texture
#define PROBE_SIZE 16

/// Stencil and depth formats to be tried
static const GLenum stencilFormats[] =
{
    GL_NONE,                    // No stencil
    GL_STENCIL_INDEX1_EXT,
    GL_STENCIL_INDEX4_EXT,
    GL_STENCIL_INDEX8_EXT,
    GL_STENCIL_INDEX16_EXT
};
static const size_t stencilBits[] =
{
    0, 1, 4, 8, 16
};
#define STENCILFORMAT_COUNT (sizeof(stencilFormats)/sizeof(GLenum))

static const GLenum depthFormats[] =
{
    GL_NONE,
    GL_DEPTH_COMPONENT16,
    GL_DEPTH_COMPONENT24,    // Prefer 24 bit depth
    GL_DEPTH_COMPONENT32,
    GL_DEPTH24_STENCIL8_EXT // packed depth / stencil
};
static const size_t depthBits[] =
{
    0, 16, 24, 32, 24
};
#define DEPTHFORMAT_COUNT (sizeof(depthFormats)/sizeof(GLenum))


namespace Demi
{
    DiGLFrameBuffer::DiGLFrameBuffer()
        :mFBOId(0)
    {
        glGenFramebuffersEXT(1, &mFBOId);
    }

    DiGLFrameBuffer::~DiGLFrameBuffer()
    {
        glDeleteFramebuffersEXT(1, &mFBOId);
    }

    void DiGLFrameBuffer::AttachSurface(uint32 attachment, DiTexturePtr surface)
    {
        DI_ASSERT(attachment < MAX_MRT);
        mColorBuffer[attachment] = surface;
        if (mColorBuffer[attachment])
            Init();
    }

    void DiGLFrameBuffer::DetarchSurface(uint32 attachment)
    {
        DI_ASSERT(attachment < MAX_MRT);
        mColorBuffer[attachment].reset();
        if (mColorBuffer[attachment])
            Init();
    }

    void DiGLFrameBuffer::Init()
    {
        size_t width = mColorBuffer[0]->GetWidth();
        size_t height = mColorBuffer[0]->GetHeight();
        DiPixelFormat origFmt = mColorBuffer[0]->GetFormat();
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
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            break;
        default:
            DI_WARNING("Cannot init frame buffer object.");
        }
    }

    void DiGLFrameBuffer::Bind()
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOId);
    }

    void DiGLFrameBuffer::SwapBuffers()
    {
        // for MSAA FBO
    }

    void DiGLFrameBuffer::AttachDepthBuffer(DiDepthBuffer* depthBuffer)
    {
        DiGLDepthBuffer *glDepthBuffer = static_cast<DiGLDepthBuffer*>(depthBuffer);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOId);

        if (glDepthBuffer)
        {
            DiGLRenderBuffer *depthBuf = glDepthBuffer->GetDepthBuffer();
            DiGLRenderBuffer *stencilBuf = glDepthBuffer->GetStencilBuffer();

            // Attach depth buffer, if it has one.
            if (depthBuf)
                depthBuf->BindToFramebuffer(GL_DEPTH_ATTACHMENT_EXT);

            // Attach stencil buffer, if it has one.
            if (stencilBuf)
                stencilBuf->BindToFramebuffer(GL_STENCIL_ATTACHMENT_EXT);
        }
        else
        {
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                GL_RENDERBUFFER_EXT, 0);
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
                GL_RENDERBUFFER_EXT, 0);
        }
    }

    void DiGLFrameBuffer::DetachDepthBuffer()
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOId);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
            GL_RENDERBUFFER_EXT, 0);
    }

    DiPixelFormat DiGLFrameBuffer::GetFormat()
    {
        DI_ASSERT(mColorBuffer[0]);
        return mColorBuffer[0]->GetFormat();
    }

    DiGLFBOManager::DiGLFBOManager(bool atimode)
        :mATIMode(atimode)
    {
        DI_ASSERT(!DiGLDriver::FBOManager);
        DiGLDriver::FBOManager = this;

        DetectFBOFormats();

        glGenFramebuffersEXT(1, &mTempFBO);
    }


    DiGLFBOManager::~DiGLFBOManager()
    {
        glDeleteFramebuffersEXT(1, &mTempFBO);
    }

    void DiGLFBOManager::DetectFBOFormats()
    {
        // Try all formats, and report which ones work as target
        GLuint fb = 0, tid = 0;
        GLint old_drawbuffer = 0, old_readbuffer = 0;
        GLenum target = GL_TEXTURE_2D;

        glGetIntegerv(GL_DRAW_BUFFER, &old_drawbuffer);
        glGetIntegerv(GL_READ_BUFFER, &old_readbuffer);

        for (uint32 x = 0; x < PIXEL_FORMAT_MAX; ++x)
        {
            mProps[x].valid = false;

            GLenum fmt = DiGLTypeMappings::GLFormatMapping[(DiPixelFormat)x];
            if (fmt == GL_NONE && x != 0)
                continue;

            if (DiPixelBox::IsCompressedFormat((DiPixelFormat)x))
                continue;

            // Buggy ATI cards *crash* on non-RGB(A) formats
            int depths[4];
            DiPixelBox::GetBitDepths((DiPixelFormat)x, depths);
            if (fmt != GL_NONE && mATIMode && (!depths[0] || !depths[1] || !depths[2]))
                continue;

            // Create and attach framebuffer
            glGenFramebuffersEXT(1, &fb);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
            if (fmt != GL_NONE)
            {
                // Create and attach texture
                glGenTextures(1, &tid);
                glBindTexture(target, tid);

                // Set some default parameters so it won't fail on NVidia cards         
                if (GLEW_VERSION_1_2)
                    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexImage2D(target, 0, fmt, PROBE_SIZE, PROBE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                    target, tid, 0);
            }
            else
            {
                // Draw to nowhere -- stencil/depth only
                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
            }

            // Check status
            GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

            // Ignore status in case of fmt==GL_NONE, because no implementation will accept
            // a buffer without *any* attachment. Buffers with only stencil and depth attachment
            // might still be supported, so we must continue probing.
            if (fmt == GL_NONE || status == GL_FRAMEBUFFER_COMPLETE_EXT)
            {
                mProps[x].valid = true;

                // For each depth/stencil formats
                for (size_t depth = 0; depth < DEPTHFORMAT_COUNT; ++depth)
                {
                    if (depthFormats[depth] != GL_DEPTH24_STENCIL8_EXT)
                    {
                        // General depth/stencil combination

                        for (size_t stencil = 0; stencil < STENCILFORMAT_COUNT; ++stencil)
                        {
                            if (TryFormat(depthFormats[depth], stencilFormats[stencil]))
                            {
                                /// Add mode to allowed modes
                                FormatProperties::Mode mode;
                                mode.depth = depth;
                                mode.stencil = stencil;
                                mProps[x].modes.push_back(mode);
                            }
                        }
                    }
                    else
                    {

                        if (TryPackedFormat(depthFormats[depth]))
                        {
                            /// Add mode to allowed modes
                            FormatProperties::Mode mode;
                            mode.depth = depth;
                            mode.stencil = 0;
                            mProps[x].modes.push_back(mode);
                        }
                    }
                }
            }
            // Delete texture and framebuffer
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            glDeleteFramebuffersEXT(1, &fb);

            // Workaround for NVIDIA / Linux 169.21 driver problem
            // see http://www.ogre3d.org/phpBB2/viewtopic.php?t=38037&start=25
            glFinish();

            if (fmt != GL_NONE)
                glDeleteTextures(1, &tid);
        }

        // It seems a bug in nVidia driver: glBindFramebufferEXT should restore
        // draw and read buffers, but in some unclear circumstances it won't.
        glDrawBuffer(old_drawbuffer);
        glReadBuffer(old_readbuffer);

        for (size_t x = 0; x < PIXEL_FORMAT_MAX; ++x)
        {
            if (mProps[x].valid)
            {
                char name[256];
                DiPixelBox::FormatGetDisplayStr((DiPixelFormat)x, name, 256);
                DI_LOG("Valid format for FBO targets: %s", name);
            }
        }
    }

    GLuint DiGLFBOManager::TryFormat(GLenum depthFormat, GLenum stencilFormat)
    {
        GLuint status, depthRB = 0, stencilRB = 0;
        bool failed = false; // flag on GL errors

        if (depthFormat != GL_NONE)
        {
            /// Generate depth renderbuffer
            glGenRenderbuffersEXT(1, &depthRB);
            /// Bind it to FBO
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRB);

            /// Allocate storage for depth buffer
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthFormat,
                PROBE_SIZE, PROBE_SIZE);

            /// Attach depth
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                GL_RENDERBUFFER_EXT, depthRB);
        }

        if (stencilFormat != GL_NONE)
        {
            /// Generate stencil renderbuffer
            glGenRenderbuffersEXT(1, &stencilRB);
            /// Bind it to FBO
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencilRB);
            glGetError(); // NV hack
            /// Allocate storage for stencil buffer
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, stencilFormat,
                PROBE_SIZE, PROBE_SIZE);
            if (glGetError() != GL_NO_ERROR) // NV hack
                failed = true;
            /// Attach stencil
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
                GL_RENDERBUFFER_EXT, stencilRB);
            if (glGetError() != GL_NO_ERROR) // NV hack
                failed = true;
        }

        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        /// If status is negative, clean up
        // Detach and destroy
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
        if (depthRB)
            glDeleteRenderbuffersEXT(1, &depthRB);
        if (stencilRB)
            glDeleteRenderbuffersEXT(1, &stencilRB);

        return status == GL_FRAMEBUFFER_COMPLETE_EXT && !failed;
    }

    bool DiGLFBOManager::TryPackedFormat(GLenum packedFormat)
    {
        GLuint packedRB = 0;
        bool failed = false; // flag on GL errors

        /// Generate renderbuffer
        glGenRenderbuffersEXT(1, &packedRB);

        /// Bind it to FBO
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, packedRB);

        /// Allocate storage for buffer
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, packedFormat, PROBE_SIZE, PROBE_SIZE);
        glGetError(); // NV hack

        /// Attach depth
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
            GL_RENDERBUFFER_EXT, packedRB);
        if (glGetError() != GL_NO_ERROR) // NV hack
            failed = true;

        /// Attach stencil
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
            GL_RENDERBUFFER_EXT, packedRB);
        if (glGetError() != GL_NO_ERROR) // NV hack
            failed = true;

        GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

        /// Detach and destroy
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
        glDeleteRenderbuffersEXT(1, &packedRB);

        return status == GL_FRAMEBUFFER_COMPLETE_EXT && !failed;
    }

    void DiGLFBOManager::GetBestDepthStencil(GLenum internalFormat, GLenum *depthFormat, GLenum *stencilFormat)
    {
        const FormatProperties &props = mProps[internalFormat];
        /// Decide what stencil and depth formats to use
        /// [best supported for internal format]
        size_t bestmode = 0;
        int bestscore = -1;
        for (size_t mode = 0; mode < props.modes.size(); mode++)
        {
            int desirability = 0;
            /// Find most desirable mode
            /// desirability == 0            if no depth, no stencil
            /// desirability == 1000...2000  if no depth, stencil
            /// desirability == 2000...3000  if depth, no stencil
            /// desirability == 3000+        if depth and stencil
            /// beyond this, the total numer of bits (stencil+depth) is maximised
            if (props.modes[mode].stencil)
                desirability += 1000;
            if (props.modes[mode].depth)
                desirability += 2000;
            if (depthBits[props.modes[mode].depth] == 24) // Prefer 24 bit for now
                desirability += 500;
            if (depthFormats[props.modes[mode].depth] == GL_DEPTH24_STENCIL8_EXT) // Prefer 24/8 packed 
                desirability += 5000;
            desirability += stencilBits[props.modes[mode].stencil] + depthBits[props.modes[mode].depth];

            if (desirability > bestscore)
            {
                bestscore = desirability;
                bestmode = mode;
            }
        }
        *depthFormat = depthFormats[props.modes[bestmode].depth];
        *stencilFormat = stencilFormats[props.modes[bestmode].stencil];
    }

}