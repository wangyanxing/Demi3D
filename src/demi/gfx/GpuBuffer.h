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
