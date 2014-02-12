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

namespace Demi
{
    class DI_GLDRV_API DiGLRenderBuffer
    {
    public:

        DiGLRenderBuffer(GLenum format, uint32 width, uint32 height);

        ~DiGLRenderBuffer();

    public:

        void    BindToFramebuffer(GLenum attachment);

        GLenum  GetGLFormat() const { return mGLFormat; }

    private:

        GLuint  mRenderbufferID;

        GLenum  mGLFormat;
    };
}