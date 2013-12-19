/**************************************************************************
    File:       Array.h
    Creator:    demiwangya
**************************************************************************/

#pragma once

#include "MiscPrerequisites.h"

#include <exception>

#pragma warning(disable:4275)

namespace Demi 
{
    class DI_MISC_API ArrayIndexOutOfBoundsException : public std::exception
    {
    public:
        const char* what() const
        {
            return "array index out of bounds exception";
        }
    };


    template<class T, int size>
    class DI_MISC_API Array
    {
    public:
        template<class E>
        T& operator [] (E nPos)    
        { 
            if (nPos < 0 || nPos >= size)
            {
                DI_ASSERT_FAIL;

                throw ArrayIndexOutOfBoundsException();
            }

            return mArray[nPos]; 
        }

        template<class E>
        const T& operator [] (E nPos) const    
        { 
            if (nPos < 0 || nPos >= size)
            {
                DI_ASSERT_FAIL;

                throw ArrayIndexOutOfBoundsException();
            }

            return mArray[nPos]; 
        }

        int GetSize() const
        {
            return size;
        }

        T* GetPtr()
        {
            return mArray;
        }
    private:
        T mArray[size];
    };

}
