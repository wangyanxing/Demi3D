
#include "plane.h"
#include "axisAlignedBox.h"
#include "matrix3.h"

namespace Demi 
{

    DiPlane::DiPlane ()
    {
        normal = DiVec3::ZERO;
        d = 0.0;
    }
    //-----------------------------------------------------------------------
    DiPlane::DiPlane (const DiPlane& rhs)
    {
        normal = rhs.normal;
        d = rhs.d;
    }
    //-----------------------------------------------------------------------
    DiPlane::DiPlane (const DiVec3& rkNormal, float fConstant)
    {
        normal = rkNormal;
        d = -fConstant;
    }
    //---------------------------------------------------------------------
    DiPlane::DiPlane (float a, float b, float c, float _d)
        : normal(a, b, c), d(_d)
    {
    }
    //-----------------------------------------------------------------------
    DiPlane::DiPlane (const DiVec3& rkNormal, const DiVec3& rkPoint)
    {
        redefine(rkNormal, rkPoint);
    }
    //-----------------------------------------------------------------------
    DiPlane::DiPlane (const DiVec3& rkPoint0, const DiVec3& rkPoint1,
        const DiVec3& rkPoint2)
    {
        redefine(rkPoint0, rkPoint1, rkPoint2);
    }
    //-----------------------------------------------------------------------
    float DiPlane::getDistance (const DiVec3& rkPoint) const
    {
        return normal.dotProduct(rkPoint) + d;
    }
    //-----------------------------------------------------------------------
    DiPlane::Side DiPlane::getSide (const DiVec3& rkPoint) const
    {
        float fDistance = getDistance(rkPoint);

        if ( fDistance < 0.0 )
            return DiPlane::NEGATIVE_SIDE;

        if ( fDistance > 0.0 )
            return DiPlane::POSITIVE_SIDE;

        return DiPlane::NO_SIDE;
    }


    //-----------------------------------------------------------------------
    DiPlane::Side DiPlane::getSide (const DiAABB& box) const
    {
        if (box.IsNull()) 
            return NO_SIDE;
        if (box.IsInfinite())
            return BOTH_SIDE;

        return getSide(box.GetCenter(), box.GetHalfSize());
    }
    //-----------------------------------------------------------------------
    DiPlane::Side DiPlane::getSide (const DiVec3& centre, const DiVec3& halfSize) const
    {
        // Calculate the distance between box centre and the plane
        float dist = getDistance(centre);

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
        float maxAbsDist = normal.absDotProduct(halfSize);

        if (dist < -maxAbsDist)
            return DiPlane::NEGATIVE_SIDE;

        if (dist > +maxAbsDist)
            return DiPlane::POSITIVE_SIDE;

        return DiPlane::BOTH_SIDE;
    }
    //-----------------------------------------------------------------------
    void DiPlane::redefine(const DiVec3& rkPoint0, const DiVec3& rkPoint1,
        const DiVec3& rkPoint2)
    {
        DiVec3 kEdge1 = rkPoint1 - rkPoint0;
        DiVec3 kEdge2 = rkPoint2 - rkPoint0;
        normal = kEdge1.crossProduct(kEdge2);
        normal.normalise();
        d = -normal.dotProduct(rkPoint0);
    }
    //-----------------------------------------------------------------------
    void DiPlane::redefine(const DiVec3& rkNormal, const DiVec3& rkPoint)
    {
        normal = rkNormal;
        d = -rkNormal.dotProduct(rkPoint);
    }
    //-----------------------------------------------------------------------
    DiVec3 DiPlane::projectVector(const DiVec3& p) const
    {
        // We know plane normal is unit length, so use simple method
        DiMat3 xform;
        xform[0][0] = 1.0f - normal.x * normal.x;
        xform[0][1] = -normal.x * normal.y;
        xform[0][2] = -normal.x * normal.z;
        xform[1][0] = -normal.y * normal.x;
        xform[1][1] = 1.0f - normal.y * normal.y;
        xform[1][2] = -normal.y * normal.z;
        xform[2][0] = -normal.z * normal.x;
        xform[2][1] = -normal.z * normal.y;
        xform[2][2] = 1.0f - normal.z * normal.z;
        return xform * p;

    }
    //-----------------------------------------------------------------------
    float DiPlane::normalise(void)
    {
        float fLength = normal.length();

        // Will also work for zero-sized vectors, but will change nothing
        if (fLength > 1e-08f)
        {
            float fInvLength = 1.0f / fLength;
            normal *= fInvLength;
            d *= fInvLength;
        }

        return fLength;
    }

}