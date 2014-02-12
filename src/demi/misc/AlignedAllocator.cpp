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
#include "MiscPch.h"
#include "AlignedAllocator.h"

namespace Demi
{
    void* DiAlignedMemory::Allocate(size_t size, size_t alignment)
    {
        DI_ASSERT(0 < alignment && alignment <= 128 && DiMath::IsPO2(alignment));

        unsigned char* p = DI_NEW unsigned char[size + alignment];
        size_t offset = alignment - (size_t(p) & (alignment-1));

        unsigned char* result = p + offset;
        result[-1] = (unsigned char)offset;

        return result;
    }
    
    void* DiAlignedMemory::Allocate(size_t size)
    {
        return Allocate(size, 16);
    }
    
    void DiAlignedMemory::Deallocate(void* p)
    {
        if (p)
        {
            unsigned char* mem = (unsigned char*)p;
            mem = mem - mem[-1];
            DI_DELETE [] mem;
        }
    }

}