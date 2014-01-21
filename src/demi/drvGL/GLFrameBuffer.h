
/********************************************************************
    File:       GLFrameBuffer.h
    Creator:    demiwangya
*********************************************************************/

#include "Texture.h"

namespace Demi
{
    class DI_GLDRV_API DiFrameBuffer
    {
    public:

        DiFrameBuffer();

        ~DiFrameBuffer();

    public:

        void            AttachSurface(uint32 attachment, DiTexturePtr surface);

        void            DetarchSurface(uint32 attachment);

    private:

        void            Init();

    private:

        GLuint          mFBOId;

        DiTexturePtr    mColorBuffer[MAX_MRT];
    };
}