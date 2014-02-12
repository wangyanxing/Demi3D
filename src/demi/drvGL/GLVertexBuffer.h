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

#ifndef DiGLVertexBuffer_h__
#define DiGLVertexBuffer_h__


#include "VertexBuffer.h"
#include "VertexDeclaration.h"

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

        bool        SetData(const void* data);

        bool        SetDataRange(const void* data, uint32 start,
                                uint32 count, bool discard = false);

    private:

        GLuint      mBufferId;

        void*       mLockingScratch;

        uint32      mLockingOffset;

        uint32      mLockingSize;
    };

    class DI_GLDRV_API DiGLVertexDeclaration : public DiVertexDeclaration
    {
    public:

        DiGLVertexDeclaration() {}

        ~DiGLVertexDeclaration(){ Release(); }

    public:

        void        Bind(){}

        void        Release(){ mVertexElements.ClearElements(); }

        void        Create(){}
    };
}

#endif
