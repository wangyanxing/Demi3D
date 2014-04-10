/**********************************************************************
This source file is a part of Demi3D
__  ___  __  __  __
|  \|_ |\/||   _)|  \
|__/|__|  ||  __)|__/

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt

@version 2.3
@author Kojack
@author Transporter
@author Klaim
Extracted From: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Euler+Angle+Class

***********************************************************************/

#ifndef DiEuler_h__
#define DiEuler_h__

#include "MiscPrerequisites.h"
#include "math/quaternion.h"
#include "math/vector3.h"
#include "math/matrix3.h"

namespace Demi
{
    class DiEuler
    {
    public:

        enum Orders 
        {
            XYZ = 1,
            XZY,
            YXZ,
            YZX,
            ZXY,
            ZYX
        };

        static void ToQuat(DiQuat& q, const DiVec3& e, Orders order = XYZ)
        {
            struct RotOrderInfo
            {
                short axis[3];
                short parity; // parity of axis permutation (even=0, odd=1)
            };

            static const RotOrderInfo rotOrders[] =
            {
                /* i, j, k, n */
                { { 0, 1, 2 }, 0 }, /* XYZ */
                { { 0, 2, 1 }, 1 }, /* XZY */
                { { 1, 0, 2 }, 1 }, /* YXZ */
                { { 1, 2, 0 }, 0 }, /* YZX */
                { { 2, 0, 1 }, 0 }, /* ZXY */
                { { 2, 1, 0 }, 1 }  /* ZYX */
            };

            const RotOrderInfo *R = (order < 1) ? &rotOrders[0] : &rotOrders[(order)-1];
            short i = R->axis[0], j = R->axis[1], k = R->axis[2];
            double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
            double a[3];

            DiVec3 ang = e;
            //WrapAngle(ang.x);
            //WrapAngle(ang.y);
            //WrapAngle(ang.z);

            ti = ang[i] * 0.5f;
            tj = ang[j] * (R->parity ? -0.5f : 0.5f);
            th = ang[k] * 0.5f;

            ci = DiMath::Cos(ti);
            cj = DiMath::Cos(tj);
            ch = DiMath::Cos(th);
            si = DiMath::Sin(ti);
            sj = DiMath::Sin(tj);
            sh = DiMath::Sin(th);

            cc = ci * ch;
            cs = ci * sh;
            sc = si * ch;
            ss = si * sh;

            a[i] = cj * sc - sj * cs;
            a[j] = cj * ss + sj * cc;
            a[k] = cj * cs - sj * sc;

            q[0] = cj * cc + sj * ss;
            q[1] = a[0];
            q[2] = a[1];
            q[3] = a[2];

            if (R->parity)
                q[j + 1] = -q[j + 1];
        }

        static inline void WrapAngle(float rangle)
        {
            if (rangle < -DiMath::PI)
            {
                rangle = fmod(rangle, -DiMath::TWO_PI);
                if (rangle < -DiMath::PI)
                    rangle += DiMath::TWO_PI;
            }
            else if (rangle > DiMath::PI)
            {
                rangle = fmod(rangle, DiMath::TWO_PI);
                if (rangle > DiMath::PI)
                    rangle -= DiMath::TWO_PI;
            }
        }

        static inline void LimitAngle(DiRadian& angle, const DiRadian& limit)
        {
            if (angle > limit)
                angle = limit;
            else if (angle < -limit)
                angle = -limit;
        }
    };
}

#endif
