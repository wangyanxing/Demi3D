
/********************************************************************
    File:       GLRenderBuffer.h
    Creator:    demiwangya
*********************************************************************/

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