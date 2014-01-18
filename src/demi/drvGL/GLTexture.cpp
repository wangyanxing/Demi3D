
#include "GLTexture.h"
#include "Image.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"

namespace Demi
{
    DiGLTextureDrv::DiGLTextureDrv(DiTexture* parent)
        :mGLFormat(GL_NONE),
        mTextureID(0)
    {
    }

    DiGLTextureDrv::~DiGLTextureDrv()
    {
    }

    void DiGLTextureDrv::BlitToMemory(const DiBox &srcBox, const DiPixelBox &dst)
    {
        
    }

    void DiGLTextureDrv::Release()
    {
    }

    void DiGLTextureDrv::CreateTexture()
    {
        uint32 width = mParent->GetWidth();
        uint32 height = mParent->GetHeight();
        uint32 numLevels = mParent->GetNumLevels();
        DiPixelFormat fmt = mParent->GetFormat();

        mGLFormat = DiGLTypeMappings::GLFormatMapping[fmt];
        if (mGLFormat == GL_NONE)
        {
            DI_WARNING("Unsupported pixel format: %d", fmt);
            return;
        }

        glGenTextures(1, &mTextureID);

        GLenum gltype = DiGLTypeMappings::GetGLTextureType(mParent->GetTextureType());
        glBindTexture(gltype, mTextureID);

        if (GLEW_VERSION_1_2)
            glTexParameteri(gltype, GL_TEXTURE_MAX_LEVEL, mParent->GetNumLevels());
        
        glTexParameteri(gltype, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(gltype, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (GLEW_VERSION_1_2)
        {
            glTexParameteri(gltype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(gltype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        // TODO: Auto generate mipmap

        BOOL isCompressed = DiPixelBox::IsCompressedFormat(fmt);
    }

    void DiGLTextureDrv::Bind(uint32 samplerIndex)
    {
    }

    void* DiGLTextureDrv::LockLevel(uint32 level, uint32 &pitch, uint32 surface)
    {
        void *buffer = nullptr;
        return buffer;
    }

    void DiGLTextureDrv::UnlockLevel(uint32 level, uint32 surface)
    {
        
    }
}