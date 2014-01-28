
/********************************************************************
    File:       Map.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <map>

template <class KEY, class T> 
class DiMap : public std::map<KEY, T, std::less<KEY>, std::allocator< std::pair<const KEY, T> > > 
{
public:

    bool contains(const KEY& key)
    {
        return find(key) != this->end();
    }

    // Find the key in the map, if no result return the default value
    inline T find_def(const KEY& key, T defaultVal)
    {
        auto it = this->find (key);
        if (it == this->end())
            return defaultVal;
        else
            return it->second;
    }

    inline bool removeKey(const KEY& key)
    {
        auto it = this->find(key);
        if (it == this->end())
            return false;
        else
        {
            erase(it);
            return true;
        }
    }
};

