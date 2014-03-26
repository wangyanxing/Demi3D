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

#ifndef DiK2Coords_h__
#define DiK2Coords_h__

#include "K2Prerequisites.h"
#include "MathMisc.h"

#pragma pack( push, 1 )

namespace Demi
{
    /** 2D plane coordinates
     */
    struct DiK2Pos
    {
        float   x;
        float   z;

        DiK2Pos(void) : x(0.0f), z(0.0f) {}
        DiK2Pos(float fX, float fZ) : x(fX), z(fZ) {}

        void Clear()
        {
            x = 0.0f;
            z = 0.0f;
        }

        DiK2Pos& operator = (DiK2Pos const& rhs)
        {
            x = rhs.x;
            z = rhs.z;
            return *this;
        }

        bool operator == (const DiK2Pos& rsh) const
        {
            return DiMath::RealEqual(x, rsh.x) && DiMath::RealEqual(z, rsh.z);
        }

        bool operator != (const DiK2Pos& Ref) const
        {
            return !((*this) == Ref);
        }
    };

}

#pragma pack( pop )

#endif
