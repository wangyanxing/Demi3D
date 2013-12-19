
/********************************************************************
    File:       D3D9VertexBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "VertexBuffer.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9VertexBuffer : public DiVertexBuffer
    {
    public:

        DiD3D9VertexBuffer();

        ~DiD3D9VertexBuffer();

    public:

        void        Create(uint32 size, uint32 usage = RU_STATIC, uint16 streamID = 0);

        void        Bind();

        void        Release();

        void        OnDeviceLost();

        void        OnDeviceReset();

        void*       Lock(uint32 offset, uint32 size, uint32 flag = 0);

        void        Unlock();

    private:

        IDirect3DVertexBuffer9* mVertexBuffer;

        DWORD       mUsage;

        D3DPOOL     mPool;
    };
}