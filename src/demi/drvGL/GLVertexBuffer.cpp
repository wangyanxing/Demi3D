
#include "GLVertexBuffer.h"
#include "GLDriver.h"
#include "GLBufferManager.h"
#include "GLTypeMappings.h"

namespace Demi
{
    DiGLVertexBuffer::DiGLVertexBuffer()
        : mBufferId(0)
        , mLockingScratch(nullptr)
        , mLockingOffset(0)
        , mLockingSize(0)
    {
    }

    DiGLVertexBuffer::~DiGLVertexBuffer()
    {
        Release();
    }

    void DiGLVertexBuffer::Create(uint32 size, uint32 usage, uint16 streamID)
    {
        if (mBufferId)
        {
            DI_WARNING("The GL vertex buffer id has already existed. It should be deleted before creating a new one.");
            glDeleteBuffersARB(1, &mBufferId);
        }

        mBufferSize = size;
        mResUsage = (DiResUsage)usage;

        glGenBuffersARB(1, &mBufferId);

        if (!mBufferId)
        {
            DI_WARNING("Cannot create OpenGL vertex buffer");
            return;
        }

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, mBufferSize, NULL, DiGLTypeMappings::GetGLUsage(mResUsage));
    }

    void DiGLVertexBuffer::Bind()
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);
    }

    void DiGLVertexBuffer::Release()
    {
        glDeleteBuffersARB(1, &mBufferId);
    }

    void DiGLVertexBuffer::OnDeviceLost()
    {
    }

    void DiGLVertexBuffer::OnDeviceReset()
    {
    }

#if 0
    void* DiGLVertexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag)
    {
        void* lk = nullptr;
        
        if (size < DiGLDriver::BufferMgr->GetMapBufferThreshold())
        {
            lk = DiGLDriver::BufferMgr->AllocateScratch(size);
            if (lk)
            {
                mLockedToScratch = true;
                mScratchOffset = offset;
                mScratchSize = size;
                mScratchPtr = lk;
                mScratchUploadOnUnlock = (flag != LOCK_READ_ONLY);

                if (flag != LOCK_DISCARD)
                {
                    ReadData(offset, size, lk);
                }
            }
        }

        if (!lk)
        {
            GLenum access = 0;
            
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);
            
            if (flag == LOCK_DISCARD)
            {
                glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL,
                    DiGLTypeMappings::GetGLUsage(mResUsage));
            }

            if (mResUsage & RU_WRITE_ONLY)
                access = GL_WRITE_ONLY_ARB;
            else if (flag == LOCK_READ_ONLY)
                access = GL_READ_ONLY_ARB;
            else
                access = GL_READ_WRITE_ARB;

            void* pBuffer = glMapBufferARB(GL_ARRAY_BUFFER_ARB, access);

            if (pBuffer == 0)
            {
                DI_WARNING("Failed to lock the vertex buffer");
            }

            lk = static_cast<void*>(
                static_cast<unsigned char*>(pBuffer)+offset);

            mLockedToScratch = false;
        }

        return lk;
    }

    void DiGLVertexBuffer::Unlock()
    {
        if (mLockedToScratch)
        {
            if (mScratchUploadOnUnlock)
            {
                WriteData(mScratchOffset, mScratchSize, mScratchPtr,
                    mScratchOffset == 0 && mScratchSize == mBufferSize);
            }

            DiGLDriver::BufferMgr->DeallocateScratch(mScratchPtr);
            mLockedToScratch = false;
        }
        else
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

            if (!glUnmapBufferARB(GL_ARRAY_BUFFER_ARB))
            {
                DI_WARNING("Failed to unlock the vertex buffer");
            }
        }
    }

#endif

    void DiGLVertexBuffer::ReadData(uint32 offset, uint32 length, void* pDest)
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);
        glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, pDest);
    }

    void DiGLVertexBuffer::WriteData(uint32 offset, uint32 length, 
        const void* pSource, bool discardWholeBuffer /*= false*/)
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

        if (offset == 0 && length == mBufferSize)
        {
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, mBufferSize, pSource,
                DiGLTypeMappings::GetGLUsage(mResUsage));
        }
        else
        {
            if (discardWholeBuffer)
            {
                glBufferDataARB(GL_ARRAY_BUFFER_ARB, mBufferSize, NULL,
                    DiGLTypeMappings::GetGLUsage(mResUsage));
            }

            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, pSource);
        }
    }

    void* DiGLVertexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag /*= LOCK_NORMAL*/)
    {
        mLockingOffset = offset;
        mLockingSize = size;

        mLockingScratch = DiGLDriver::BufferMgr->AllocScratchBuffer(size);
        return mLockingScratch;
    }

    void DiGLVertexBuffer::Unlock()
    {
        SetDataRange(mLockingScratch, mLockingOffset, mLockingSize);
        DiGLDriver::BufferMgr->DeallocScratchBuffer(mLockingScratch);
        mLockingScratch = nullptr;
        mLockingOffset = 0;
        mLockingSize = 0;
    }

    bool DiGLVertexBuffer::SetData(const void* data)
    {
        if (!data)
            return false;

        glBindBufferARB(GL_ARRAY_BUFFER, mBufferId);
        glBufferDataARB(GL_ARRAY_BUFFER, mBufferSize, data, DiGLTypeMappings::GetGLUsage(mResUsage));

        return true;
    }

    bool DiGLVertexBuffer::SetDataRange(const void* data, uint32 start,
        uint32 count, bool discard /*= false*/)
    {
        if (start == 0 && count == mLockingSize)
            return SetData(data);

        if (!data || !count)
            return false;

        if (start + count > mBufferSize)
        {
            DI_WARNING("Invalid vertex buffer data range");
            return false;
        }

        glBindBufferARB(GL_ARRAY_BUFFER, mBufferId);

        if (!discard || start != 0)
            glBufferSubDataARB(GL_ARRAY_BUFFER, start, count, data);
        else
            glBufferDataARB(GL_ARRAY_BUFFER, count, data, DiGLTypeMappings::GetGLUsage(mResUsage));

        return true;
    }

}