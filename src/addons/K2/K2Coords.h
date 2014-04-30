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
#include "math/MathMisc.h"

#pragma pack( push, 1 )

namespace Demi
{
    /** some global scale data
     */
    struct DEMI_K2_API K2MapScale
    {
        static float GridSize;
    };

    /** 2D plane coordinates
        based on the vert number of the terrain
     */
    struct DiK2Pos
    {
        float   x;
        float   z;

        DiK2Pos(void) : x(0.0f), z(0.0f) {}
        DiK2Pos(float fX, float fZ) : x(fX), z(fZ) {}

        inline void Clear()
        {
            x = 0.0f;
            z = 0.0f;
        }

        inline DiK2Pos& operator = (DiK2Pos const& rhs)
        {
            x = rhs.x;
            z = rhs.z;
            return *this;
        }

        inline bool operator == (const DiK2Pos& rsh) const
        {
            return DiMath::RealEqual(x, rsh.x) && DiMath::RealEqual(z, rsh.z);
        }

        inline bool operator != (const DiK2Pos& Ref) const
        {
            return !((*this) == Ref);
        }

        inline float Distance(const DiK2Pos& rhs) const
        {
            return DiMath::Sqrt(SquareDistance(rhs));
        }

        inline float SquareDistance(const DiK2Pos& rhs) const
        {
            double dx = x - rhs.x;
            double dz = z - rhs.z;
            return (float)(dx * dx + dz * dz);
        }

        inline void FromWorldPos(const DiVec3& worldPos)
        {
            x = worldPos.x / K2MapScale::GridSize;
            z = worldPos.z / K2MapScale::GridSize;
        }

        inline DiVec3 ToWorldPos() const
        {
            DiVec3 ret;
            ret.x = x * K2MapScale::GridSize;
            ret.y = 0;
            ret.z = z * K2MapScale::GridSize;
            return ret;
        }
    };

}

#pragma pack( pop )

#endif
