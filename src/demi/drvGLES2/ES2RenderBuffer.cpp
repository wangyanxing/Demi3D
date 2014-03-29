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
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, format, width, height);
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