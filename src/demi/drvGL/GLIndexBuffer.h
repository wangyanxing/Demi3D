
/********************************************************************
    File:       GLIndexBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "IndexBuffer.h"

namespace Demi
{
    class DI_GLDRV_API DiGLIndexBuffer : public DiIndexBuffer
    {
    public:

        DiGLIndexBuffer();

        ~DiGLIndexBuffer();

    public:

        void        Create(uint32 size, DiIndexBufType type = IB_16BITS, uint32 usage = RU_STATIC);

        void        Bind();

        void        Release();

        void        OnDeviceLost();

        void        OnDeviceReset();

        void*       Lock(uint32 offset, uint32 size, DiLockFlag flag = LOCK_NORMAL);

        void        Unlock();

    private:

        bool        SetData(const void* data);
        
        bool        SetDataRange(const void* data, uint32 start, uint32 count, bool discard = false);

    private:

        GLuint      mBufferId;
        
        bool        mScratchUploadOnUnlock;

        void*       mLockingScratch;

        uint32      mLockingOffset;

        uint32      mLockingSize;
    };
}