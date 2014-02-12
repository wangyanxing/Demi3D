


#ifndef DiplaneBoundedVolume_h__
#define DiplaneBoundedVolume_h__


#include "MiscPrerequisites.h"
#include "AxisAlignedBox.h"
#include "Sphere.h"
#include "Math.h"
#include "Plane.h"

#include <vector>

namespace Demi 
{
    class DI_MISC_API DiPlaneBoundedVol
    {
    public:
        typedef DiVector<DiPlane> PlaneList;
        /// Publicly accessible plane list, you can modify this direct
        PlaneList planes;
        DiPlane::Side outside;

        DiPlaneBoundedVol() :outside(DiPlane::NEGATIVE_SIDE) {}
        /** Constructor, determines which side is deemed to be 'outside' */
        DiPlaneBoundedVol(DiPlane::Side theOutside) 
            : outside(theOutside) {}

        /** Intersection test with AABB
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const DiAABB& box) const
        {
            if (box.IsNull()) return false;
            if (box.IsInfinite()) return true;

            // Get centre of the box
            DiVec3 centre = box.GetCenter();
            // Get the half-size of the box
            DiVec3 halfSize = box.GetHalfSize();
            
            PlaneList::const_iterator i, iend;
            iend = planes.end();
            for (i = planes.begin(); i != iend; ++i)
            {
                const DiPlane& plane = *i;

                DiPlane::Side side = plane.getSide(centre, halfSize);
                if (side == outside)
                {
                    // Found a splitting plane therefore return not intersecting
                    return false;
                }
            }

            // couldn't find a splitting plane, assume intersecting
            return true;

        }
        /** Intersection test with Sphere
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const DiSphere& sphere) const
        {
            PlaneList::const_iterator i, iend;
            iend = planes.end();
            for (i = planes.begin(); i != iend; ++i)
            {
                const DiPlane& plane = *i;

                // Test which side of the plane the sphere is
                float d = plane.getDistance(sphere.getCenter());
                // Negate d if planes point inwards
                if (outside == DiPlane::NEGATIVE_SIDE) d = -d;

                if ( (d - sphere.getRadius()) > 0)
                    return false;
            }

            return true;

        }

        inline DiPair<bool, float> intersects(const DiRay& ray)
        {
            return DiMath::intersects(ray, planes, outside == DiPlane::POSITIVE_SIDE);
        }

    };

    typedef DiVector<DiPlaneBoundedVol> PlaneBoundedVolumeList;

}

#endif
