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
