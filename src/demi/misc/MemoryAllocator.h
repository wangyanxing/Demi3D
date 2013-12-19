/**************************************************************************
    File:     MemoryAllocator.h
    Creator:  demiwangya
**************************************************************************/

#pragma once

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