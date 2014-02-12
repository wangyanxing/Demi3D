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

#ifndef Diray_h__
#define Diray_h__


#include "MiscPrerequisites.h"
#include "mathmisc.h"
#include "vector3.h"
#include "planeBoundedVolume.h"

namespace Demi 
{
    class DI_MISC_API DiRay
    {
    protected:
        DiVec3 mOrigin;
        DiVec3 mDirection;
    public:
        DiRay():mOrigin(DiVec3::ZERO), mDirection(DiVec3::UNIT_Z) {}
        DiRay(const DiVec3& origin, const DiVec3& direction)
            :mOrigin(origin), mDirection(direction) {}

        /** Sets the origin of the ray. */
        void setOrigin(const DiVec3& origin) {mOrigin = origin;} 
        /** Gets the origin of the ray. */
        const DiVec3& getOrigin(void) const {return mOrigin;} 

        /** Sets the direction of the ray. */
        void setDirection(const DiVec3& dir) {mDirection = dir;} 
        /** Gets the direction of the ray. */
        const DiVec3& getDirection(void) const {return mDirection;} 

        /** Gets the position of a point t units along the ray. */
        DiVec3 getPoint(float t) const { 
            return DiVec3(mOrigin + (mDirection * t));
        }
        
        /** Gets the position of a point t units along the ray. */
        DiVec3 operator*(float t) const { 
            return getPoint(t);
        }

        /** Tests whether this ray intersects the given plane. 
        @returns A pair structure where the first element indicates whether
            an intersection occurs, and if true, the second element will
            indicate the distance along the ray at which it intersects. 
            This can be converted to a point in space by calling getPoint().
        */
        DiPair<bool, float> intersects(const DiPlane& p) const
        {
            return DiMath::intersects(*this, p);
        }
        /** Tests whether this ray intersects the given plane bounded volume. 
        @returns A pair structure where the first element indicates whether
        an intersection occurs, and if true, the second element will
        indicate the distance along the ray at which it intersects. 
        This can be converted to a point in space by calling getPoint().
        */
        DiPair<bool, float> intersects(const DiPlaneBoundedVol& p) const
        {
            return DiMath::intersects(*this, p.planes, p.outside == DiPlane::POSITIVE_SIDE);
        }
        /** Tests whether this ray intersects the given sphere. 
        @returns A pair structure where the first element indicates whether
            an intersection occurs, and if true, the second element will
            indicate the distance along the ray at which it intersects. 
            This can be converted to a point in space by calling getPoint().
        */
        DiPair<bool, float> intersects(const DiSphere& s) const
        {
            return DiMath::intersects(*this, s);
        }
        /** Tests whether this ray intersects the given box. 
        @returns A pair structure where the first element indicates whether
            an intersection occurs, and if true, the second element will
            indicate the distance along the ray at which it intersects. 
            This can be converted to a point in space by calling getPoint().
        */
        DiPair<bool, float> intersects(const DiAABB& box) const
        {
            return DiMath::intersects(*this, box);
        }

    };

}

#endif
