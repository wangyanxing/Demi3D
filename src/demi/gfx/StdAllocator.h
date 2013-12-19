/**************************************************************************
    File:       StdAllocator.h
    Creator:    demiwangya
**************************************************************************/

#pragma once

#include "StdMemory.h"
#include <limits>

namespace Engine 
{


    template <typename Type>
    class StdAllocator 
    {
    public:
        //Typedefs 
        typedef Type *pointer;
        typedef const Type *const_pointer;
        typedef Type& reference;
        typedef const Type& const_reference;

        typedef size_t size_type;
        typedef Type value_type;

        typedef std::ptrdiff_t        difference_type;

        //Member Functions 

        //Constructs a specific type of object at a specified address that is initialized with a specified value.
        inline explicit StdAllocator() 
        { 
        }

        StdAllocator(const StdAllocator<Type>& /*_Right*/)
        {
            //this = _Right;
        }

        template<class Other>
        StdAllocator(const StdAllocator<Other>& /*_Right*/)
        {

        }

        void construct(pointer _Ptr, const Type& _Val)
        {
            new(static_cast<void*>(_Ptr)) Type(_Val);
        }

        ~StdAllocator()
        { }

        //Finds the address of an object whose value is specified.
        pointer address( reference _Val ) const
        {
            return &_Val;
        }

        const_pointer address(const_reference _Val) const
        {
            return &_Val;
        }

        //Allocates a block of memory large enough to store at least some specified number of elements.
        pointer allocate(size_type _Count, const Type* _Hint = 0)
        {
            _Hint;

            register size_type stSize = _Count*sizeof( Type );
            pointer pMemory =  static_cast< pointer >( StdMemory::Alloc(stSize) );

            return pMemory;
        }

        //Frees a specified number of objects from storage beginning at a specified position.
        void deallocate(pointer _Ptr, size_type /*_Count*/)
        {        
            StdMemory::Free(_Ptr);
        }

        //Calls an objects destructor without deallocating the memory where the object was stored.
        void destroy(pointer _Ptr)
        {
            _Ptr->~Type();
            _Ptr; // avoid warning c4100
        }

        //Returns the number of elements of type Type that could be allocated by an object of class allocator before the free memory is used up.
        size_type max_size( ) const
        {
            //return static_cast < size_type > ( std::numeric_limits<int>::max() );
            return static_cast < size_type > (0xFFFFFFFF);
        }

        template<class _Other>
        struct rebind 
        {
            typedef StdAllocator<_Other> other;
        };


    };

}