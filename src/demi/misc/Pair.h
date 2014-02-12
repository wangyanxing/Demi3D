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

#ifndef DiPair_h__
#define DiPair_h__


#include <utility>

namespace Demi
{
    template<class _Ty1,class _Ty2> 
    struct DiPair : public std::pair<_Ty1,_Ty2>
    {
    public:
        DiPair() : std::pair<_Ty1,_Ty2>()
        {
        }

        DiPair(const _Ty1& _Val1, const _Ty2& _Val2)
            :std::pair<_Ty1,_Ty2>(_Val1,_Val2)
        {
        }
    };
}


#endif
