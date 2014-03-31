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
#include "ES2VertexBuffer.h"
#include "GLES2Driver.h"
#include "ES2TypeMappings.h"
#include "ES2Util.h"

namespace Demi
{
    DiGLES2VertexBuffer::DiGLES2VertexBuffer()
        : mBufferId(0)
        , mLockStart(0)
        , mLockSize(0)
    {
    }

    DiGLES2VertexBuffer::~DiGLES2VertexBuffer()
    {
        Release();
    }

    void DiGLES2VertexBuffer::Create(uint32 size, uint32 usage, uint16 streamID)
    {
        if (mBufferId)
        {
            DI_WARNING("The GL vertex buffer id has already existed. It should be deleted before creating a new one.");
            DiGLES2Driver::StateCache->deleteGLBuffer(GL_ARRAY_BUFFER, mBufferId);
        }

        mBufferSize = size;
        mResUsage = (DiResUsage)usage;
        mStreamId = streamID;

        glGenBuffers(1, &mBufferId);

        if (!mBufferId)
        {
            DI_WARNING("Cannot create OpenGL vertex buffer");
            return;
        }

        DiGLES2Driver::StateCache->bindGLBuffer(GL_ARRAY_BUFFER, mBufferId);
        glBufferData(GL_ARRAY_BUFFER, mBufferSize, NULL, DiGLTypeMappings::GetGLUsage(mResUsage));
    }

    void DiGLES2VertexBuffer::Bind()
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ARRAY_BUFFER, mBufferId);
    }

    void DiGLES2VertexBuffer::Release()
    {
        DiGLES2Driver::StateCache->deleteGLBuffer(GL_ARRAY_BUFFER, mBufferId);
    }

    void DiGLES2VertexBuffer::OnDeviceLost()
    {
    }

    void DiGLES2VertexBuffer::OnDeviceReset()
    {
    }

    void* DiGLES2VertexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag /*= LOCK_NORMAL*/)
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ARRAY_BUFFER, mBufferId);
        
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
            access |= GL_MAP_UNSYNCHRONIZED_BIT_EXT;
        }
        else if (flag == LOCK_READ_ONLY)
            access = GL_MAP_READ_BIT_EXT;
        else
            access = GL_MAP_READ_BIT_EXT | GL_MAP_WRITE_BIT_EXT;

        CHECK_GL_ERROR(pBuffer = glMapBufferRangeEXT(GL_ARRAY_BUFFER, offset, size, access));

        if (pBuffer == 0)
        {
            DI_WARNING("Cannot lock vertex buffer, out of memory");
        }

        // return offsetted
        void* retPtr = static_cast<void*>(
            static_cast<unsigned char*>(pBuffer)+offset);

        mLockStart = offset;
        mLockSize = size;

        return retPtr;
    }

    void DiGLES2VertexBuffer::Unlock()
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ARRAY_BUFFER, mBufferId);
        if (mResUsage & RU_WRITE_ONLY)
        {
            CHECK_GL_ERROR(glFlushMappedBufferRangeEXT(GL_ARRAY_BUFFER, mLockStart, mLockSize));
        }

        GLboolean mapped;
        CHECK_GL_ERROR(mapped = glUnmapBufferOES(GL_ARRAY_BUFFER));
        if (!mapped)
        {
            DI_WARNING("Vertex buffer data corrupted, please reload");
        }

        mLockStart = 0;
        mLockSize = 0;
    }

    void DiGLES2VertexBuffer::ReadData(uint32 offset, uint32 length, void* pDest)
    {
        if (DiGLES2Driver::GLUtil->CheckExtension("GL_EXT_map_buffer_range") || gleswIsSupported(3, 0))
        {
            // Map the buffer range then copy out of it into our destination buffer
            void* srcData;
            CHECK_GL_ERROR(srcData = glMapBufferRangeEXT(GL_ARRAY_BUFFER, offset, length, GL_MAP_READ_BIT_EXT));
            memcpy(pDest, srcData, length);

            // Unmap the buffer since we are done.
            GLboolean mapped;
            CHECK_GL_ERROR(mapped = glUnmapBufferOES(GL_ARRAY_BUFFER));
            if (!mapped)
            {
                DI_WARNING("Buffer data corrupted, please reload");
            }
        }
        else
        {
            DI_WARNING("Read hardware buffer is not supported");
        }
    }

    void DiGLES2VertexBuffer::WriteData(uint32 offset, uint32 length, const void* pSource, bool discardWholeBuffer /*= false*/)
    {
        DiGLES2Driver::StateCache->bindGLBuffer(GL_ARRAY_BUFFER, mBufferId);

        if (offset == 0 && length == mBufferSize)
        {
            CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, mBufferSize, pSource,
                DiGLTypeMappings::GetGLUsage(mResUsage)));
        }
        else
        {
            if (discardWholeBuffer)
            {
                CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, mBufferSize, NULL,
                    DiGLTypeMappings::GetGLUsage(mResUsage)));
            }

            CHECK_GL_ERROR(glBufferSubData(GL_ARRAY_BUFFER, offset, length, pSource));
        }
    }

}