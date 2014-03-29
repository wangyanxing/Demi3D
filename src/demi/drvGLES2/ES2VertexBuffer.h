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

#ifndef DiGLES2VertexBuffer_h__
#define DiGLES2VertexBuffer_h__

#include "ES2Prerequisites.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2VertexBuffer : public DiVertexBuffer
    {
    public:

        DiGLES2VertexBuffer();

        ~DiGLES2VertexBuffer();

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

        GLuint      mBufferId;

        uint32      mLockStart;

        uint32      mLockSize;
    };

    class DI_GLES2_API DiGLES2VertexDeclaration : public DiVertexDeclaration
    {
    public:

        DiGLES2VertexDeclaration() {}

        ~DiGLES2VertexDeclaration(){ Release(); }

    public:

        void        Bind(){}

        void        Release(){ mVertexElements.ClearElements(); }

        void        Create(){}
    };
}

#endif
