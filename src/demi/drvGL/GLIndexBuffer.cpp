
#include "GLIndexBuffer.h"
#include "GLDriver.h"
#include "GLBufferManager.h"
#include "GLTypeMappings.h"

namespace Demi
{
    DiGLIndexBuffer::DiGLIndexBuffer()
        : mBufferId(0)
        , mLockingScratch(nullptr)
        , mLockingOffset(0)
        , mLockingSize(0)
    {

    }

    DiGLIndexBuffer::~DiGLIndexBuffer()
    {
        Release();
    }

    void DiGLIndexBuffer::Create(uint32 size, DiIndexBufType type, uint32 usage)
    {
        if (mBufferId)
            glDeleteBuffersARB(1, &mBufferId);

        glGenBuffersARB(1, &mBufferId);

        if (!mBufferId)
        {
            DI_WARNING("Cannot create index buffer");
        }

        mBufferSize = size;

        if (mFormat == IB_16BITS)
            mMaxIndices = mBufferSize / sizeof(uint16);
        else
            mMaxIndices = mBufferSize / sizeof(uint32);

        mResUsage = (DiResUsage)usage;

        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, NULL,
            DiGLTypeMappings::GetGLUsage(mResUsage));
    }

    void DiGLIndexBuffer::Bind()
    {
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);
    }

    void DiGLIndexBuffer::Release()
    {
        glDeleteBuffersARB(1, &mBufferId);
    }

    void DiGLIndexBuffer::OnDeviceLost()
    {
    }

    void DiGLIndexBuffer::OnDeviceReset()
    {
    }
#if 0
    void* DiGLIndexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag)
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
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

            if (flag == LOCK_DISCARD)
            {
                // Discard the buffer
                glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, NULL,
                    DiGLTypeMappings::GetGLUsage(mResUsage));
            }
            if (mResUsage & RU_WRITE_ONLY)
                access = GL_WRITE_ONLY_ARB;
            else if (flag == LOCK_READ_ONLY)
                access = GL_READ_ONLY_ARB;
            else
                access = GL_READ_WRITE_ARB;

            void* pBuffer = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, access);

            if (pBuffer == 0)
            {
                DI_WARNING("Cannot lock index buffer");
            }

            lk = static_cast<void*>(
                static_cast<unsigned char*>(pBuffer)+offset);

            mLockedToScratch = false;

        }
        
        return lk;
    }

    void DiGLIndexBuffer::Unlock()
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

            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

            if (!glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB))
            {
                DI_WARNING("Cannot unlock index buffer");
            }
        }
    }
#endif

    void DiGLIndexBuffer::ReadData(uint32 offset, uint32 length, void* pDest)
    {
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);
        glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, pDest);
    }

    void DiGLIndexBuffer::WriteData(uint32 offset, uint32 length, const void* pSource, bool discardWholeBuffer /*= false*/)
    {
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

        if (offset == 0 && length == mBufferSize)
        {
            glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, pSource,
                DiGLTypeMappings::GetGLUsage(mResUsage));
        }
        else
        {
            if (discardWholeBuffer)
            {
                glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, NULL,
                    DiGLTypeMappings::GetGLUsage(mResUsage));
            }

            glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, pSource);
        }
    }

    bool DiGLIndexBuffer::SetData(const void* data)
    {
        if (!data)
            return false;

        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, data, DiGLTypeMappings::GetGLUsage(mResUsage));
        return true;
    }

    bool DiGLIndexBuffer::SetDataRange(const void* data, uint32 start, uint32 count, bool discard /*= false*/)
    {
        if (start == 0 && count == mBufferSize)
            return SetData(data);

        if (!data || !count)
            return false;

        if (start + count > mBufferSize)
        {
            DI_WARNING("Invalid index buffer data range");
            return false;
        }

        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);
        if (start == 0 && count == mBufferSize)
        {
            glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, data,
                DiGLTypeMappings::GetGLUsage(mResUsage));
        }
        else
        {
            if (discard)
            {
                glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferSize, NULL,
                    DiGLTypeMappings::GetGLUsage(mResUsage));
            }

            glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, start, count, data);
        }

        return true;
    }

    void* DiGLIndexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag /*= LOCK_NORMAL*/)
    {
        mLockingOffset = offset;
        mLockingSize = size;

        mLockingScratch = DiGLDriver::BufferMgr->AllocScratchBuffer(size);
        return mLockingScratch;
    }

    void DiGLIndexBuffer::Unlock()
    {
        SetDataRange(mLockingScratch, mLockingOffset, mLockingSize);
        DiGLDriver::BufferMgr->DeallocScratchBuffer(mLockingScratch);
        mLockingScratch = nullptr;
        mLockingOffset = 0;
        mLockingSize = 0;
    }

}