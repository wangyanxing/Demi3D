
/********************************************************************
    File:       Vec.h
    Creator:    demiwangya
*********************************************************************/

#pragma once


#include <vector>
#include <algorithm>

namespace Demi
{
    template <typename T>
    class DiVector : public std::vector< T, std::allocator<T> >
    {
    public:
        DiVector() : std::vector<T, std::allocator<T> >() {}
        DiVector(int size) : std::vector<T, std::allocator<T> >(size) {}
        DiVector(int size, const T& val) : std::vector<T, std::allocator<T> >(size, val) {}

        iterator find(const T& value)
        {
            iterator i = std::find(begin(),end(),value);
            return i;
        }

        const_iterator find(const T& value) const
        {
            const_iterator i = std::find(begin(),end(),value);
            return i;
        }

        bool contains(const T& value)
        {
            const_iterator i = std::find(begin(),end(),value);
            return i != end();
        }

        bool remove(const T& value)
        {
            iterator i = std::find(begin(), end(), value);
            if (i == end())
                return false;
            else
            {
                erase(i);
                return true;
            }
        }

        // note the order of the elements will be changed after this quickRemove method
        // sometimes we don't care about the order
        iterator quickRemove(iterator& it)
        {
            const size_t idx = it - begin();
            *it = back();
            pop_back();
            return begin() + idx;
        }
    };
}
