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
#include "ES2RenderBuffer.h"
#include "GLES2Driver.h"
#include "ES2RenderTarget.h"
#include "ES2TypeMappings.h"

namespace Demi
{
    DiGLES2RenderBuffer::DiGLES2RenderBuffer(GLenum format, uint32 width, uint32 height)
        :mRenderbufferID(0),
        mGLFormat(format)
    {
        // Generate render buffer
        CHECK_GL_ERROR(glGenRenderbuffers(1, &mRenderbufferID));

        //TODO debug label

        // Bind it to FBO
        CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mRenderbufferID));
        CHECK_GL_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));
    }

    DiGLES2RenderBuffer::~DiGLES2RenderBuffer()
    {
        CHECK_GL_ERROR(glDeleteRenderbuffers(1, &mRenderbufferID));
    }

    void DiGLES2RenderBuffer::BindToFramebuffer(GLenum attachment)
    {
        CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, mRenderbufferID));
    }

}