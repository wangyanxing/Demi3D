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

#ifndef DiSort_h__
#define DiSort_h__


namespace Demi
{
    template<class _RanIt,
    class _Pr> inline
        void Sort(_RanIt _First, _RanIt _Last, _Pr _Pred)
    {
        std::sort(_First,_Last,_Pred);
    }

    template<class _RanIt> 
    inline void Sort(_RanIt _First, _RanIt _Last)
    {
        std::sort(_First,_Last);
    }
}


#endif
