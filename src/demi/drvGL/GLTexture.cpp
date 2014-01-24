
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
        mImageSize(0),
        mBuffer(nullptr),
        mCurrentLockFlag(LOCK_NORMAL)
    {
        mParent = parent;
    }

    DiGLTextureDrv::~DiGLTextureDrv()
    {
        if (mBuffer->data)
        {
            delete[] (uint8*)mBuffer->data;
            mBuffer->data = nullptr;
        }
    }

    void DiGLTextureDrv::CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst)
    {
        DI_ASSERT(mBuffer);
        if (!mBuffer->Contains(srcBox))
        {
            DI_WARNING("Source box out of range.");
            return;
        }

        if (mBuffer->GetWidth() != srcBox.GetWidth() || mBuffer->GetHeight() != srcBox.GetHeight())
        {
            DI_WARNING("No scale is supported currently.");
            return;
        }

        Download(dst,0,0);
    }

    void DiGLTextureDrv::Release()
    {
        glDeleteTextures(1, &mTextureID); 
        if (mBuffer)
        {
            DI_DELETE mBuffer;
            mBuffer = nullptr;
        }
    }

    void DiGLTextureDrv::CreateTexture()
    {
        uint32 width = mParent->GetWidth();
        uint32 height = mParent->GetHeight();
        uint32 numLevels = mParent->GetNumLevels();
        DiPixelFormat fmt = mParent->GetFormat();

        mBuffer = DI_NEW DiPixelBox(width, height, fmt);

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
            glActiveTextureARB(GL_TEXTURE0 + samplerIndex);
        }
    }

    void* DiGLTextureDrv::LockLevel(uint32 level, uint32 surface, DiLockFlag lockflag)
    {
        DiBox lockbox(0, 0, mBuffer->GetWidth(), mBuffer->GetHeight());

        AllocateBuffer();

        mCurrentLockFlag = lockflag;
        if (lockflag != LOCK_DISCARD)
        {
            Download(*mBuffer, level, surface);
        }

        return mBuffer->data;
    }

    void DiGLTextureDrv::UnlockLevel(uint32 level, uint32 surface)
    {
        if (mCurrentLockFlag != LOCK_READ_ONLY)
        {
            DiBox lockbox(0, 0, mBuffer->GetWidth(), mBuffer->GetHeight());
            Upload(*mBuffer, lockbox, level, surface);
        }
        DeallocateBuffer();
    }

    void* DiGLTextureDrv::GetSurfaceHandle()
    {
        return nullptr;
    }

    void DiGLTextureDrv::CopyFromMemory(const DiPixelBox &srcBox, const DiBox &dst, uint32 level, uint32 surface /*= 0*/)
    {
        AllocateBuffer();
        Upload(srcBox, dst, level, surface);
        DeallocateBuffer();
    }

    void DiGLTextureDrv::AllocateBuffer()
    {
        if (!mBuffer->data)
        {
            mBuffer->data = DI_NEW uint8[mImageSize];
        }
    }

    void DiGLTextureDrv::DeallocateBuffer()
    {
        if (mParent->GetResourceUsage() & RU_STATIC)
        {
            delete[] (uint8*)mBuffer->data;
            mBuffer->data = nullptr;
        }
    }

    void DiGLTextureDrv::Upload(const DiPixelBox &src, const DiBox &dst, uint32 level, uint32 surface)
    {
        glBindTexture(mGLTextureType, mTextureID);
        
        DiPixelFormat fmt = mParent->GetFormat();
        BOOL isCompressed = DiPixelBox::IsCompressedFormat(fmt);

        GLenum faceType = GL_TEXTURE_2D;
        if (mGLTextureType == GL_TEXTURE_CUBE_MAP)
            faceType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + surface;

        if (isCompressed)
        {
            switch (mGLTextureType)
            {
            case GL_TEXTURE_2D:
            case GL_TEXTURE_CUBE_MAP:
                if (dst.left == 0 && dst.top == 0)
                {
                    glCompressedTexImage2DARB(faceType, level,
                        mGLFormat,
                        dst.GetWidth(),
                        dst.GetHeight(),
                        0,
                        src.GetConsecutiveSize(),
                        src.data);
                }
                else
                {
                    glCompressedTexSubImage2DARB(faceType, level,
                        dst.left, dst.top,
                        dst.GetWidth(), dst.GetHeight(),
                        mGLFormat, src.GetConsecutiveSize(),
                        src.data);
                }
                break;
            }
        }
        else
        {
            if (src.GetWidth() != src.rowPitch)
                glPixelStorei(GL_UNPACK_ROW_LENGTH, src.rowPitch);
            if (src.GetWidth() > 0 && src.GetHeight()*src.GetWidth() != src.slicePitch)
                glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, (src.slicePitch / src.GetWidth()));
            if (src.left > 0 || src.top > 0)
                glPixelStorei(GL_UNPACK_SKIP_PIXELS, src.left + src.rowPitch * src.top);
            if ((src.GetWidth()*DiPixelBox::GetNumElemBytes(src.format)) & 3) {
                // Standard alignment of 4 is not right
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            }
            switch (mGLTextureType) 
            {
            case GL_TEXTURE_2D:
            case GL_TEXTURE_CUBE_MAP:
                glTexSubImage2D(faceType, level,
                    dst.left, dst.top,
                    dst.GetWidth(), dst.GetHeight(),
                    mGLFormat, mGLFormat,
                    src.data);
                break;
            }
        }

        // restore
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        if (GLEW_VERSION_1_2)
            glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    void DiGLTextureDrv::Download(const DiPixelBox &data, uint32 level, uint32 surface)
    {
        if (data.GetWidth() != mParent->GetWidth() ||
            data.GetHeight() != mParent->GetHeight())
        {
            DI_WARNING("Only download of entire buffer of the texture is supported.");
            return;
        }

        glBindTexture(mGLTextureType, mTextureID);

        DiPixelFormat fmt = mParent->GetFormat();
        BOOL isCompressed = DiPixelBox::IsCompressedFormat(fmt);

        GLenum faceType = GL_TEXTURE_2D;
        if (mGLTextureType == GL_TEXTURE_CUBE_MAP)
            faceType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + surface;
        
        if (isCompressed)
        {
            glGetCompressedTexImageARB(faceType, level, data.data);
        }
        else
        {
            if (data.GetWidth() != data.rowPitch)
                glPixelStorei(GL_PACK_ROW_LENGTH, data.rowPitch);
            if (data.GetHeight()*data.GetWidth() != data.slicePitch)
                glPixelStorei(GL_PACK_IMAGE_HEIGHT, (data.slicePitch / data.GetWidth()));
            if (data.left > 0 || data.top > 0)
                glPixelStorei(GL_PACK_SKIP_PIXELS, data.left + data.rowPitch * data.top);
            if ((data.GetWidth()*DiPixelBox::GetNumElemBytes(data.format)) & 3) {
                // Standard alignment of 4 is not right
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
            }
            // We can only get the entire texture
            glGetTexImage(faceType, level, mGLFormat, mGLFormat, data.data);
            // Restore defaults
            glPixelStorei(GL_PACK_ROW_LENGTH, 0);
            glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
            glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
            glPixelStorei(GL_PACK_ALIGNMENT, 4);
        }
    }

    void DiGLTextureDrv::BindToFrameBuffer(GLenum attachment, uint32 level, uint32 surface)
    {
        GLenum faceType = GL_TEXTURE_2D;
        if (mGLTextureType == GL_TEXTURE_CUBE_MAP)
            faceType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + surface;

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment,
            faceType, mTextureID, level);
    }

}