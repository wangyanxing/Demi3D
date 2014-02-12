


#ifndef DiStringHash_h__
#define DiStringHash_h__

#include "Str.h"
#include "HashMap.h"

namespace Demi
{
#if DEMI_HASH_LIB == DEMI_UNORDED

    struct _DiStrHash
    {
        size_t operator()(const DiString& _Keyval) const
        {
            return _Keyval.ToHash();
        }
    };

    struct _DiStrEq : std::binary_function<const DiString&, const DiString&, bool>
    {
        result_type operator()(first_argument_type lhs, second_argument_type rhs) const
        {
            return lhs == rhs;
        }
    };

    template <class _val>
    struct DiStrHash : public DiHashMap <DiString,
        _val, _DiStrHash, _DiStrEq>
    {
    };

#else

    template <class _val>
    struct DiStrHash : public DiHashMap<DiString, _val>
    {
    };

#endif

}

#endif // StringHash_h__