


#ifndef DiGpuBuffer_h__
#define DiGpuBuffer_h__


#include "DeviceLostListener.h"

namespace Demi 
{
    class DI_GFX_API DiGpuBuffer : public DiDeviceLostListener
    {
    public:

        DiGpuBuffer()
        {
            mResUsage = RU_STATIC;
            mOffset    = 0;
            mBufferSize = 0;
        }

        virtual     ~DiGpuBuffer(){}

    public:

        void        SetOffset(uint32 offset);

        uint32      GetOffset() const {return mOffset;}

        uint32      GetBufferSize() const{return mBufferSize;}

    protected:

        uint32        mOffset;

        DiResUsage    mResUsage;

        uint32        mBufferSize;
    };
}

#endif
