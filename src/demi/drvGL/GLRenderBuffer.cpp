
#include "DrvGLPch.h"
#include "GLRenderBuffer.h"
#include "GLDriver.h"
#include "GLRenderTarget.h"
#include "GLTypeMappings.h"

namespace Demi
{
    DiGLRenderBuffer::DiGLRenderBuffer(GLenum format, uint32 width, uint32 height)
        :mRenderbufferID(0),
        mGLFormat(format)
    {
        glGenRenderbuffersEXT(1, &mRenderbufferID);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderbufferID);
    }

    DiGLRenderBuffer::~DiGLRenderBuffer()
    {
        glDeleteRenderbuffersEXT(1, &mRenderbufferID);
    }

    void DiGLRenderBuffer::BindToFramebuffer(GLenum attachment)
    {
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment,
            GL_RENDERBUFFER_EXT, mRenderbufferID);
    }

}