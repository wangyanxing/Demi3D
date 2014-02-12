


#ifndef DiSet_h__
#define DiSet_h__


#include <set>

namespace Demi
{
    template <class T, class COMPARE = std::less<T> >
    class DiSet : public std::set<T, COMPARE, std::allocator<T> > 
    {
    public:
        bool contains(const T& key)
        {
            return this->find(key) != this->end();
        }
    };
}

#endif
