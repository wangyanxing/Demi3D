
/********************************************************************
    File:       GLVertexBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "VertexBuffer.h"

namespace Demi
{
    class DI_GLDRV_API DiGLVertexBuffer : public DiVertexBuffer
    {
    public:

        DiGLVertexBuffer();

        ~DiGLVertexBuffer();

    public:

        void        Create(uint32 size, uint32 usage = RU_STATIC, uint16 streamID = 0);

        void        Bind();

        void        Release();

        void        OnDeviceLost();

        void        OnDeviceReset();

        void*       Lock(uint32 offset, uint32 size, DiLockFlag flag = LOCK_NORMAL);

        void        Unlock();

        GLuint      GetBufferId() const { return mBufferId; }

    private:

        void        ReadData(uint32 offset, uint32 length, void* pDest);

        void        WriteData(uint32 offset, uint32 length,
                        const void* pSource, bool discardWholeBuffer = false);

        bool        SetData(const void* data);

        bool        SetDataRange(const void* data, uint32 start,
                                uint32 count, bool discard = false);

    private:

        GLuint      mBufferId;

        bool        mLockedToScratch;
        
        uint32      mScratchOffset;
        
        uint32      mScratchSize;
        
        void*       mScratchPtr;

        bool        mScratchUploadOnUnlock;

        void*       mLockingScratch;

        uint32      mLockingOffset;

        uint32      mLockingSize;
    };
}