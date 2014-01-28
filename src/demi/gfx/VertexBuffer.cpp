
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
        Driver->AddGpuResource(this);
    }

    DiVertexBuffer::~DiVertexBuffer()
    {
        if (Driver)
            Driver->RemoveGpuResource(this);
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