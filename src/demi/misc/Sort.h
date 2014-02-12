


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
