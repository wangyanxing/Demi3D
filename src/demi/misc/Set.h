
/********************************************************************
    File:       Set.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <set>

namespace Demi
{
    template <class T, class COMPARE = std::less<T> >
    class DiSet : public std::set<T, COMPARE, std::allocator<T> > 
    {
    public:
        bool contains(const T& key)
        {
            return find(key) != end();
        }
    };
}