
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