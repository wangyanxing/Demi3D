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
#include "IndexBuffer.h"
#include "GfxDriver.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiIndexBuffer::DiIndexBuffer()
    {
        mMaxIndices    = 0;
        mFormat     = IB_16BITS;
        Driver->AddDeviceLostListener(this);
    }

    DiIndexBuffer::~DiIndexBuffer()
    {
        Driver->RemoveDeviceLostListener(this);
    }
}