/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#ifndef DiD3D9VertexBuffer_h__
#define DiD3D9VertexBuffer_h__


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

        void*       Lock(uint32 offset, uint32 size, DiLockFlag flag = LOCK_NORMAL);

        void        Unlock();

        void        ReadData(uint32 offset, uint32 length, void* pDest);

        void        WriteData(uint32 offset, uint32 length, const void* pSource, bool discardWholeBuffer = false);

    private:

        IDirect3DVertexBuffer9* mVertexBuffer;

        DWORD       mUsage;

        D3DPOOL     mPool;
    };
}

#endif
