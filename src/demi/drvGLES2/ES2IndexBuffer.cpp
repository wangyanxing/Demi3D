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
#include "ES2IndexBuffer.h"
#include "GLES2Driver.h"
#include "ES2TypeMappings.h"

namespace Demi
{
    DiGLES2IndexBuffer::DiGLES2IndexBuffer()
        : mBufferId(0)
        , mLockingOffset(0)
        , mLockingSize(0)
    {
    }

    DiGLES2IndexBuffer::~DiGLES2IndexBuffer()
    {
        Release();
    }

    void DiGLES2IndexBuffer::Create(uint32 size, DiIndexBufType type, uint32 usage)
    {
        if (mBufferId)
        {
            DI_WARNING("The GL vertex buffer id has already existed. It should be deleted before creating a new one.");
            DiGLES2Driver::StateCache->deleteGLBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
        }

        glGenBuffers(1, &mBufferId);

        if (!mBufferId)
        {
            DI_WARNING("Cannot create index buffer");
        }

        mBufferSize = size;

        if (mFormat == IB_16BITS)
            mMaxIndices = mBufferSize / sizeof(uint16);
        else
        {
            mMaxIndices = mBufferSize / sizeof(uint32);

            // TODO Caps detection
        }

        mResUsage = (DiResUsage)usage;

        DiGLES2Driver::StateCache->bindGLBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBufferSize, NULL, DiGLTypeMappings::GetGLUsage(mResUsage));
    }

    void DiGLES2IndexBuffer::Bind()
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
    }

    void DiGLES2IndexBuffer::Release()
    {
        DiGLES2Driver::StateCache->deleteGLBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
    }

    void DiGLES2IndexBuffer::OnDeviceLost()
    {
    }

    void DiGLES2IndexBuffer::OnDeviceReset()
    {
    }

    void* DiGLES2IndexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag /*= LOCK_NORMAL*/)
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);

        GLenum access = 0;
        void* pBuffer = nullptr;

        if (mResUsage & RU_WRITE_ONLY)
        {
            access = GL_MAP_WRITE_BIT_EXT;
            access |= GL_MAP_FLUSH_EXPLICIT_BIT_EXT;
            if (flag == LOCK_DISCARD || flag == LOCK_NO_OVERWRITE)
            {
                // Discard the buffer
                access |= GL_MAP_INVALIDATE_RANGE_BIT_EXT;
            }
        }
        else if (flag == LOCK_READ_ONLY)
            access = GL_MAP_READ_BIT_EXT;
        else
            access = GL_MAP_READ_BIT_EXT | GL_MAP_WRITE_BIT_EXT;

        CHECK_GL_ERROR(pBuffer = glMapBufferRangeEXT(GL_ELEMENT_ARRAY_BUFFER, offset, size, access));

        if (pBuffer == 0)
        {
            DI_WARNING("Cannot lock vertex buffer, out of memory");
        }

        // return offsetted
        void* retPtr = static_cast<void*>(
            static_cast<unsigned char*>(pBuffer)+offset);

        mLockingOffset = offset;
        mLockingSize = size;

        return retPtr;
    }

    void DiGLES2IndexBuffer::Unlock()
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ARRAY_BUFFER, mBufferId);

        if (mResUsage & RU_WRITE_ONLY)
        {
            CHECK_GL_ERROR(glFlushMappedBufferRangeEXT(GL_ELEMENT_ARRAY_BUFFER, mLockingOffset, mLockingSize));
        }

        GLboolean mapped;
        CHECK_GL_ERROR(mapped = glUnmapBufferOES(GL_ELEMENT_ARRAY_BUFFER));
        if (!mapped)
        {
            DI_WARNING("Index buffer data corrupted, please reload");
        }

        mLockingOffset = 0;
        mLockingSize = 0;
    }
}