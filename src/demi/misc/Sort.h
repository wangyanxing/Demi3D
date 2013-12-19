
/********************************************************************
    File:       Sort.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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
