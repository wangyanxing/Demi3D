
#include "IndexBuffer.h"
#include "GfxDriver.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiIndexBuffer::DiIndexBuffer()
    {
        mMaxIndices    = 0;
        mFormat     = IB_16BITS;
        Driver->AddGpuResource(this);
    }

    DiIndexBuffer::~DiIndexBuffer()
    {
        Driver->RemoveGpuResource(this);
    }
}