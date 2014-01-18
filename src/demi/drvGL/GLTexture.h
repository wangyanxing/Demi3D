
/********************************************************************
    File:       GLTexture.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Texture.h"

namespace Demi
{
    class DI_GLDRV_API DiGLTextureDrv : public DiTextureDrv
    {
    public:

        DiGLTextureDrv(DiTexture* parent);

        ~DiGLTextureDrv();

    public:

        void                    CreateTexture();

        void                    Release();

        void*                   LockLevel(uint32 level, uint32 &pitch, uint32 surface = 0);

        void                    UnlockLevel(uint32 level, uint32 surface = 0);

        void                    Bind(uint32 samplerIndex);

        void                    BlitToMemory(const DiBox &srcBox, const DiPixelBox &dst);

    private:

        GLenum                  mGLFormat;

        GLuint                  mTextureID;
    };
}