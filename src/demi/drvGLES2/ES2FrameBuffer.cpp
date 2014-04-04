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
#include "ES2FrameBuffer.h"
#include "ES2DepthBuffer.h"
#include "GLES2Driver.h"
#include "ES2RenderTarget.h"
#include "ES2Texture.h"
#include "ES2DepthBuffer.h"
#include "ES2TypeMappings.h"
#include "ES2RenderBuffer.h"
#include "ES2Util.h"
#include <sstream>

#include "Image.h"

// Size of probe texture
#define PROBE_SIZE 16

// Stencil and depth formats to be tried
static const GLenum stencilFormats[] =
{
    GL_NONE,                    // No stencil
    GL_STENCIL_INDEX1_OES,
    GL_STENCIL_INDEX4_OES,
    GL_STENCIL_INDEX8
};
static const size_t stencilBits[] =
{
    0,
    1,
    4,
    8
};
#define STENCILFORMAT_COUNT (sizeof(stencilFormats)/sizeof(GLenum))

static const GLenum depthFormats[] =
{
    GL_NONE,
    GL_DEPTH_COMPONENT16
    , GL_DEPTH_COMPONENT24_OES   // Prefer 24 bit depth
    , GL_DEPTH_COMPONENT32_OES
    , GL_DEPTH24_STENCIL8_OES    // Packed depth / stencil
};
static const size_t depthBits[] =
{
    0
    , 16
    , 24
    , 32
    , 24
};
#define DEPTHFORMAT_COUNT (sizeof(depthFormats)/sizeof(GLenum))

namespace Demi
{
    DiGLES2FrameBuffer::DiGLES2FrameBuffer()
        :mFBOId(0)
    {
        CHECK_GL_ERROR(glGenFramebuffers(1, &mFBOId));

        // TODO debug label

        // TODO MSAA
    }

    DiGLES2FrameBuffer::~DiGLES2FrameBuffer()
    {
        CHECK_GL_ERROR(glDeleteFramebuffers(1, &mFBOId));
    }

    void DiGLES2FrameBuffer::AttachSurface(uint32 attachment, DiTexturePtr surface)
    {
        DI_ASSERT(attachment < MAX_MRT);
        mColorBuffer[attachment] = surface;
        if (mColorBuffer[attachment])
            Init();
    }

    void DiGLES2FrameBuffer::DetarchSurface(uint32 attachment)
    {
        DI_ASSERT(attachment < MAX_MRT);
        mColorBuffer[attachment].reset();
        if (mColorBuffer[attachment])
            Init();
    }

    void DiGLES2FrameBuffer::Init()
    {
        size_t width = mColorBuffer[0]->GetWidth();
        size_t height = mColorBuffer[0]->GetHeight();
        DiPixelFormat origFmt = mColorBuffer[0]->GetFormat();
        uint32 maxSupportedMRTs = MAX_MRT;  //TODO

        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFBOId));

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
                
                DiGLES2TextureDrv* td = static_cast<DiGLES2TextureDrv*>(mColorBuffer[x]->GetTextureDriver());
                
                if(GetFormat() == PF_DEPTH)
                    td->BindToFrameBuffer(GL_DEPTH_ATTACHMENT,0,0);
                else
                    td->BindToFrameBuffer(GL_COLOR_ATTACHMENT0+x,0,0);
            }
            else
            {
                CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + x,
                    GL_RENDERBUFFER, 0));
            }
        }

        GLenum bufs[MAX_MRT];
        GLsizei n = 0;
        for (uint32 x = 0; x < MAX_MRT; ++x)
        {
            if (mColorBuffer[x])
            {
                if (GetFormat() == PF_DEPTH)
                    bufs[x] = GL_DEPTH_ATTACHMENT;
                else
                    bufs[x] = GL_COLOR_ATTACHMENT0 + x;
                // Keep highest used buffer + 1
                n = x + 1;
            }
            else
            {
                bufs[x] = GL_NONE;
            }
        }

        // Check status
        GLuint status;
        CHECK_GL_ERROR(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));

        // Bind main buffer
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        // The screen buffer is 1 on iOS
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 1));
#else
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#endif

        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE:
            // All is good
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            DI_WARNING("All framebuffer formats with this texture internal format unsupported");
        default:
            DI_WARNING("Framebuffer incomplete or other FBO status error");
        }
    }

    void DiGLES2FrameBuffer::Bind()
    {
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFBOId));
    }

    void DiGLES2FrameBuffer::SwapBuffers()
    {
        // for MSAA FBO
    }

    void DiGLES2FrameBuffer::AttachDepthBuffer(DiDepthBuffer* depthBuffer)
    {
        DiGLES2DepthBuffer *glDepthBuffer = static_cast<DiGLES2DepthBuffer*>(depthBuffer);

        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFBOId));

        if (glDepthBuffer)
        {
            DiGLES2RenderBuffer *depthBuf = glDepthBuffer->GetDepthBuffer();
            DiGLES2RenderBuffer *stencilBuf = glDepthBuffer->GetStencilBuffer();

            // Attach depth buffer, if it has one.
            if (depthBuf)
                depthBuf->BindToFramebuffer(GL_DEPTH_ATTACHMENT);

            // Attach stencil buffer, if it has one.
            if (stencilBuf)
                stencilBuf->BindToFramebuffer(GL_STENCIL_ATTACHMENT);
        }
        else
        {
            CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER, 0));
            CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER, 0));
        }
    }

    void DiGLES2FrameBuffer::DetachDepthBuffer()
    {
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFBOId));
        CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
        CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0));
    }

    DiPixelFormat DiGLES2FrameBuffer::GetFormat()
    {
        DI_ASSERT(mColorBuffer[0]);
        return mColorBuffer[0]->GetFormat();
    }

    DiGLES2FBOManager::DiGLES2FBOManager(bool atimode)
    {
        DI_ASSERT(!DiGLES2Driver::FBOManager);
        DiGLES2Driver::FBOManager = this;

        DetectFBOFormats();

        CHECK_GL_ERROR(glGenFramebuffers(1, &mTempFBO));
    }


    DiGLES2FBOManager::~DiGLES2FBOManager()
    {
        CHECK_GL_ERROR(glDeleteFramebuffers(1, &mTempFBO));
    }

    void DiGLES2FBOManager::DetectFBOFormats()
    {
        // Try all formats, and report which ones work as target
        GLuint fb = 0, tid = 0;

        for (uint32 x = 0; x < PIXEL_FORMAT_MAX; ++x)
        {
            mProps[x].valid = false;

            // Fetch GL format token
            GLint internalFormat = DiGLTypeMappings::GLInternalFormatMapping[(DiPixelFormat)x];
            GLenum fmt = DiGLTypeMappings::GLFormatMapping[(DiPixelFormat)x];
            GLenum type = DiGLTypeMappings::GetDataType((DiPixelFormat)x);

#if DEMI_PLATFORM == DEMI_PLATFORM_ANDROID
            if (internalFormat == GL_NONE || fmt == GL_NONE || type == GL_NONE)
                continue;
#else
            if ((internalFormat == GL_NONE || fmt == GL_NONE || type == GL_NONE) && (x != 0))
                continue;
#endif

            if (DiPixelBox::IsCompressedFormat((DiPixelFormat)x))
                continue;

            // Create and attach framebuffer
            CreateTempFramebuffer((DiPixelFormat)x, internalFormat, fmt, type, fb, tid);

            // Ignore status in case of fmt==GL_NONE, because no implementation will accept
            // a buffer without *any* attachment. Buffers with only stencil and depth attachment
            // might still be supported, so we must continue probing.
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            {
                mProps[x].valid = true;

                DiString name = DiPixelBox::GetPixelTypeName((DiPixelFormat)x);
                DiString log = "FBO " + name + " depth/stencil support: ";
                std::stringstream str;
                str << "FBO " << name.c_str() << " depth/stencil support: ";

                // For each depth/stencil formats
                for (size_t depth = 0; depth < DEPTHFORMAT_COUNT; ++depth)
                {
                    if (DiGLES2Driver::GLUtil->CheckExtension("GL_OES_packed_depth_stencil") &&
                        depthFormats[depth] != GL_DEPTH24_STENCIL8_OES)
                    {
                        // General depth/stencil combination
                        for (size_t stencil = 0; stencil < STENCILFORMAT_COUNT; ++stencil)
                        {
                            if (TryFormat(depthFormats[depth], stencilFormats[stencil]))
                            {
                                // Add mode to allowed modes
                                str << "D" << depthBits[depth] << "S" << stencilBits[stencil] << " ";
                                FormatProperties::Mode mode;
                                mode.depth = depth;
                                mode.stencil = stencil;
                                mProps[x].modes.push_back(mode);
                            }
                            else
                            {
                                // There is a small edge case that FBO is trashed during the test
                                // on some drivers resulting in undefined behavior
                                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                                glDeleteFramebuffers(1, &fb);

                                CreateTempFramebuffer((DiPixelFormat)x, internalFormat, fmt, type, fb, tid);
                            }
                        }
                    }
                    else
                    {
                        // Packed depth/stencil format
                        if (TryPackedFormat(depthFormats[depth]))
                        {
                            // Add mode to allowed modes
                            str << "Packed-D" << depthBits[depth] << "S" << 8 << " ";
                            FormatProperties::Mode mode;
                            mode.depth = depth;
                            mode.stencil = 0;   // unuse
                            mProps[x].modes.push_back(mode);
                        }
                        else
                        {
                            // There is a small edge case that FBO is trashed during the test
                            // on some drivers resulting in undefined behavior
                            glBindFramebuffer(GL_FRAMEBUFFER, 0);
                            glDeleteFramebuffers(1, &fb);

                            CreateTempFramebuffer((DiPixelFormat)x, internalFormat, fmt, type, fb, tid);
                        }
                    }
                }
                DI_LOG(str.str().c_str());
            }

            // Delete texture and framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, &fb);

            if (internalFormat != GL_NONE)
            {
                glDeleteTextures(1, &tid);
                tid = 0;
            }
        }

        // Clear any errors
        glGetError();

        DiString fmtstring;
        for (size_t x = 0; x < PIXEL_FORMAT_MAX; ++x)
        {
            if (mProps[x].valid)
                fmtstring += DiPixelBox::GetPixelTypeName((DiPixelFormat)x) + " ";
        }
        DI_LOG("[GLES2] : Valid FBO targets %s", fmtstring.c_str());
    }

    GLuint DiGLES2FBOManager::TryFormat(GLenum depthFormat, GLenum stencilFormat)
    {
        GLuint status, depthRB = 0, stencilRB = 0;

        if (depthFormat != GL_NONE)
        {
            // Generate depth renderbuffer
            glGenRenderbuffers(1, &depthRB);

            // Bind it to FBO
            glBindRenderbuffer(GL_RENDERBUFFER, depthRB);

            // Allocate storage for depth buffer
            glRenderbufferStorage(GL_RENDERBUFFER, depthFormat,
                PROBE_SIZE, PROBE_SIZE);

            // Attach depth
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRB);
        }

        // Stencil buffers aren't available on iOS
        if (stencilFormat != GL_NONE)
        {
            // Generate stencil renderbuffer
            glGenRenderbuffers(1, &stencilRB);

            // Bind it to FBO
            glBindRenderbuffer(GL_RENDERBUFFER, stencilRB);

            // Allocate storage for stencil buffer
            glRenderbufferStorage(GL_RENDERBUFFER, stencilFormat, PROBE_SIZE, PROBE_SIZE);

            // Attach stencil
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER, stencilRB);
        }

        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        // If status is negative, clean up
        // Detach and destroy
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);

        if (depthRB)
            glDeleteRenderbuffers(1, &depthRB);

        if (stencilRB)
            glDeleteRenderbuffers(1, &stencilRB);

        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    bool DiGLES2FBOManager::TryPackedFormat(GLenum packedFormat)
    {
        GLuint packedRB;

        // Generate renderbuffer
        glGenRenderbuffers(1, &packedRB);

        // Bind it to FBO
        glBindRenderbuffer(GL_RENDERBUFFER, packedRB);

        // Allocate storage for buffer
        glRenderbufferStorage(GL_RENDERBUFFER, packedFormat, PROBE_SIZE, PROBE_SIZE);

        // Attach depth
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, packedRB);

        // Attach stencil
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, packedRB);

        GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        // Detach and destroy
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &packedRB);

        return status == GL_FRAMEBUFFER_COMPLETE;
    }

    void DiGLES2FBOManager::GetBestDepthStencil(DiPixelFormat internalFormat, GLenum *depthFormat, GLenum *stencilFormat)
    {
        const FormatProperties &props = mProps[internalFormat];
        // Decide what stencil and depth formats to use
        // [best supported for internal format]
        size_t bestmode = 0;
        int bestscore = -1;
        bool requestDepthOnly = internalFormat == PF_DEPTH;
        for (size_t mode = 0; mode < props.modes.size(); mode++)
        {
            int desirability = 0;
            // Find most desirable mode
            // desirability == 0            if no depth, no stencil
            // desirability == 1000...2000  if no depth, stencil
            // desirability == 2000...3000  if depth, no stencil
            // desirability == 3000+        if depth and stencil
            // beyond this, the total number of bits (stencil+depth) is maximised
            if (props.modes[mode].stencil && !requestDepthOnly)
                desirability += 1000;
            if (props.modes[mode].depth)
                desirability += 2000;
            if (depthBits[props.modes[mode].depth] == 24) // Prefer 24 bit for now
                desirability += 500;
            if (DiGLES2Driver::GLUtil->CheckExtension("GL_OES_packed_depth_stencil") || gleswIsSupported(3, 0))
            if (depthFormats[props.modes[mode].depth] == GL_DEPTH24_STENCIL8_OES) // Prefer 24/8 packed
                desirability += 5000;

            desirability += stencilBits[props.modes[mode].stencil] + depthBits[props.modes[mode].depth];

            if (desirability > bestscore)
            {
                bestscore = desirability;
                bestmode = mode;
            }
        }
        *depthFormat = depthFormats[props.modes[bestmode].depth];
        if (requestDepthOnly)
            *stencilFormat = 0;
        else
            *stencilFormat = stencilFormats[props.modes[bestmode].stencil];
    }

    DiPixelFormat DiGLES2FBOManager::GetSupportedAlternative(DiPixelFormat format)
    {
        if (CheckFormat(format))
        {
            return format;
        }

        /// Find first alternative
        PixelComponentType pct = DiPixelBox::GetComponentType(format);

        switch (pct)
        {
        case PCT_BYTE:
            format = PF_A8R8G8B8;
            break;
        case PCT_FLOAT16:
            format = PF_A16B16G16R16F;
            break;
        case PCT_FLOAT32:
            format = PF_A32B32G32R32F;
            break;
        case PCT_COUNT:
        default:
            break;
        }

        if (CheckFormat(format))
            return format;

        /// If none at all, return to default
        return PF_A8R8G8B8;
    }

    void DiGLES2FBOManager::CreateTempFramebuffer(DiPixelFormat pixFmt,
        GLuint internalFormat, GLuint fmt, GLenum dataType, GLuint &fb, GLuint &tid)
    {
        // Create and attach framebuffer
        glGenFramebuffers(1, &fb);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        if (internalFormat != GL_NONE)
        {
            if (tid)
                glDeleteTextures(1, &tid);

            // Create and attach texture
            glGenTextures(1, &tid);
            glBindTexture(GL_TEXTURE_2D, tid);

            // Set some default parameters
            DiGLES2Driver::StateCache->setTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            DiGLES2Driver::StateCache->setTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            DiGLES2Driver::StateCache->setTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            DiGLES2Driver::StateCache->setTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, PROBE_SIZE, PROBE_SIZE, 0, fmt, dataType, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, tid, 0);
        }
    }

}