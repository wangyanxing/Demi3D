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

#ifndef DiGLES2IndexBuffer_h__
#define DiGLES2IndexBuffer_h__

#include "ES2Prerequisites.h"
#include "IndexBuffer.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2IndexBuffer : public DiIndexBuffer
    {
    public:

        DiGLES2IndexBuffer();

        ~DiGLES2IndexBuffer();

    public:

        void        Create(uint32 size, DiIndexBufType type = IB_16BITS, uint32 usage = RU_STATIC);

        void        Bind();

        void        Release();

        void        OnDeviceLost();

        void        OnDeviceReset();

        void*       Lock(uint32 offset, uint32 size, DiLockFlag flag = LOCK_NORMAL);

        void        Unlock();

        void        ReadData(uint32 offset, uint32 length, void* pDest);

        void        WriteData(uint32 offset, uint32 length, const void* pSource, bool discardWholeBuffer = false);

    private:

        GLuint      mBufferId;

        uint32      mLockingOffset;

        uint32      mLockingSize;
    };
}

#endif
