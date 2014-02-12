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
#ifndef DiArray_h__
#define DiArray_h__


#include "MiscPrerequisites.h"

#include <exception>

namespace Demi 
{

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

                throw std::runtime_error("array index out of bounds");
            }

            return mArray[nPos]; 
        }

        template<class E>
        const T& operator [] (E nPos) const    
        { 
            if (nPos < 0 || nPos >= size)
            {
                DI_ASSERT_FAIL;

                throw std::runtime_error("array index out of bounds");
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


#endif
