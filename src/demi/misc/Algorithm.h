
/********************************************************************
    File:      Algorithm.h
    Creator: demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    // return distance between iterators
    template<class _InIt> inline
        typename _InIt::difference_type
        Distance(_InIt _First, _InIt _Last)
    {    
        typename _InIt::difference_type _Off = 0;
        for (; _First != _Last; ++_First)
            ++_Off;

        return (_Off);
    }

    // increment iterator by offset, arbitrary iterators
    template<class _InIt,
    class _Diff> inline
        void Advance(_InIt& _Where, _Diff _Off)
    {
        for (; 0 < _Off; --_Off)
            ++_Where;
        for (; _Off < 0; ++_Off)
            --_Where;
    }

    // find first element not before _Val, using operator<
    template<class _FwdIt,
    class _Ty> inline
        _FwdIt LowerBound(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
    {    
        _FwdIt::difference_type _Count = Demi::Distance(_First, _Last);
        for (; 0 < _Count; )
        {
            _FwdIt::difference_type _Count2 = _Count / 2;
            _FwdIt _Mid = _First;
            Demi::Advance(_Mid, _Count2);

            if ((*_Mid < _Val))
                _First = ++_Mid, _Count -= _Count2 + 1;
            else
                _Count = _Count2;
        }

        return (_First);
    }

    // find first element not before _Val, using operator<
    template<class _FwdIt,
    class _Ty,
    class _Pr> inline
        _FwdIt LowerBound(_FwdIt _First, _FwdIt _Last,
        const _Ty& _Val, _Pr _Pred)
    {
        _FwdIt::difference_type _Count = Demi::Distance(_First, _Last);
        for (; 0 < _Count; )
        {
            _FwdIt::difference_type _Count2 = _Count / 2;
            _FwdIt _Mid = _First;
            Demi::Advance(_Mid, _Count2);

            if (_Pred(*_Mid, _Val))
                _First = ++_Mid, _Count -= _Count2 + 1;
            else
                _Count = _Count2;
        }

        return (_First);
    }

    // find first element that _Val is before, using operator<
    template<class _FwdIt,
    class _Ty> inline
        _FwdIt UpperBound(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
    {
        _FwdIt::difference_type _Count = Demi::Distance(_First, _Last);
        for (; 0 < _Count; )
        {
            _FwdIt::difference_type _Count2 = _Count / 2;
            _FwdIt _Mid = _First;
            Demi::Advance(_Mid, _Count2);

            if (!(_Val < *_Mid))
                _First = ++_Mid, _Count -= _Count2 + 1;
            else
                _Count = _Count2;
        }
        return (_First);
    }

    // find first element that _Val is before, using _Pred
    template<class _FwdIt,
    class _Ty,
    class _Pr> inline
        _FwdIt UpperBound(_FwdIt _First, _FwdIt _Last,
        const _Ty& _Val, _Pr _Pred)
    {
        _FwdIt::difference_type _Count = Demi::Distance(_First, _Last);
        for (; 0 < _Count; )
        {
            _FwdIt::difference_type _Count2 = _Count / 2;
            _FwdIt _Mid = _First;
            Demi::Advance(_Mid, _Count2);

            if (!_Pred(_Val, *_Mid))
                _First = ++_Mid, _Count -= _Count2 + 1;
            else
                _Count = _Count2;
        }
        return (_First);
    }
}