


#ifndef DiMap_h__
#define DiMap_h__


#include <map>

template <class KEY, class T> 
class DiMap : public std::map<KEY, T, std::less<KEY>, std::allocator< std::pair<const KEY, T> > > 
{
public:

    bool contains(const KEY& key)
    {
        return this->find(key) != this->end();
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
            this->erase(it);
            return true;
        }
    }
};



#endif
