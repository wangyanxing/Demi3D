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
#include "ES2Texture.h"
#include "Image.h"
#include "GLES2Driver.h"
#include "ES2TypeMappings.h"
#include "ES2Util.h"
#include "Command.h"
#include "ConsoleVariable.h"
#include "Capabilities.h"

namespace Demi
{
    DiGLES2TextureDrv::DiGLES2TextureDrv(DiTexture* parent)
        :mGLFormat(GL_NONE),
        mGLOriginFormat(GL_NONE),
        mGLTextureType(GL_TEXTURE_2D),
        mTextureID(0),
        mImageSize(0),
        mBuffer(nullptr),
        mCurrentLockFlag(LOCK_NORMAL)
    {
        mParent = parent;
    }

    DiGLES2TextureDrv::~DiGLES2TextureDrv()
    {
        if (mBuffer && mBuffer->data)
        {
            delete[] (uint8*)mBuffer->data;
            mBuffer->data = nullptr;
        }
    }

    void DiGLES2TextureDrv::CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst)
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

    void DiGLES2TextureDrv::Release()
    {
        glDeleteTextures(1, &mTextureID); 
        if (mBuffer)
        {
            DI_DELETE mBuffer;
            mBuffer = nullptr;
        }
    }
    
    void DiGLES2TextureDrv::CreateTexture()
    {
        uint32 numMipmaps = mParent->GetNumLevels();
        DiPixelFormat fmt = mParent->GetFormat();

        uint32 width = mParent->GetWidth();
        uint32 height = mParent->GetHeight();

        if (!Driver->GetGfxCaps()->hasCapability(RSC_NON_POWER_OF_2_TEXTURES) ||
            (Driver->GetGfxCaps()->getNonPOW2TexturesLimited() && numMipmaps > 0))
        {
            // Convert to nearest power-of-two size if required
            width = DiGLTypeMappings::OptionalPO2(width);
            height = DiGLTypeMappings::OptionalPO2(height);
        }

        fmt = DiGLTypeMappings::GetNativeFormat(mParent->GetTextureType(), fmt, mParent->GetResourceUsage());

        mBuffer = DI_NEW DiPixelBox(width, height, fmt);

        // generate the texture
        CHECK_GL_ERROR(glGenTextures(1, &mTextureID));

        // bind texture
        mGLTextureType = DiGLTypeMappings::GetGLTextureType(mParent->GetTextureType());
        DiGLES2Driver::StateCache->bindGLTexture(mGLTextureType, mTextureID);

        // mip maps
        uint32 maxMips = DiGLTypeMappings::GetMaxMipmaps(width, height, 1, fmt);
        bool genMipmap = Driver->GetGfxCaps()->hasCapability(RSC_AUTOMIPMAP) && !DiPixelBox::IsCompressedFormat(fmt);
        if (!DiMath::IsPO2(width) || !DiMath::IsPO2(height))
            genMipmap = false;

        // glGenerateMipmap require all mip levels to be prepared. So override how many this texture has.
        if (mParent->IsAutoMipmap() && genMipmap && numMipmaps)
            numMipmaps = maxMips;

        if (DiGLES2Driver::GLUtil->CheckExtension("GL_APPLE_texture_max_level") || gleswIsSupported(3, 0))
            DiGLES2Driver::StateCache->setTexParameteri(mGLTextureType, GL_TEXTURE_MAX_LEVEL_APPLE, numMipmaps);

        // Set some misc default parameters, these can of course be changed later
        DiGLES2Driver::StateCache->setTexParameteri(mGLTextureType,
            GL_TEXTURE_MIN_FILTER, (mParent->IsAutoMipmap() && numMipmaps) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
        DiGLES2Driver::StateCache->setTexParameteri(mGLTextureType,
            GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        DiGLES2Driver::StateCache->setTexParameteri(mGLTextureType,
            GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        DiGLES2Driver::StateCache->setTexParameteri(mGLTextureType,
            GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        GLenum glErr = GL_NO_ERROR;
        mImageSize = DiPixelBox::ComputeImageByteSize(width, height, fmt);
        mGLFormat = DiGLTypeMappings::GetClosestGLInternalFormat(fmt);
        if (mGLFormat == GL_NONE)
        {
            DI_WARNING("Unsupported pixel format: %d", fmt);
            return;
        }
        mGLOriginFormat = DiGLTypeMappings::GLFormatMapping[fmt];
        unsigned dataType = DiGLTypeMappings::GetDataType(fmt);

        bool isCompressed = DiPixelBox::IsCompressedFormat(fmt);
        if (!isCompressed)
        {
            for (GLint mip = 0; mip <= numMipmaps; ++mip)
            {
                switch (mGLTextureType)
                {
                case GL_TEXTURE_2D:
                    CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D,
                        mip,
                        mGLFormat,
                        width, height,
                        0,
                        mGLOriginFormat,
                        dataType, 0));
                    break;
                case GL_TEXTURE_CUBE_MAP:
                    for (int face = 0; face < 6; face++)
                    {
                        CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, mGLFormat,
                            width, height, 0,
                            mGLOriginFormat, dataType, 0));
                    }
                    break;
                }

                if (width > 1)
                    width = DiMath::FirstPO2From(width / 2);
                if (height > 1)
                    height = DiMath::FirstPO2From(height / 2);
            }
        }
        else
        {
            //TODO: is it necessary?
            uint8* tmpdata = new uint8[mImageSize];
            memset(tmpdata, 0, mImageSize);
            for (GLint mip = 0; mip <= numMipmaps; ++mip)
            {
                switch (mGLTextureType)
                {
                case GL_TEXTURE_2D:
                    CHECK_GL_ERROR(glCompressedTexImage2D(GL_TEXTURE_2D,
                        mip,
                        mGLFormat,
                        width, height,
                        0,
                        mImageSize,
                        tmpdata));
                    break;
                case GL_TEXTURE_CUBE_MAP:
                    for (int face = 0; face < 6; face++)
                    {
                        CHECK_GL_ERROR(glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                            mip, 
                            mGLFormat,
                            width, height, 
                            0,
                            mImageSize,
                            tmpdata));
                    }
                    break;
                }

                if (width > 1)
                    width = width / 2;
                if (height > 1)
                    height = height / 2;
            }
            delete[] tmpdata;
        }
    }

    void DiGLES2TextureDrv::Bind(uint32 samplerIndex)
    {
        if (!mTextureID)
        {
            glBindTexture(mGLTextureType, 0);
        }
        else
        {
            glEnable(mGLTextureType);
            glActiveTexture(GL_TEXTURE0 + samplerIndex);
            glBindTexture(mGLTextureType, mTextureID);

            glTexParameteri(mGLTextureType, GL_TEXTURE_WRAP_S, DiGLTypeMappings::GLAddressMode[mParent->GetAddressingU()]);
            glTexParameteri(mGLTextureType, GL_TEXTURE_WRAP_T, DiGLTypeMappings::GLAddressMode[mParent->GetAddressingV()]);

            if (mParent->GetAddressingU() == AM_BORDER ||
                mParent->GetAddressingV() == AM_BORDER)
            {
                glTexParameterfv(mGLTextureType, GL_TEXTURE_BORDER_COLOR, mParent->GetBorderColor().Ptr());
            }

            DiFilterType filter = mParent->GetFilter();
            if (filter == FILTER_DEFAULT)
            {
                auto filterVar = CommandMgr->GetConsoleVar("def_filter");
                if (filterVar)
                    filter = (DiFilterType)filterVar->GetAsInt();
                else
                    filter = FILTER_BILINEAR;
            }

            // Filtering
            switch (filter)
            {
            case FILTER_NEAREST:
                glTexParameteri(mGLTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(mGLTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;

            case FILTER_BILINEAR:
                if (mParent->GetNumLevels() < 2)
                    glTexParameteri(mGLTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                else
                    glTexParameteri(mGLTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                glTexParameteri(mGLTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;

            case FILTER_ANISOTROPIC:
            case FILTER_TRILINEAR:
                if (mParent->GetNumLevels() < 2)
                    glTexParameteri(mGLTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                else
                    glTexParameteri(mGLTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(mGLTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;

            default:
                break;
            }
        }
    }

    void* DiGLES2TextureDrv::LockLevel(uint32 level, uint32 surface, DiLockFlag lockflag)
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

    void DiGLES2TextureDrv::UnlockLevel(uint32 level, uint32 surface)
    {
        if (mCurrentLockFlag != LOCK_READ_ONLY)
        {
            DiBox lockbox(0, 0, mBuffer->GetWidth(), mBuffer->GetHeight());
            Upload(*mBuffer, lockbox, level, surface);
        }
        DeallocateBuffer();
    }

    void* DiGLES2TextureDrv::GetSurfaceHandle()
    {
        return nullptr;
    }

    void DiGLES2TextureDrv::CopyFromMemory(const DiPixelBox &srcBox, const DiBox &dst, uint32 level, uint32 surface /*= 0*/)
    {
        AllocateBuffer();
        Upload(srcBox, dst, level, surface);
        DeallocateBuffer();
    }

    void DiGLES2TextureDrv::AllocateBuffer()
    {
        if (!mBuffer->data)
        {
            mBuffer->data = DI_NEW uint8[mImageSize];
        }
    }

    void DiGLES2TextureDrv::DeallocateBuffer()
    {
        if (mParent->GetResourceUsage() & RU_STATIC)
        {
            delete[] (uint8*)mBuffer->data;
            mBuffer->data = nullptr;
        }
    }

    void DiGLES2TextureDrv::Upload(const DiPixelBox &src, const DiBox &dst, uint32 level, uint32 surface)
    {
        glBindTexture(mGLTextureType, mTextureID);
        
        DiPixelFormat fmt = mParent->GetFormat();
        bool isCompressed = DiPixelBox::IsCompressedFormat(fmt);

        unsigned dataType = DiGLTypeMappings::GetDataType(mGLFormat);

        GLenum faceType = GL_TEXTURE_2D;
        if (mGLTextureType == GL_TEXTURE_CUBE_MAP)
            faceType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + surface;

        if (isCompressed)
        {
            if (!src.IsConsecutive())
            {
                DI_WARNING("Compressed images should be consective.");
                return;
            }

            size_t size = src.GetConsecutiveSize();
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
                        size,
                        src.data);
                }
                else
                {
                    glCompressedTexSubImage2DARB(faceType, level,
                        dst.left, dst.top,
                        dst.GetWidth(), dst.GetHeight(),
                        mGLFormat, size,
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

            if ((src.GetWidth() * DiPixelBox::GetNumElemBytes(src.format)) & 3)
            {
                // Standard alignment of 4 is not right
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            }

            glGetError();

            switch (mGLTextureType) 
            {
            case GL_TEXTURE_2D:
            case GL_TEXTURE_CUBE_MAP:
                if (dst.left == 0 && dst.top == 0)
                {
                    glTexImage2D(faceType, level, mGLFormat, dst.GetWidth(), dst.GetHeight(),
                        0, mGLOriginFormat, dataType, src.data);
                }
                else
                {
                    glTexSubImage2D(faceType, level,
                        dst.left, dst.top,
                        dst.GetWidth(), dst.GetHeight(),
                        mGLOriginFormat, dataType, src.data);
                }
                break;
            }

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR)
                DI_WARNING("Uploading texture (surface %d, level %d): %s", surface, level, (const char*)gluErrorString(glErr));
        }

        // restore
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        if (GLEW_VERSION_1_2)
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    void DiGLES2TextureDrv::Download(const DiPixelBox &data, uint32 level, uint32 surface)
    {
        if (data.GetWidth() != mParent->GetWidth() ||
            data.GetHeight() != mParent->GetHeight())
        {
            DI_WARNING("Only download of entire buffer of the texture is supported.");
            return;
        }

        glBindTexture(mGLTextureType, mTextureID);

        DiPixelFormat fmt = mParent->GetFormat();
        bool isCompressed = DiPixelBox::IsCompressedFormat(fmt);

        GLenum faceType = GL_TEXTURE_2D;
        if (mGLTextureType == GL_TEXTURE_CUBE_MAP)
            faceType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + surface;
        
        GLenum glfmt = DiGLTypeMappings::GLFormatMapping[fmt];
        GLenum glType = DiGLTypeMappings::GetDataType(fmt);
        
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
            glGetTexImage(faceType, level, glfmt, glType, data.data);
            // Restore defaults
            glPixelStorei(GL_PACK_ROW_LENGTH, 0);
            glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
            glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
            glPixelStorei(GL_PACK_ALIGNMENT, 4);
        }
    }

    void DiGLES2TextureDrv::BindToFrameBuffer(GLenum attachment, uint32 level, uint32 surface)
    {
        GLenum faceType = GL_TEXTURE_2D;
        if (mGLTextureType == GL_TEXTURE_CUBE_MAP)
            faceType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + surface;

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment,
            faceType, mTextureID, level);
    }

}