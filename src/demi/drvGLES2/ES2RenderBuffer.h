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

#ifndef DiGLES2RenderBuffer_h__
#define DiGLES2RenderBuffer_h__

#include "ES2Prerequisites.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2RenderBuffer
    {
    public:

        DiGLES2RenderBuffer(GLenum format, uint32 width, uint32 height);

        ~DiGLES2RenderBuffer();

    public:

        void    BindToFramebuffer(GLenum attachment);

        GLenum  GetGLFormat() const { return mGLFormat; }

    private:

        GLuint  mRenderbufferID;

        GLenum  mGLFormat;
    };
}

#endif