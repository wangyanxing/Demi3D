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
#ifndef DiMemoryAllocator_h__
#define DiMemoryAllocator_h__


#include "MemoryConfig.h"

#if (MEMORY_ALLLOCATOR_USING == MEMORY_ALLOCATOR_STANDARD)    
    #define DI_MALLOC malloc
    #define DI_FREE   free
#endif

template <class T>
class DiMallocInterface 
{
public:
    inline static T* Allocate(unsigned int uiNumElements)
    {
        return (T*)(DI_MALLOC(sizeof(T)*uiNumElements));
    }
    inline static void Deallocate(T* pArray)
    {
        DI_FREE(pArray);
    }
};

template <class T> 
class DiNewInterface 
{
public:
    inline static T* Allocate(unsigned int uiNumElements)
    {
        return DI_NEW T[uiNumElements];
    }
    inline static void Deallocate(T* pArray)
    {
        DI_DELETE[] pArray;
    }
};

#endif
