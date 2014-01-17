
/********************************************************************
    File:       VertexBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuBuffer.h"

namespace Demi 
{
    enum DiInstanceState
    {
        SOURCE_NO_INSTANCE,
        SOURCE_INSTANCE_INDEX,
        SOURCE_INSTANCE_DATA
    };

    class DI_GFX_API DiVertexBuffer : public DiGpuBuffer
    {
    public:

        DiVertexBuffer();

        virtual                 ~DiVertexBuffer();

    public:

        virtual void            Create(uint32 size, uint32 usage = RU_STATIC, uint16 streamID = 0) = 0;

        virtual void            Bind() = 0;

        virtual void            Release() = 0;

        virtual void            OnDeviceLost() = 0;

        virtual void            OnDeviceReset() = 0;

        void                    SetStride(uint32 stride);

        uint32                  GetStride() const {return mStride;}

        virtual void*           Lock(uint32 offset, uint32 size, DiLockFlag flag = LOCK_NORMAL) = 0;

        virtual void            Unlock() = 0;

        DiInstanceState         GetInstanceState() const { return mInstance; }

        void                    SetInstanceState(DiInstanceState val) { mInstance = val; }

        void                    SetInstanceNum(uint32 rate);

        uint32                  GetInstanceNum() const{return mInstanceNum;}

    protected:

        uint32                  mStride;

        uint32                  mStreamId;

        DiInstanceState         mInstance;

        uint32                  mInstanceNum;
    };
}