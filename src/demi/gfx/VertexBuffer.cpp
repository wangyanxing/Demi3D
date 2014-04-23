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
#include "GfxPch.h"
#include "VertexBuffer.h"
#include "GfxDriver.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiVertexBuffer::DiVertexBuffer()
    {
        mBufferSize  = 0;
        mStride      = 0;
        mStreamId    = 0;
        mInstance    = SOURCE_NO_INSTANCE;
        mInstanceNum = 1;
        Driver->AddDeviceLostListener(this);
    }

    DiVertexBuffer::~DiVertexBuffer()
    {
        if (Driver)
            Driver->RemoveDeviceLostListener(this);
    }

    void DiVertexBuffer::SetStride( uint32 stride )
    {
        mStride = stride;
    }

    void DiVertexBuffer::SetInstanceNum( uint32 rate )
    {
        DI_ASSERT(rate > 0);
        mInstanceNum = rate;
    }
}