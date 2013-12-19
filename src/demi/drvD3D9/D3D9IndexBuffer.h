
/********************************************************************
    File:       D3D9IndexBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "IndexBuffer.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9IndexBuffer : public DiIndexBuffer
    {
    public:

        DiD3D9IndexBuffer();

        ~DiD3D9IndexBuffer();

    public:

        void        Create(uint32 size, DiIndexBufType type = IB_16BITS, uint32 usage = RU_STATIC);

        void        Bind();

        void        Release();

        void        OnDeviceLost();

        void        OnDeviceReset();

        void*       Lock(uint32 offset, uint32 size, uint32 flag = 0);

        void        Unlock();

    private:

        IDirect3DIndexBuffer9* mIndexBuffer;

        DWORD       mUsage;

        D3DPOOL     mPool;
    };
}