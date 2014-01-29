
#include "MiscPch.h"
#include "MathMisc.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "ray.h"
#include "sphere.h"
#include "axisalignedbox.h"
#include "planeBoundedVolume.h"
#include "plane.h"
#include "matrix4.h"

#if (DEMI_COMPILER == DEMI_COMPILER_MSVC)
#   pragma warning(push)
#   pragma warning(disable : 4305)
#   pragma warning(disable : 4127)
#endif

namespace Demi 
{
    #define LINE_EPS (0.00001f)

    const float DiMath::POS_INFINITY    = std::numeric_limits<float>::infinity();
    const float DiMath::NEG_INFINITY    = -std::numeric_limits<float>::infinity();
    const float DiMath::PI              = 3.141592653f;
    const float DiMath::TWO_PI          = 6.283185306f;
    const float DiMath::HALF_PI         = 1.5707963265f;
    const float DiMath::_DEG_TO_RAD     = DiMath::PI / 180.0f;
    const float DiMath::_RAD_TO_DEG     = 180.0f / DiMath::PI;
    const float DiMath::LOG2            = log(2.0f);
    AngleUnit   DiMath::sAngleUnit;

    inline float PointToLineDistance( const DiVec3 &p1,const DiVec3 &p2,const DiVec3 &p3 )
    {
        DiVec3 d = p2 - p1;
        float u = d.dotProduct(p3-p1) / (d).squaredLength();
        if (u < 0)
        {
            return (p3 - p1).length();
        }
        else if (u > 1)
        {
            return (p3 - p2).length();
        }
        else
        {
            DiVec3 p = p1 + u*d;
            return (p3 - p).length();
        }
    }

    inline float PointToLineDistance( const DiVec3 &p1,const DiVec3 &p2,const DiVec3 &p3,DiVec3 &intersectPoint )
    {
        DiVec3 d = p2 - p1;
        float fLength2 = d.squaredLength();

        if(fLength2<0.00001f)
        {
            intersectPoint = p1;
            return (p3 - p1).length();
        }

        float u = d.dotProduct(p3-p1) / fLength2;
        if (u < 0)
        {
            intersectPoint = p1;
            return (p3 - p1).length();
        }
        else if (u > 1)
        {
            intersectPoint = p2;
            return (p3 - p2).length();
        }
        else
        {
            DiVec3 p = p1 + u*d;
            intersectPoint = p;
            return (p3 - p).length();
        }
    }

    inline float RayToLineDistance( const DiVec3 &raySrc,const DiVec3 &rayTrg,
        const DiVec3 &p1,const DiVec3 &p2,DiVec3 &nearestPoint )
    {
        DiVec3 intPnt;
        DiVec3 rayLineP1 = raySrc;
        DiVec3 rayLineP2 = rayTrg;
        DiVec3 pa,pb;
        float ua,ub;

        if (!DiMath::LineLineIntersect( p1,p2, rayLineP1,rayLineP2, pa,pb,ua,ub ))
        {
            return FLT_MAX;
        }

        float d = 0;
        if (ua < 0)
        {
            d = PointToLineDistance( rayLineP1,rayLineP2,p1,intPnt );
        }
        else if (ua > 1)
        {
            d = PointToLineDistance( rayLineP1,rayLineP2,p2,intPnt );
        }
        else
        {
            intPnt = rayLineP1 + ub*(rayLineP2-rayLineP1);
            d = (pb-pa).length();
        }
        nearestPoint = intPnt;
        return d;
    }

    bool DiMath::RealEqual( float a, float b, float tolerance /*= std::numeric_limits<float>::epsilon()*/ )
    {
        if (fabs(b-a) <= tolerance)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    float DiMath::Sign( float fValue )
    {
        if ( fValue > 0.0 )
            return 1.0;

        if ( fValue < 0.0 )
            return -1.0;

        return 0.0;
    }

    float DiMath::InvSqrt( float fValue )
    {
        return 1 / sqrt(fValue);
    }

    float DiMath::UnitRandom()
    {
        return  float( rand() ) / float( RAND_MAX );
    }

    float DiMath::RangeRandom( float fLow, float fHigh )
    {
        return (fHigh-fLow)*UnitRandom() + fLow;
    }

    float DiMath::SymmetricRandom()
    {
        return 2.0f * UnitRandom() - 1.0f;
    }

    bool DiMath::pointInTri2D( const DiVec2& p, const DiVec2& a, 
        const DiVec2& b, const DiVec2& c )
    {
        // Winding must be consistent from all edges for point to be inside
        DiVec2 v1, v2;
        float dot[3];
        bool zeroDot[3];

        v1 = b - a;
        v2 = p - a;

        // Note we don't care about normalisation here since sign is all we need
        // It means we don't have to worry about magnitude of cross products either
        dot[0] = v1.crossProduct(v2);
        zeroDot[0] = DiMath::RealEqual(dot[0], 0.0f, 1e-3);


        v1 = c - b;
        v2 = p - b;

        dot[1] = v1.crossProduct(v2);
        zeroDot[1] = DiMath::RealEqual(dot[1], 0.0f, 1e-3);

        // Compare signs (ignore colinear / coincident points)
        if(!zeroDot[0] && !zeroDot[1] 
        && DiMath::Sign(dot[0]) != DiMath::Sign(dot[1]))
        {
            return false;
        }

        v1 = a - c;
        v2 = p - c;

        dot[2] = v1.crossProduct(v2);
        zeroDot[2] = DiMath::RealEqual(dot[2], 0.0f, 1e-3);
        // Compare signs (ignore colinear / coincident points)
        if((!zeroDot[0] && !zeroDot[2] 
        && DiMath::Sign(dot[0]) != DiMath::Sign(dot[2])) ||
            (!zeroDot[1] && !zeroDot[2] 
        && DiMath::Sign(dot[1]) != DiMath::Sign(dot[2])))
        {
            return false;
        }


        return true;
    }

    bool DiMath::pointInTri3D( const DiVec3& p, const DiVec3& a,
        const DiVec3& b, const DiVec3& c, const DiVec3& normal )
    {
        DiVec3 v1, v2;
        float dot[3];
        bool zeroDot[3];

        v1 = b - a;
        v2 = p - a;

        // Note we don't care about normalisation here since sign is all we need
        // It means we don't have to worry about magnitude of cross products either
        dot[0] = v1.crossProduct(v2).dotProduct(normal);
        zeroDot[0] = DiMath::RealEqual(dot[0], 0.0f, 1e-3);


        v1 = c - b;
        v2 = p - b;

        dot[1] = v1.crossProduct(v2).dotProduct(normal);
        zeroDot[1] = DiMath::RealEqual(dot[1], 0.0f, 1e-3);

        // Compare signs (ignore colinear / coincident points)
        if(!zeroDot[0] && !zeroDot[1] 
        && DiMath::Sign(dot[0]) != DiMath::Sign(dot[1]))
        {
            return false;
        }

        v1 = a - c;
        v2 = p - c;

        dot[2] = v1.crossProduct(v2).dotProduct(normal);
        zeroDot[2] = DiMath::RealEqual(dot[2], 0.0f, 1e-3);
        // Compare signs (ignore colinear / coincident points)
        if((!zeroDot[0] && !zeroDot[2] 
        && DiMath::Sign(dot[0]) != DiMath::Sign(dot[2])) ||
            (!zeroDot[1] && !zeroDot[2] 
        && DiMath::Sign(dot[1]) != DiMath::Sign(dot[2])))
        {
            return false;
        }


        return true;
    }


    DiPair<bool, float> DiMath::intersects( const DiRay& ray, const DiPlane& plane )
    {
        float denom = plane.normal.dotProduct(ray.getDirection());
        if (DiMath::Abs(denom) < std::numeric_limits<float>::epsilon())
        {
            // Parallel
            return DiPair<bool, float>(false, 0);
        }
        else
        {
            float nom = plane.normal.dotProduct(ray.getOrigin()) + plane.d;
            float t = -(nom/denom);
            return DiPair<bool, float>(t >= 0, t);
        }
    }

    DiPair<bool, float> DiMath::intersects( const DiRay& ray, 
        const DiSphere& sphere, bool discardInside /*= true*/ )
    {
        const DiVec3& raydir = ray.getDirection();
        // Adjust ray origin relative to sphere center
        const DiVec3& rayorig = ray.getOrigin() - sphere.getCenter();
        float radius = sphere.getRadius();

        // Check origin inside first
        if (rayorig.squaredLength() <= radius*radius && discardInside)
        {
            return DiPair<bool, float>(true, 0);
        }

        // Mmm, quadratics
        // Build coeffs which can be used with std quadratic solver
        // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
        float a = raydir.dotProduct(raydir);
        float b = 2 * rayorig.dotProduct(raydir);
        float c = rayorig.dotProduct(rayorig) - radius*radius;

        // Calc determinant
        float d = (b*b) - (4 * a * c);
        if (d < 0)
        {
            // No intersection
            return DiPair<bool, float>(false, 0);
        }
        else
        {
            // BTW, if d=0 there is one intersection, if d > 0 there are 2
            // But we only want the closest one, so that's ok, just use the 
            // '-' version of the solver
            float t = ( -b - DiMath::Sqrt(d) ) / (2 * a);
            if (t < 0)
                t = ( -b + DiMath::Sqrt(d) ) / (2 * a);
            return DiPair<bool, float>(true, t);
        }
    }

    DiPair<bool, float> DiMath::intersects( const DiRay& ray, const DiAABB& box )
    {
        if (box.IsNull()) return DiPair<bool, float>(false, 0);
        if (box.IsInfinite()) return DiPair<bool, float>(true, 0);

        float lowt = 0.0f;
        float t;
        bool hit = false;
        DiVec3 hitpoint;
        const DiVec3& min = box.GetMinimum();
        const DiVec3& max = box.GetMaximum();
        const DiVec3& rayorig = ray.getOrigin();
        const DiVec3& raydir = ray.getDirection();

        // Check origin inside first
        if ( rayorig > min && rayorig < max )
        {
            return DiPair<bool, float>(true, 0);
        }

        // Check each face in turn, only check closest 3
        // Min x
        if (rayorig.x <= min.x && raydir.x > 0)
        {
            t = (min.x - rayorig.x) / raydir.x;
            if (t >= 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max x
        if (rayorig.x >= max.x && raydir.x < 0)
        {
            t = (max.x - rayorig.x) / raydir.x;
            if (t >= 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Min y
        if (rayorig.y <= min.y && raydir.y > 0)
        {
            t = (min.y - rayorig.y) / raydir.y;
            if (t >= 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max y
        if (rayorig.y >= max.y && raydir.y < 0)
        {
            t = (max.y - rayorig.y) / raydir.y;
            if (t >= 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Min z
        if (rayorig.z <= min.z && raydir.z > 0)
        {
            t = (min.z - rayorig.z) / raydir.z;
            if (t >= 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max z
        if (rayorig.z >= max.z && raydir.z < 0)
        {
            t = (max.z - rayorig.z) / raydir.z;
            if (t >= 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }

        return DiPair<bool, float>(hit, lowt);
    }

    bool DiMath::intersects( const DiRay& ray, const DiAABB& box, float* d1, float* d2 )
    {
        if (box.IsNull())
            return false;

        if (box.IsInfinite())
        {
            if (d1) *d1 = 0;
            if (d2) *d2 = DiMath::POS_INFINITY;
            return true;
        }

        const DiVec3& min = box.GetMinimum();
        const DiVec3& max = box.GetMaximum();
        const DiVec3& rayorig = ray.getOrigin();
        const DiVec3& raydir = ray.getDirection();

        DiVec3 absDir;
        absDir[0] = DiMath::Abs(raydir[0]);
        absDir[1] = DiMath::Abs(raydir[1]);
        absDir[2] = DiMath::Abs(raydir[2]);

        // Sort the axis, ensure check minimise floating error axis first
        int imax = 0, imid = 1, imin = 2;
        if (absDir[0] < absDir[2])
        {
            imax = 2;
            imin = 0;
        }
        if (absDir[1] < absDir[imin])
        {
            imid = imin;
            imin = 1;
        }
        else if (absDir[1] > absDir[imax])
        {
            imid = imax;
            imax = 1;
        }

        float start = 0, end = DiMath::POS_INFINITY;

#define _CALC_AXIS(i)                                       \
    do {                                                    \
    float denom = 1 / raydir[i];                         \
    float newstart = (min[i] - rayorig[i]) * denom;      \
    float newend = (max[i] - rayorig[i]) * denom;        \
    if (newstart > newend) std::swap(newstart, newend); \
    if (newstart > end || newend < start) return false; \
    if (newstart > start) start = newstart;             \
    if (newend < end) end = newend;                     \
    } while(0)

        // Check each axis in turn

        _CALC_AXIS(imax);

        if (absDir[imid] < std::numeric_limits<float>::epsilon())
        {
            // Parallel with middle and minimise axis, check bounds only
            if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
                rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
                return false;
        }
        else
        {
            _CALC_AXIS(imid);

            if (absDir[imin] < std::numeric_limits<float>::epsilon())
            {
                // Parallel with minimise axis, check bounds only
                if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
                    return false;
            }
            else
            {
                _CALC_AXIS(imin);
            }
        }
#undef _CALC_AXIS

        if (d1) *d1 = start;
        if (d2) *d2 = end;

        return true;
    }

    DiPair<bool, float> DiMath::intersects( const DiRay& ray, const DiVec3& a, 
        const DiVec3& b, const DiVec3& c,
        const DiVec3& normal, bool positiveSide /*= true*/,
        bool negativeSide /*= true*/ )
    {
        float t;
        {
            float denom = normal.dotProduct(ray.getDirection());

            // Check intersect side
            if (denom > + std::numeric_limits<float>::epsilon())
            {
                if (!negativeSide)
                    return DiPair<bool, float>(false, 0);
            }
            else if (denom < - std::numeric_limits<float>::epsilon())
            {
                if (!positiveSide)
                    return DiPair<bool, float>(false, 0);
            }
            else
            {
                // Parallel or triangle area is close to zero when
                // the plane normal not normalised.
                return DiPair<bool, float>(false, 0);
            }

            t = normal.dotProduct(a - ray.getOrigin()) / denom;

            if (t < 0)
            {
                // Intersection is behind origin
                return DiPair<bool, float>(false, 0);
            }
        }

        //
        // Calculate the largest area projection plane in X, Y or Z.
        //
        size_t i0, i1;
        {
            float n0 = DiMath::Abs(normal[0]);
            float n1 = DiMath::Abs(normal[1]);
            float n2 = DiMath::Abs(normal[2]);

            i0 = 1; i1 = 2;
            if (n1 > n2)
            {
                if (n1 > n0) i0 = 0;
            }
            else
            {
                if (n2 > n0) i1 = 0;
            }
        }

        //
        // Check the intersection point is inside the triangle.
        //
        {
            float u1 = b[i0] - a[i0];
            float v1 = b[i1] - a[i1];
            float u2 = c[i0] - a[i0];
            float v2 = c[i1] - a[i1];
            float u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];
            float v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

            float alpha = u0 * v2 - u2 * v0;
            float beta  = u1 * v0 - u0 * v1;
            float area  = u1 * v2 - u2 * v1;

            // epsilon to avoid float precision error
            const float EPSILON = 1e-6f;

            float tolerance = - EPSILON * area;

            if (area > 0)
            {
                if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
                    return DiPair<bool, float>(false, 0);
            }
            else
            {
                if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
                    return DiPair<bool, float>(false, 0);
            }
        }

        return DiPair<bool, float>(true, t);
    }

    DiPair<bool, float> DiMath::intersects( const DiRay& ray, const DiVec3& a, 
        const DiVec3& b, const DiVec3& c, 
        bool positiveSide /*= true*/, bool negativeSide /*= true*/ )
    {
        DiVec3 normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
        return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
    }

    bool DiMath::intersects(const DiSphere& sphere, const DiAABB& box)
    {
        if (box.IsNull()) return false;
        if (box.IsInfinite()) return true;

        // Use splitting planes
        const DiVec3& center = sphere.getCenter();
        float radius = sphere.getRadius();
        const DiVec3& min = box.GetMinimum();
        const DiVec3& max = box.GetMaximum();

        // Arvo's algorithm
        float s, d = 0;
        for (int i = 0; i < 3; ++i)
        {
            if (center.ptr()[i] < min.ptr()[i])
            {
                s = center.ptr()[i] - min.ptr()[i];
                d += s * s; 
            }
            else if(center.ptr()[i] > max.ptr()[i])
            {
                s = center.ptr()[i] - max.ptr()[i];
                d += s * s; 
            }
        }
        return d <= radius * radius;

    }

    bool DiMath::intersects(const DiPlane& plane, const DiAABB& box)
    {
        return (plane.getSide(box) == DiPlane::BOTH_SIDE);
    }

    bool DiMath::intersects(const DiSphere& sphere, const DiPlane& plane)
    {
        return (
            DiMath::Abs(plane.getDistance(sphere.getCenter()))
            <= sphere.getRadius() );
    }

    DiPair<bool, float> DiMath::intersects( const DiRay& ray, 
        const DiVector<DiPlane>& planes, 
        bool normalIsOutside )
    {
        DiList<DiPlane> planesList;
        for (DiVector<DiPlane>::const_iterator i = planes.begin(); i != planes.end(); ++i)
        {
            planesList.push_back(*i);
        }
        return intersects(ray, planesList, normalIsOutside);
    }

    DiPair<bool, float> DiMath::intersects( const DiRay& ray, 
        const DiList<DiPlane>& planes,
        bool normalIsOutside )
    {
        DiList<DiPlane>::const_iterator planeit, planeitend;
        planeitend = planes.end();
        bool allInside = true;
        DiPair<bool, float> ret;
        DiPair<bool, float> end;
        ret.first = false;
        ret.second = 0.0f;
        end.first = false;
        end.second = 0;

        DiPlane::Side outside = normalIsOutside ? DiPlane::POSITIVE_SIDE : DiPlane::NEGATIVE_SIDE;

        for (planeit = planes.begin(); planeit != planeitend; ++planeit)
        {
            const DiPlane& plane = *planeit;
            // is origin outside?
            if (plane.getSide(ray.getOrigin()) == outside)
            {
                allInside = false;
                // Test single plane
                DiPair<bool, float> planeRes = 
                    ray.intersects(plane);
                if (planeRes.first)
                {
                    // Ok, we intersected
                    ret.first = true;
                    // Use the most distant result since convex volume
                    ret.second = DiMath::Max(ret.second, planeRes.second);
                }
                else
                {
                    ret.first =false;
                    ret.second=0.0f;
                    return ret;
                }
            }
            else
            {
                DiPair<bool, float> planeRes = 
                    ray.intersects(plane);
                if (planeRes.first)
                {
                    if( !end.first )
                    {
                        end.first = true;
                        end.second = planeRes.second;
                    }
                    else
                    {
                        end.second = DiMath::Min( planeRes.second, end.second );
                    }

                }

            }
        }

        if (allInside)
        {
            ret.first = true;
            ret.second = 0.0f;
            return ret;
        }

        if( end.first )
        {
            if( end.second < ret.second )
            {
                ret.first = false;
                return ret;
            }
        }
        return ret;
    }

    DiVec3 DiMath::calculateTangentSpaceVector( const DiVec3& position1,
        const DiVec3& position2, 
        const DiVec3& position3, 
        float u1, float v1, float u2,
        float v2, float u3, float v3 )
    {
        DiVec3 side0 = position1 - position2;
        DiVec3 side1 = position3 - position1;
        //Calculate face normal
        DiVec3 normal = side1.crossProduct(side0);
        normal.normalise();
        //Now we use a formula to calculate the tangent. 
        float deltaV0 = v1 - v2;
        float deltaV1 = v3 - v1;
        DiVec3 tangent = deltaV1 * side0 - deltaV0 * side1;
        tangent.normalise();
        //Calculate binormal
        float deltaU0 = u1 - u2;
        float deltaU1 = u3 - u1;
        DiVec3 binormal = deltaU1 * side0 - deltaU0 * side1;
        binormal.normalise();

        DiVec3 tangentCross = tangent.crossProduct(binormal);
        if (tangentCross.dotProduct(normal) < 0.0f)
        {
            tangent = -tangent;
            binormal = -binormal;
        }

        return tangent;
    }

    DiMat4 DiMath::buildReflectionMatrix( const DiPlane& p )
    {
        return DiMat4(
            -2 * p.normal.x * p.normal.x + 1,   -2 * p.normal.x * p.normal.y,       -2 * p.normal.x * p.normal.z,       -2 * p.normal.x * p.d, 
            -2 * p.normal.y * p.normal.x,       -2 * p.normal.y * p.normal.y + 1,   -2 * p.normal.y * p.normal.z,       -2 * p.normal.y * p.d, 
            -2 * p.normal.z * p.normal.x,       -2 * p.normal.z * p.normal.y,       -2 * p.normal.z * p.normal.z + 1,   -2 * p.normal.z * p.d, 
            0,                                  0,                                  0,                                  1);
    }

    DiVec4 DiMath::calculateFaceNormal(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3)
    {
        DiVec3 normal = calculateBasicFaceNormal(v1, v2, v3);
        return DiVec4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
    }

    DiVec3 DiMath::calculateBasicFaceNormal(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3)
    {
        DiVec3 normal = (v2 - v1).crossProduct(v3 - v1);
        normal.normalise();
        return normal;
    }

    DiVec4 DiMath::calculateFaceNormalWithoutNormalize(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3)
    {
        DiVec3 normal = calculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
        return DiVec4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
    }

    DiVec3 DiMath::calculateBasicFaceNormalWithoutNormalize(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3)
    {
        DiVec3 normal = (v2 - v1).crossProduct(v3 - v1);
        return normal;
    }

    float DiMath::gaussianDistribution( float x, float offset /*= 0.0f*/, float scale /*= 1.0f*/ )
    {
        float nom = DiMath::Exp(
            -DiMath::Sqr(x - offset) / (2 * DiMath::Sqr(scale)));
        float denom = scale * DiMath::Sqrt(2 * DiMath::PI);

        return nom / denom;
    }

    DiMat4 DiMath::makeViewMatrix( const DiVec3& position,
        const DiQuat& orientation, 
        const DiMat4* reflectMatrix /*= 0*/ )
    {
        DiMat4 viewMatrix;

        // View matrix is:
        //
        //  [ Lx  Uy  Dz  Tx  ]
        //  [ Lx  Uy  Dz  Ty  ]
        //  [ Lx  Uy  Dz  Tz  ]
        //  [ 0   0   0   1   ]
        //
        // Where T = -(Transposed(Rot) * Pos)

        // This is most efficiently done using 3x3 Matrices
        DiMat3 rot;
        orientation.ToRotationMatrix(rot);

        // Make the translation relative to DI_NEW axes
        DiMat3 rotT = rot.Transpose();
        DiVec3 trans = -rotT * position;

        // Make final matrix
        viewMatrix = DiMat4::IDENTITY;
        viewMatrix = rotT; // fills upper 3x3
        viewMatrix[0][3] = trans.x;
        viewMatrix[1][3] = trans.y;
        viewMatrix[2][3] = trans.z;

        // Deal with reflections
        if (reflectMatrix)
        {
            viewMatrix = viewMatrix * (*reflectMatrix);
        }

        return viewMatrix;
    }

    int DiMath::ISign( int iValue )
    {
        return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
    }

    float DiMath::ACos( float fValue )
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return (acos(fValue));
            else
                return (0.0);
        }
        else
        {
            return (PI);
        }
    }

    float DiMath::ASin( float fValue )
    {
        if ( -1.0 < fValue )
        {
            if ( fValue < 1.0 )
                return (asin(fValue));
            else
                return (HALF_PI);
        }
        else
        {
            return (-HALF_PI);
        }
    }

    float DiMath::AngleUnitsToRadians( float angleunits )
    {
        if (sAngleUnit == AU_DEGREE)
        {
            return angleunits * _DEG_TO_RAD;
        }
        else
        {
            return angleunits;
        }
    }

    float DiMath::RadiansToAngleUnits( float radians )
    {
        if (sAngleUnit == AU_DEGREE)
        {
            return radians * _RAD_TO_DEG;
        }
        else
        {
            return radians;
        }
    }

    float DiMath::AngleUnitsToDegrees( float angleunits )
    {
        if (sAngleUnit == AU_RADIAN)
        {
            return angleunits * _RAD_TO_DEG;
        }
        else
        {
            return angleunits;
        }
    }

    float DiMath::DegreesToAngleUnits( float degrees )
    {
        if (sAngleUnit == AU_RADIAN)
        {
            return degrees * _DEG_TO_RAD;
        }
        else
        {
            return degrees;
        }
    }

    DiMath::DiMath()
    {
        sAngleUnit = AU_DEGREE;
    }

    void DiMath::SetAngleUnit( AngleUnit unit )
    {
        sAngleUnit = unit;
    }

    Demi::AngleUnit DiMath::GetAngleUnit( void )
    {
        return sAngleUnit;
    }

    float DiMath::BoundingRadiusFromAABB( const DiAABB& aabb )
    {
        DiVec3 max = aabb.GetMaximum();
        DiVec3 min = aabb.GetMinimum();

        DiVec3 magnitude = max;
        magnitude.makeCeil(-max);
        magnitude.makeCeil(min);
        magnitude.makeCeil(-min);

        return magnitude.length();
    }

    bool DiMath::isNaN( float f )
    {
        return f != f;
    }

    float DiMath::PointToLineDistance2D( const DiVec3 &p1,const DiVec3 &p2,const DiVec3 &p3 )
    {
        float dx = p2.x-p1.x;
        float dy = p2.y-p1.y;
        if (dx+dy == 0)
        {
            return (float)sqrt( (p3.x-p1.x)*(p3.x-p1.x) + (p3.y-p1.y)*(p3.y-p1.y) );
        }
        float u = ((p3.x-p1.x)*dx + (p3.y-p1.y)*dy) / (dx*dx + dy*dy);
        if (u < 0)
        {
            return (float)sqrt( (p3.x-p1.x)*(p3.x-p1.x) + (p3.y-p1.y)*(p3.y-p1.y) );
        }
        else if (u > 1)
        {
            return (float)sqrt( (p3.x-p2.x)*(p3.x-p2.x) + (p3.y-p2.y)*(p3.y-p2.y) );
        }
        else
        {
            float x = p1.x + u*dx;
            float y = p1.y + u*dy;
            return (float)sqrt( (p3.x-x)*(p3.x-x) + (p3.y-y)*(p3.y-y) );
        }
    }

    bool DiMath::LineLineIntersect( const DiVec3 &p1,const DiVec3 &p2,const DiVec3 &p3,const DiVec3 &p4,
        DiVec3 &pa,DiVec3 &pb,float &mua,float &mub )
    {
        DiVec3 p13,p43,p21;
        float d1343,d4321,d1321,d4343,d2121;
        float numer,denom;

        p13.x = p1.x - p3.x;
        p13.y = p1.y - p3.y;
        p13.z = p1.z - p3.z;
        p43.x = p4.x - p3.x;
        p43.y = p4.y - p3.y;
        p43.z = p4.z - p3.z;
        if (fabs(p43.x)  < LINE_EPS && fabs(p43.y)  < LINE_EPS && fabs(p43.z)  < LINE_EPS)
        {
            return false;
        }
        p21.x = p2.x - p1.x;
        p21.y = p2.y - p1.y;
        p21.z = p2.z - p1.z;
        if (fabs(p21.x)  < LINE_EPS && fabs(p21.y)  < LINE_EPS && fabs(p21.z)  < LINE_EPS)
        {
            return false;
        }

        d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
        d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
        d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
        d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
        d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

        denom = d2121 * d4343 - d4321 * d4321;
        if (fabs(denom) < LINE_EPS)
        {
            return false;
        }
        numer = d1343 * d4321 - d1321 * d4343;

        mua = numer / denom;
        mub = (d1343 + d4321 * (mua)) / d4343;

        pa.x = p1.x + mua * p21.x;
        pa.y = p1.y + mua * p21.y;
        pa.z = p1.z + mua * p21.z;
        pb.x = p3.x + mub * p43.x;
        pb.y = p3.y + mub * p43.y;
        pb.z = p3.z + mub * p43.z;

        return true;
    }

    bool DiMath::RayAABBEdgeIntersect( const DiVec3 &raySrc,const DiVec3 &rayDir, const DiAABB& aabb, float epsilonDist,float &dist,DiVec3 &intPnt )
    {
        DiVec3 rayTrg = raySrc + rayDir*10000.0f;
        DiVec3 pnt[12];

        float d[12];

        // Near
        d[0] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMinimum().y,aabb.GetMaximum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMinimum().y,aabb.GetMaximum().z),pnt[0] );
        d[1] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMaximum().y,aabb.GetMaximum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMaximum().y,aabb.GetMaximum().z),pnt[1] );
        d[2] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMinimum().y,aabb.GetMaximum().z),
            DiVec3(aabb.GetMinimum().x,aabb.GetMaximum().y,aabb.GetMaximum().z),pnt[2] );
        d[3] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMaximum().x,aabb.GetMinimum().y,aabb.GetMaximum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMaximum().y,aabb.GetMaximum().z),pnt[3] );

        // Far
        d[4] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMinimum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMinimum().y,aabb.GetMinimum().z),pnt[4] );
        d[5] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMaximum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMaximum().y,aabb.GetMinimum().z),pnt[5] );
        d[6] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMinimum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMinimum().x,aabb.GetMaximum().y,aabb.GetMinimum().z),pnt[6] );
        d[7] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMaximum().x,aabb.GetMinimum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMaximum().y,aabb.GetMinimum().z),pnt[7] );

        // Sides.
        d[8] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMinimum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMinimum().x,aabb.GetMinimum().y,aabb.GetMaximum().z),pnt[8] );
        d[9] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMaximum().x,aabb.GetMinimum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMinimum().y,aabb.GetMaximum().z),pnt[9] );
        d[10] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMinimum().x,aabb.GetMaximum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMinimum().x,aabb.GetMaximum().y,aabb.GetMaximum().z),pnt[10] );
        d[11] = RayToLineDistance( raySrc,rayTrg,DiVec3(aabb.GetMaximum().x,aabb.GetMaximum().y,aabb.GetMinimum().z),
            DiVec3(aabb.GetMaximum().x,aabb.GetMaximum().y,aabb.GetMaximum().z),pnt[11] );

        dist = FLT_MAX;
        for (int i = 0; i < 12; i++)
        {
            if (d[i] < dist)
            {
                dist = d[i];
                intPnt = pnt[i];
            }
        }
        if (dist < epsilonDist)
        {
            return true;
        }
        return false;
    }
}

#if (DEMI_COMPILER == DEMI_COMPILER_MSVC)
#   pragma warning(pop)
#endif

