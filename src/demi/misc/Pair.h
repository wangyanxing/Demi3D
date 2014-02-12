


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
