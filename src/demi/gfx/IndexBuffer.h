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

#ifndef DiIndexBuffer_h__
#define DiIndexBuffer_h__


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

        virtual void*           Lock(uint32 offset, uint32 size, DiLockFlag flag = LOCK_NORMAL) = 0;

        virtual void            Unlock() = 0;

        DiIndexBufType          GetType() const { return mFormat; }

    protected:

        DiIndexBufType          mFormat;

        uint32                  mMaxIndices;
    };
}

#endif
