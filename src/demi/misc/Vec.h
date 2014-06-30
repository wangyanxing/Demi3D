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

#ifndef Vec_h__
#define Vec_h__

#include <vector>
#include <algorithm>

namespace Demi
{
    template <typename T>
    class DiVector : public std::vector< T, std::allocator<T> >
    {
    public:
        typedef typename std::vector< T, std::allocator<T> >::iterator _iterator;
        typedef typename std::vector< T, std::allocator<T> >::const_iterator _const_iterator;
        
        DiVector() : std::vector<T, std::allocator<T> >() {}
        DiVector(int size) : std::vector<T, std::allocator<T> >(size) {}
        DiVector(int size, const T& val) : std::vector<T, std::allocator<T> >(size, val) {}
        DiVector(std::initializer_list<T> __il) : std::vector<T, std::allocator<T> >(__il) {}

        _iterator find(const T& value)
        {
            _iterator i = std::find(this->begin(),this->end(),value);
            return i;
        }

        _const_iterator find(const T& value) const
        {
            _const_iterator i = std::find(this->begin(),this->end(),value);
            return i;
        }

        bool contains(const T& value) const
        {
            _const_iterator i = std::find(this->begin(),this->end(),value);
            return i != this->end();
        }

        bool remove(const T& value)
        {
            _iterator i = std::find(this->begin(), this->end(), value);
            if (i == this->end())
                return false;
            else
            {
                erase(i);
                return true;
            }
        }

        // note the order of the elements will be changed after this quickRemove method
        // sometimes we don't care about the order
        _iterator quickRemove(_iterator& it)
        {
            const size_t idx = it - this->begin();
            *it = this->back();
            this->pop_back();
            return this->begin() + idx;
        }
    };

    template<typename T>
    typename T::iterator efficientVectorRemove(T& container, typename T::iterator& iterator)
    {
        const size_t idx = iterator - container.begin();
        *iterator = container.back();
        container.pop_back();

        return container.begin() + idx;
    }
}

#endif
