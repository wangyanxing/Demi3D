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

#ifndef DiAlignedAllocator_h__
#define DiAlignedAllocator_h__


namespace Demi
{
    class DI_MISC_API DiAlignedMemory
    {
    public:
       
        static void*    Allocate(size_t size, size_t alignment);

        static void*    Allocate(size_t size);

        static void     Deallocate(void* p);
    };
}

#endif
