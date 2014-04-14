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

#ifndef __RawPtr_H__
#define __RawPtr_H__

#include "GfxPrerequisites.h"
#include "PlatformSpecific.h"
#include "AlignedAllocator.h"

namespace Demi
{
    /** Similar to std::unique_ptr, but:
            * Uses a custom allocator (DEMI_MALLOC_SIMD)
            * Pointers must be really unique (RESTRICT_ALIAS modifier is used!)
            * To access the pointer, use get(); instead of using this container directly
        The purpose of this container is to enclose a raw pointer while avoiding breaking
        the rule of 3 when copying.
        When defining the macro "DEMI_RAW_PTR_PROFILE", this container will raise an exception
        if the copy constructor or the assignment operator are used.
    */
    template <typename T, MemoryCategory M_CATEGORY> class RawSimdUniquePtr
    {
        T * RESTRICT_ALIAS  mPtr;
        size_t              mNumElements;

    public:
        RawSimdUniquePtr() : mPtr( 0 ), mNumElements( 0 ) {}

        RawSimdUniquePtr( size_t numElements ) :
            mPtr( static_cast<T * RESTRICT_ALIAS>(
                    DEMI_MALLOC_SIMD( numElements * sizeof( T ), M_CATEGORY ) ) ),
            mNumElements( numElements )
        {
        }

        RawSimdUniquePtr( const RawSimdUniquePtr &copy ) :
            mPtr( 0 ),
            mNumElements( copy.mNumElements )
        {
            if( copy.mPtr )
            {
                mPtr = static_cast<T * RESTRICT_ALIAS>(
                            DEMI_MALLOC_SIMD( copy.mNumElements * sizeof( T ), M_CATEGORY ) );
                memcpy( mPtr, copy.mPtr, copy.mNumElements * sizeof( T ) );
            }
        }

        ~RawSimdUniquePtr()
        {
            DEMI_FREE_SIMD( mPtr, M_CATEGORY );
            mPtr = 0;
        }

        void swap( RawSimdUniquePtr &copy )
        {
            std::swap( mPtr, copy.mPtr );
            std::swap( mNumElements, copy.mNumElements );
        }

        void operator = ( const RawSimdUniquePtr &copy )
        {
            if( this != &copy )
            {
                DEMI_FREE_SIMD( mPtr, M_CATEGORY );
                mPtr = 0;
                mNumElements = copy.mNumElements;

                if( copy.mPtr )
                {
                    mPtr = static_cast<T * RESTRICT_ALIAS>(
                                    DEMI_MALLOC_SIMD( copy.mNumElements * sizeof( T ), M_CATEGORY ) );
                    memcpy( mPtr, copy.mPtr, copy.mNumElements * sizeof( T ) );
                }
            }
        }

        T * RESTRICT_ALIAS get()                { return mPtr; }
        const T * RESTRICT_ALIAS get() const    { return mPtr; }
        size_t size() const                     { return mNumElements; }
    };
}

#endif
