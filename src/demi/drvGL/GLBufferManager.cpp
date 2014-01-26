
#include "GLBufferManager.h"
#include "AlignedAllocator.h"
#include "GLDriver.h"

namespace Demi
{
    // Scratch pool management (32 bit structure)
    struct GLScratchBufferAlloc
    {
        /// Size in bytes
        uint32 size : 31;
        /// Free? (pack with size)
        uint32 free : 1;
    };
#define SCRATCH_POOL_SIZE 1 * 1024 * 1024
#define SCRATCH_ALIGNMENT 32

    //////////////////////////////////////////////////////////////////////////

    DiGLBufferManager::DiGLBufferManager()
        : mScratchBufferPool(nullptr), 
        mMapBufferThreshold(_GL_SCRATCH_THRESHOLD)
        , mMaxScratchBufRequest(0)
    {
        DI_ASSERT(!DiGLDriver::BufferMgr);
        DiGLDriver::BufferMgr = this;

        mScratchBufferPool = static_cast<char*>(DiAlignedMemory::Allocate(SCRATCH_POOL_SIZE, SCRATCH_ALIGNMENT));
        GLScratchBufferAlloc* ptrAlloc = (GLScratchBufferAlloc*)mScratchBufferPool;
        ptrAlloc->size = SCRATCH_POOL_SIZE - sizeof(GLScratchBufferAlloc);
        ptrAlloc->free = 1;

#ifdef WIN32
        // Win32 machines with ATI GPU are having issues glMapBuffer, looks like buffer corruption
        if (true)   //ATI vendor
        {
            mMapBufferThreshold = 0xffffffffUL;  // maximum unsigned long
        }
#else
        mMapBufferThreshold = 0;
#endif
    }

    DiGLBufferManager::~DiGLBufferManager()
    {
        DiAlignedMemory::Deallocate(mScratchBufferPool);
    }

    void* DiGLBufferManager::AllocateScratch(uint32 size)
    {
        if (size % 4 != 0)
        {
            size += 4 - (size % 4);
        }

        uint32 bufferPos = 0;
        while (bufferPos < SCRATCH_POOL_SIZE)
        {
            GLScratchBufferAlloc* pNext = (GLScratchBufferAlloc*)(mScratchBufferPool + bufferPos);
            // Big enough?
            if (pNext->free && pNext->size >= size)
            {
                // split? And enough space for control block
                if (pNext->size > size + sizeof(GLScratchBufferAlloc))
                {
                    uint32 offset = (uint32)sizeof(GLScratchBufferAlloc)+size;

                    GLScratchBufferAlloc* pSplitAlloc = (GLScratchBufferAlloc*)
                        (mScratchBufferPool + bufferPos + offset);
                    pSplitAlloc->free = 1;
                    // split size is remainder minus new control block
                    pSplitAlloc->size = pNext->size - size - sizeof(GLScratchBufferAlloc);

                    // New size of current
                    pNext->size = size;
                }
                // allocate and return
                pNext->free = 0;

                // return pointer just after this control block (++ will do that for us)
                return ++pNext;

            }

            bufferPos += (uint32)sizeof(GLScratchBufferAlloc)+pNext->size;

        }

        // no available alloc
        return 0;
    }

    void DiGLBufferManager::DeallocateScratch(void* ptr)
    {
        uint32 bufferPos = 0;
        GLScratchBufferAlloc* pLast = 0;
        while (bufferPos < SCRATCH_POOL_SIZE)
        {
            GLScratchBufferAlloc* pCurrent = (GLScratchBufferAlloc*)(mScratchBufferPool + bufferPos);

            // Pointers match?
            if ((mScratchBufferPool + bufferPos + sizeof(GLScratchBufferAlloc))
                == ptr)
            {
                // dealloc
                pCurrent->free = 1;

                // merge with previous
                if (pLast && pLast->free)
                {
                    // adjust buffer pos
                    bufferPos -= (pLast->size + (uint32)sizeof(GLScratchBufferAlloc));
                    // merge free space
                    pLast->size += pCurrent->size + sizeof(GLScratchBufferAlloc);
                    pCurrent = pLast;
                }

                // merge with next
                uint32 offset = bufferPos + pCurrent->size + (uint32)sizeof(GLScratchBufferAlloc);
                if (offset < SCRATCH_POOL_SIZE)
                {
                    GLScratchBufferAlloc* pNext = (GLScratchBufferAlloc*)(
                        mScratchBufferPool + offset);
                    if (pNext->free)
                    {
                        pCurrent->size += pNext->size + sizeof(GLScratchBufferAlloc);
                    }
                }

                // done
                return;
            }

            bufferPos += (uint32)sizeof(GLScratchBufferAlloc)+pCurrent->size;
            pLast = pCurrent;

        }

        // Should never get here unless there's a corruption
        DI_WARNING("Scratch deallocation error");
    }

    void* DiGLBufferManager::AllocScratchBuffer(uint32 size)
    {
        if (!size)
            return nullptr;

        if (size > mMaxScratchBufRequest)
            mMaxScratchBufRequest = size;

        for (auto i = mScratchBuffers.begin(); i != mScratchBuffers.end(); ++i)
        {
            if (!i->reserved && i->size >= size)
            {
                i->reserved = true;
                return i->data;
            }
        }

        for (auto i = mScratchBuffers.begin(); i != mScratchBuffers.end(); ++i)
        {
            if (!i->reserved)
            {
                i->data = new uint8[size];
                i->size = size;
                i->reserved = true;
                return i->data;
            }
        }

        ScratchBuffer newBuffer;
        newBuffer.data = new uint8[size];
        newBuffer.size = size;
        newBuffer.reserved = true;
        mScratchBuffers.push_back(newBuffer);
        return newBuffer.data;
    }

    void DiGLBufferManager::DeallocScratchBuffer(void* buffer)
    {
        if (!buffer)
            return;

        for (auto i = mScratchBuffers.begin(); i != mScratchBuffers.end(); ++i)
        {
            if (i->reserved && i->data == buffer)
            {
                i->reserved = false;
                return;
            }
        }

        DI_WARNING("Cannot locate the created scratch buffer.");
    }

    void DiGLBufferManager::ReleaseScratchBuffers()
    {
        for (auto i = mScratchBuffers.begin(); i != mScratchBuffers.end(); ++i)
        {
            if (!i->reserved && i->size > mMaxScratchBufRequest * 2)
            {
                i->data = mMaxScratchBufRequest > 0 ? new uint8[mMaxScratchBufRequest] : nullptr;
                i->size = mMaxScratchBufRequest;
            }
        }

        mMaxScratchBufRequest = 0;
    }
}