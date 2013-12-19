
/********************************************************************
    File:       IndexBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuBuffer.h"

namespace Demi 
{
    class DI_GFX_API DiIndexBuffer : public DiGpuBuffer
    {
    public:

        DiIndexBuffer();

        virtual                 ~DiIndexBuffer();

    public:

        virtual void            Create(uint32 size, DiIndexBufType type = IB_16BITS, uint32 usage = RU_STATIC) = 0;

        virtual void            Bind() = 0;

        virtual void            Release() = 0;

        virtual void            OnDeviceLost() = 0;

        virtual void            OnDeviceReset() = 0;

        uint32                  GetMaxIndices() const {return mMaxIndices;}

        virtual void*           Lock(uint32 offset, uint32 size, uint32 flag = 0) = 0;

        virtual void            Unlock() = 0;

    protected:

        DiIndexBufType          mFormat;

        uint32                  mMaxIndices;
    };
}