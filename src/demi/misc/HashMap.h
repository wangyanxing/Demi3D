
/********************************************************************
    File:       HashMap.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <hash_map>

namespace Demi
{
    template <class KEY, class T> 
    class DiHashMap : public stdext::hash_map<KEY, T, 
        stdext::hash_compare<KEY, std::less<KEY> >, std::allocator<  std::pair<const KEY, T> >> 
    {
    public:

        bool contains(const KEY& key) const
        {
            return find(key) != end();
        }
    };
}
