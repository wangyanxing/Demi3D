
#include "GLTexture.h"
#include "Image.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"

namespace Demi
{
    DiGLTextureDrv::DiGLTextureDrv(DiTexture* parent)
        :mGLFormat(GL_NONE),
        mGLTextureType(GL_TEXTURE_2D),
        mTextureID(0),
        mLockedBuffer(nullptr),
        mImageSize(0)
    {
    }

    DiGLTextureDrv::~DiGLTextureDrv()
    {
        if (mLockedBuffer)
        {
            DI_DELETE[] mLockedBuffer;
            mLockedBuffer = nullptr;
        }
    }

    void DiGLTextureDrv::CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst)
    {
    }

    void DiGLTextureDrv::Release()
    {
        glDeleteTextures(1, &mTextureID);
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

        mGLTextureType = DiGLTypeMappings::GetGLTextureType(mParent->GetTextureType());
        glBindTexture(mGLTextureType, mTextureID);

        if (GLEW_VERSION_1_2)
            glTexParameteri(mGLTextureType, GL_TEXTURE_MAX_LEVEL, numLevels);
        
        glTexParameteri(mGLTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(mGLTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (GLEW_VERSION_1_2)
        {
            glTexParameteri(mGLTextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(mGLTextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        mImageSize = DiPixelBox::ComputeImageByteSize(width, height, fmt);

        // TODO: Auto generate mipmap

        BOOL isCompressed = DiPixelBox::IsCompressedFormat(fmt);
        if (isCompressed)
        {
            uint32 imageSize = mImageSize;

            uint8 *tmpdata = new uint8[imageSize];
            memset(tmpdata, 0, imageSize);

            for (size_t i = 0; i < numLevels; i++)
            {
                imageSize = DiPixelBox::ComputeImageByteSize(width, height, fmt);
                switch (mGLTextureType)
                {
                case GL_TEXTURE_2D:
                    glCompressedTexImage2DARB(GL_TEXTURE_2D, i, mGLFormat,
                        width, height, 0, imageSize, tmpdata);
                    break;
                case GL_TEXTURE_CUBE_MAP:
                    for (int face = 0; face < 6; face++) 
                    {
                        glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, i, mGLFormat,
                            width, height, 0, imageSize, tmpdata);
                    }
                    break;
                }
                if (width > 1) width = width / 2;
                if (height > 1) height = height / 2;
            }

            delete[] tmpdata;
        }
        else
        {
            for (size_t i = 0; i < numLevels; i++)
            {
                switch (mGLTextureType)
                {
                case GL_TEXTURE_2D:
                    glTexImage2D(GL_TEXTURE_2D, i, mGLFormat,
                        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                    break;
                case GL_TEXTURE_CUBE_MAP:
                    for (int face = 0; face < 6; face++) 
                    {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, i, mGLFormat,
                            width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                    }
                    break;
                }
                if (width > 1) width = width / 2;
                if (height > 1) height = height / 2;
            }
        }
    }

    void DiGLTextureDrv::Bind(uint32 samplerIndex)
    {
        if (!mTextureID)
        {
            glBindTexture(mGLTextureType, 0);
        }
        else
        {
            glEnable(mGLTextureType);
            glBindTexture(mGLTextureType, mTextureID);
        }
    }

    void* DiGLTextureDrv::LockLevel(uint32 level, uint32 surface, DiLockFlag lockflag)
    {
        if (!mLockedBuffer)
        {
            mLockedBuffer = DI_NEW uint8[mImageSize];
        }

        return mLockedBuffer;
    }

    void DiGLTextureDrv::UnlockLevel(uint32 level, uint32 surface)
    {
        glBindTexture(mGLTextureType, mTextureID);

        DiPixelFormat fmt = mParent->GetFormat();
        BOOL isCompressed = DiPixelBox::IsCompressedFormat(fmt);

        if (isCompressed)
        {
            /*switch(mGLTextureType)
            {
            case GL_TEXTURE_2D:
            case GL_TEXTURE_CUBE_MAP:
            glCompressedTexImage2DARB(mFaceTarget, level,
            mGLFormat,
            dest.getWidth(),
            dest.getHeight(),
            0,
            data.getConsecutiveSize(),
            data.data);
            break;
            }*/
        }
        else
        {

        }

        if (mParent->GetResourceUsage() != RU_STATIC)
        {
            DI_DELETE[] mLockedBuffer;
            mLockedBuffer = nullptr;
        }
    }

    void* DiGLTextureDrv::GetSurfaceHandle()
    {
        return nullptr;
    }

    void DiGLTextureDrv::CopyFromMemory(const DiPixelBox &srcBox, const DiBox &dst, uint32 level, uint32 surface /*= 0*/)
    {

    }
}