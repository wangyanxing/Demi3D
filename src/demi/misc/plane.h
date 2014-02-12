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

#ifndef Diplane_h__
#define Diplane_h__


#include "MiscPrerequisites.h"
#include "vector3.h"

namespace Demi 
{
    class DI_MISC_API DiPlane
    {
    public:
        /** Default constructor - sets everything to 0.
        */
        DiPlane ();
        DiPlane (const DiPlane& rhs);
        /** Construct a plane through a normal, and a distance to move the plane along the normal.*/
        DiPlane (const DiVec3& rkNormal, float fConstant);
        /** Construct a plane using the 4 constants directly **/
        DiPlane (float a, float b, float c, float d);
        DiPlane (const DiVec3& rkNormal, const DiVec3& rkPoint);
        DiPlane (const DiVec3& rkPoint0, const DiVec3& rkPoint1,
            const DiVec3& rkPoint2);

        /** The "positive side" of the plane is the half space to which the
            plane normal points. The "negative side" is the other half
            space. The flag "no side" indicates the plane itself.
        */
        enum Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
            BOTH_SIDE
        };

        Side getSide (const DiVec3& rkPoint) const;

        /**
        Returns the side where the alignedBox is. The flag BOTH_SIDE indicates an intersecting box.
        One corner ON the plane is sufficient to consider the box and the plane intersecting.
        */
        Side getSide (const DiAABB& rkBox) const;

        /** Returns which side of the plane that the given box lies on.
            The box is defined as centre/half-size pairs for effectively.
        @param centre The centre of the box.
        @param halfSize The half-size of the box.
        @returns
            POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
            NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
            and BOTH_SIDE if the box intersects the plane.
        */
        Side getSide (const DiVec3& centre, const DiVec3& halfSize) const;

        /** This is a pseudodistance. The sign of the return value is
            positive if the point is on the positive side of the plane,
            negative if the point is on the negative side, and zero if the
            point is on the plane.
            @par
            The absolute value of the return value is the true distance only
            when the plane normal is a unit length vector.
        */
        float getDistance (const DiVec3& rkPoint) const;

        /** Redefine this plane based on 3 points. */
        void redefine(const DiVec3& rkPoint0, const DiVec3& rkPoint1,
            const DiVec3& rkPoint2);

        /** Redefine this plane based on a normal and a point. */
        void redefine(const DiVec3& rkNormal, const DiVec3& rkPoint);

        DiVec3 projectVector(const DiVec3& v) const;

        float normalise(void);

        DiVec3 normal;
        float d;

        /// Comparison operator
        bool operator==(const DiPlane& rhs) const
        {
            return (rhs.d == d && rhs.normal == normal);
        }
        bool operator!=(const DiPlane& rhs) const
        {
            return (rhs.d != d || rhs.normal != normal);
        }
    };
}

#endif
