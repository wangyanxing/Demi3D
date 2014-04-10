/**********************************************************************
This source file is a part of Demi3D
  __  ___  __  __  __
  |  \|_ |\/||   _)|  \
  |__/|__|  ||  __)|__/

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
**********************************************************************/

#ifndef DiMathMisc_h
#define DiMathMisc_h

#include "MiscPrerequisites.h"
#include "Assert.h"
#include "Vec.h"
#include "Pair.h"
#include "List.h"

//#include <xutility>
#include <math.h>
#include <limits>

namespace Demi 
{
    enum AngleUnit
    {
        AU_DEGREE,
        AU_RADIAN
    };

#define DEG2RAD( a ) ( (a) * (DiMath::PI/180.0f) )
#define RAD2DEG( a ) ( (a) * (180.0f/DiMath::PI) )

    class DI_MISC_API DiRadian
    {
        float mRad;

    public:
        explicit DiRadian ( float r=0 ) : mRad(r) {}
        DiRadian ( const DiDegree& d );
        DiRadian& operator = ( const float& f ) { mRad = f; return *this; }
        DiRadian& operator = ( const DiRadian& r ) { mRad = r.mRad; return *this; }
        DiRadian& operator = ( const DiDegree& d );

        float valueDegrees() const;
        float valueRadians() const { return mRad; }
        float valueAngleUnits() const;

        const DiRadian& operator + () const { return *this; }
        DiRadian operator + ( const DiRadian& r ) const { return DiRadian ( mRad + r.mRad ); }
        DiRadian operator + ( const DiDegree& d ) const;
        DiRadian& operator += ( const DiRadian& r ) { mRad += r.mRad; return *this; }
        DiRadian& operator += ( const DiDegree& d );
        DiRadian operator - () const { return DiRadian(-mRad); }
        DiRadian operator - ( const DiRadian& r ) const { return DiRadian ( mRad - r.mRad ); }
        DiRadian operator - ( const DiDegree& d ) const;
        DiRadian& operator -= ( const DiRadian& r ) { mRad -= r.mRad; return *this; }
        DiRadian& operator -= ( const DiDegree& d );
        DiRadian operator * ( float f ) const { return DiRadian ( mRad * f ); }
        DiRadian operator * ( const DiRadian& f ) const { return DiRadian ( mRad * f.mRad ); }
        DiRadian& operator *= ( float f ) { mRad *= f; return *this; }
        DiRadian operator / ( float f ) const { return DiRadian ( mRad / f ); }
        DiRadian& operator /= ( float f ) { mRad /= f; return *this; }

        bool operator <  ( const DiRadian& r ) const { return mRad <  r.mRad; }
        bool operator <= ( const DiRadian& r ) const { return mRad <= r.mRad; }
        bool operator == ( const DiRadian& r ) const { return mRad == r.mRad; }
        bool operator != ( const DiRadian& r ) const { return mRad != r.mRad; }
        bool operator >= ( const DiRadian& r ) const { return mRad >= r.mRad; }
        bool operator >  ( const DiRadian& r ) const { return mRad >  r.mRad; }
    };

    class DI_MISC_API DiDegree
    {
        float mDeg;

    public:
        explicit DiDegree ( float d=0 ) : mDeg(d) {}
        DiDegree ( const DiRadian& r ) : mDeg(r.valueDegrees()) {}
        DiDegree& operator = ( const float& f ) { mDeg = f; return *this; }
        DiDegree& operator = ( const DiDegree& d ) { mDeg = d.mDeg; return *this; }
        DiDegree& operator = ( const DiRadian& r ) { mDeg = r.valueDegrees(); return *this; }

        float valueDegrees() const { return mDeg; }
        float valueRadians() const; // see bottom of this file
        float valueAngleUnits() const;

        const DiDegree& operator + () const { return *this; }
        DiDegree operator + ( const DiDegree& d ) const { return DiDegree ( mDeg + d.mDeg ); }
        DiDegree operator + ( const DiRadian& r ) const { return DiDegree ( mDeg + r.valueDegrees() ); }
        DiDegree& operator += ( const DiDegree& d ) { mDeg += d.mDeg; return *this; }
        DiDegree& operator += ( const DiRadian& r ) { mDeg += r.valueDegrees(); return *this; }
        DiDegree operator - () const { return DiDegree(-mDeg); }
        DiDegree operator - ( const DiDegree& d ) const { return DiDegree ( mDeg - d.mDeg ); }
        DiDegree operator - ( const DiRadian& r ) const { return DiDegree ( mDeg - r.valueDegrees() ); }
        DiDegree& operator -= ( const DiDegree& d ) { mDeg -= d.mDeg; return *this; }
        DiDegree& operator -= ( const DiRadian& r ) { mDeg -= r.valueDegrees(); return *this; }
        DiDegree operator * ( float f ) const { return DiDegree ( mDeg * f ); }
        DiDegree operator * ( const DiDegree& f ) const { return DiDegree ( mDeg * f.mDeg ); }
        DiDegree& operator *= ( float f ) { mDeg *= f; return *this; }
        DiDegree operator / ( float f ) const { return DiDegree ( mDeg / f ); }
        DiDegree& operator /= ( float f ) { mDeg /= f; return *this; }

        bool operator <  ( const DiDegree& d ) const { return mDeg <  d.mDeg; }
        bool operator <= ( const DiDegree& d ) const { return mDeg <= d.mDeg; }
        bool operator == ( const DiDegree& d ) const { return mDeg == d.mDeg; }
        bool operator != ( const DiDegree& d ) const { return mDeg != d.mDeg; }
        bool operator >= ( const DiDegree& d ) const { return mDeg >= d.mDeg; }
        bool operator >  ( const DiDegree& d ) const { return mDeg >  d.mDeg; }
    };

    class DI_MISC_API DiAngle
    {
        float mAngle;
    public:
        explicit DiAngle ( float angle ) : mAngle(angle) {}
        operator DiRadian() const;
        operator DiDegree() const;
    };

    inline DiRadian::DiRadian ( const DiDegree& d ) : mRad(d.valueRadians()) 
    {
    }
    inline DiRadian& DiRadian::operator = ( const DiDegree& d ) 
    {
        mRad = d.valueRadians(); return *this;
    }
    inline DiRadian DiRadian::operator + ( const DiDegree& d ) const 
    {
        return DiRadian ( mRad + d.valueRadians() );
    }
    inline DiRadian& DiRadian::operator += ( const DiDegree& d )
    {
        mRad += d.valueRadians();
        return *this;
    }
    inline DiRadian DiRadian::operator - ( const DiDegree& d ) const
    {
        return DiRadian ( mRad - d.valueRadians() );
    }
    inline DiRadian& DiRadian::operator -= ( const DiDegree& d )
    {
        mRad -= d.valueRadians();
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////

    struct DI_MISC_API DiMath
    {
        DiMath();

        static bool isNaN(float f);

        static inline float Max(float a, float b)
        {
            return (a > b) ? a : b;
        }

        static inline double Max(double a, double b)
        {
            return (a > b) ? a : b;
        }

        static inline int Max(int a, int b)
        {
            return (a > b) ? a : b;
        }

        static inline int Max(ulong a, ulong b)
        {
            return (a > b) ? a : b;
        }

        static inline float Min(float a, float b)
        {
            return (a < b) ? a : b;
        }

        static inline double Min(double a, double b)
        {
            return (a < b) ? a : b;
        }

        static inline int Min(int a, int b)
        {
            return (a < b) ? a : b;
        }

        static inline int Min(uint32 a, uint32 b)
        {
            return (a < b) ? a : b;
        }

        static inline int Min(ulong a, ulong b)
        {
            return (a < b) ? a : b;
        }

        template<typename T>
        static inline bool IsPO2(T n)
        {
            return (n & (n-1)) == 0;
        }

        static inline uint32 FirstPO2From(uint32 n)
        {
            --n;            
            n |= n >> 16;
            n |= n >> 8;
            n |= n >> 4;
            n |= n >> 2;
            n |= n >> 1;
            ++n;
            return n;
        }

        static inline float Log2 (float fValue) 
        {
            return float(log(fValue)/LOG2); 
        }

        template <typename T>
        static T Clamp(T val, T minval, T maxval)
        {
            DI_ASSERT (minval < maxval);
            return std::max<T>(std::min<T>(val, maxval), minval);
        }

        static bool RealEqual(float a, float b,
            float tolerance = 0.00001f);

        static inline float Pow (float fBase, float fExponent) { return float(pow(fBase,fExponent)); }

        static float Sign (float fValue);

        static inline DiRadian Sign ( const DiRadian& rValue )
        {
            return DiRadian(Sign(rValue.valueRadians()));
        }

        static inline DiDegree Sign ( const DiDegree& dValue )
        {
            return DiDegree(Sign(dValue.valueDegrees()));
        }

        static inline float Sin (const DiRadian& fValue) 
        {
            return sin(fValue.valueRadians());
        }

        static inline float Sin (float fValue) 
        {
            return sin(fValue);
        }

        static inline float Cos (const DiRadian& fValue) 
        {
            return cos(fValue.valueRadians());
        }

        static inline float Cos (float fValue) 
        {
            return cos(fValue);
        }

        static inline void SinCos(float angle, float* pSin, float* pCos)
        {
            *pSin = float(Sin(angle));    
            *pCos = float(Cos(angle));
        }

        static inline int CeilPow2(unsigned int in) {
            in -= 1;

            in |= in >> 16;
            in |= in >> 8;
            in |= in >> 4;
            in |= in >> 2;
            in |= in >> 1;

            return in + 1;
        }

        static inline float Sqr (float fValue) { return fValue*fValue; }

        static inline float Sqrt (float fValue) { return float(sqrt(fValue)); }

        static inline DiRadian Sqrt (const DiRadian& fValue) { return DiRadian(sqrt(fValue.valueRadians())); }

        static inline DiDegree Sqrt (const DiDegree& fValue) { return DiDegree(sqrt(fValue.valueDegrees())); }

        static int    IntRound(double f)
        {
            return f < 0.0 ? int(f - 0.5) : int(f + 0.5);
        }

        static double Round( double fVal, double fStep )
        {
            if (fStep > 0.f)
                fVal = IntRound(fVal / fStep) * fStep;
            return fVal;
        }

        static float InvSqrt(float fValue);

        static float UnitRandom ();  // in [0,1]

        static float RangeRandom (float fLow, float fHigh);  // in [fLow,fHigh]

        static float SymmetricRandom ();  // in [-1,1]

        static inline float Tan (const DiRadian& fValue) 
        {
            return tan(fValue.valueRadians()) ;
        }

        static inline float Tan (float fValue) 
        {
            return tan(fValue);
        }

        static inline float DegreesToRadians(float degrees) { return degrees * _DEG_TO_RAD; }

        static inline float RadiansToDegrees(float radians) { return radians * _RAD_TO_DEG; }

        static bool pointInTri2D(const DiVec2& p, const DiVec2& a, 
            const DiVec2& b, const DiVec2& c);

        static bool pointInTri3D(const DiVec3& p, const DiVec3& a, 
            const DiVec3& b, const DiVec3& c, const DiVec3& normal);

        static DiPair<bool, float> intersects(const DiRay& ray, const DiPlane& plane);

        static DiPair<bool, float> intersects(const DiRay& ray, const DiSphere& sphere, 
            bool discardInside = true);

        static DiPair<bool, float> intersects(const DiRay& ray, const DiAABB& box);


        static bool intersects(const DiRay& ray, const DiAABB& box,
            float* d1, float* d2);


        static DiPair<bool, float> intersects(const DiRay& ray, const DiVec3& a,
            const DiVec3& b, const DiVec3& c, const DiVec3& normal,
            bool positiveSide = true, bool negativeSide = true);

        static DiPair<bool, float> intersects(const DiRay& ray, const DiVec3& a,
            const DiVec3& b, const DiVec3& c,
            bool positiveSide = true, bool negativeSide = true);

        static bool intersects(const DiSphere& sphere, const DiAABB& box);

        static bool intersects(const DiPlane& plane, const DiAABB& box);

        static DiPair<bool, float> intersects(
            const DiRay& ray, const DiVector<DiPlane>& planeList, 
            bool normalIsOutside);

        static DiPair<bool, float> intersects(
            const DiRay& ray, const DiList<DiPlane>& planeList, 
            bool normalIsOutside);

        static bool intersects(const DiSphere& sphere, const DiPlane& plane);

        static DiVec3 calculateTangentSpaceVector(
            const DiVec3& position1, const DiVec3& position2, const DiVec3& position3,
            float u1, float v1, float u2, float v2, float u3, float v3);

        static DiMat4 buildReflectionMatrix(const DiPlane& p);

        static DiVec4 calculateFaceNormal(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3);

        static DiVec3 calculateBasicFaceNormal(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3);

        static DiVec4 calculateFaceNormalWithoutNormalize(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3);

        static DiVec3 calculateBasicFaceNormalWithoutNormalize(const DiVec3& v1, const DiVec3& v2, const DiVec3& v3);

        static float gaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

        static DiMat4 makeViewMatrix(const DiVec3& position, const DiQuat& orientation, 
            const DiMat4* reflectMatrix = 0);

        static inline int         IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
        static inline int         ICeil (float fValue) { return int(ceil(fValue)); }
        static inline int         IFloor (float fValue) { return int(floor(fValue)); }
        static int                ISign (int iValue);

        static inline float       Abs (float fValue) { return float(fabs(fValue)); }
        static inline DiDegree    Abs (const DiDegree& dValue) { return DiDegree(fabs(dValue.valueDegrees())); }
        static inline DiRadian    Abs (const DiRadian& rValue) { return DiRadian(fabs(rValue.valueRadians())); }
        static float              ACos (float fValue);
        
        static float              ASin (float fValue);
        
        static inline float       ATan (float fValue) { return (atan(fValue)); }
        
        static inline float       ATan2 (float fY, float fX) { return (atan2(fY,fX)); }
        
        static inline float       Ceil (float fValue) { return float(ceil(fValue)); }
        
        static inline float       Exp (float fValue) { return float(exp(fValue)); }
        
        static inline float       Floor (float fValue) { return float(floor(fValue)); }
        
        static inline float       Log (float fValue) { return float(log(fValue)); }

        static void               SetAngleUnit(AngleUnit unit);
        
        static AngleUnit          GetAngleUnit(void);

        static float              AngleUnitsToRadians(float units);
        
        static float              RadiansToAngleUnits(float radians);
        
        static float              AngleUnitsToDegrees(float units);
        
        static float              DegreesToAngleUnits(float degrees);

        static float              BoundingRadiusFromAABB(const DiAABB& aabb);

        static float              PointToLineDistance2D( const DiVec3 &p1,const DiVec3 &p2,const DiVec3 &p3 );

        static bool               LineLineIntersect( const DiVec3 &p1,const DiVec3 &p2,const DiVec3 &p3,const DiVec3 &p4,
                                  DiVec3 &pa,DiVec3 &pb,float &mua,float &mub );

        static bool               RayAABBEdgeIntersect( const DiVec3 &raySrc,const DiVec3 &rayDir,
                                    const DiAABB& aabb, float epsilonDist,float &dist,DiVec3 &intPnt );

        static inline int32        IsNeg(f64 x) 
        {
            union { f32 f; uint32 i; } u;
            u.f=(f32)x; return (int32)(u.i>>31);
        }
        static inline int32        IsNeg(f32 x) 
        {
            union { f32 f; uint32 i; } u;
            u.f=x; return (int32)(u.i>>31);
        }
        static inline int32        IsNeg(int32 x)
        {
            return (int32)((uint32)x>>31);
        }
        static AngleUnit    sAngleUnit;

        static const float  POS_INFINITY;
        static const float  NEG_INFINITY;
        static const float  PI;
        static const float  TWO_PI;
        static const float  HALF_PI;
        static const float  _DEG_TO_RAD;
        static const float  _RAD_TO_DEG;
        static const float  LOG2;
    };

    inline float DiRadian::valueDegrees() const
    {
        return DiMath::RadiansToDegrees ( mRad );
    }


    inline float DiDegree::valueRadians() const
    {
        return DiMath::DegreesToRadians ( mDeg );
    }

    inline DiRadian operator * ( float a, const DiRadian& b )
    {
        return DiRadian ( a * b.valueRadians() );
    }

    inline DiRadian operator / ( float a, const DiRadian& b )
    {
        return DiRadian ( a / b.valueRadians() );
    }

    inline DiDegree operator * ( float a, const DiDegree& b )
    {
        return DiDegree ( a * b.valueDegrees() );
    }

    inline DiDegree operator / ( float a, const DiDegree& b )
    {
        return DiDegree ( a / b.valueDegrees() );
    }

    inline DiAngle::operator DiRadian() const
    {
        return DiRadian(DiMath::AngleUnitsToRadians(mAngle));
    }

    inline DiAngle::operator DiDegree() const
    {
        return DiDegree(DiMath::AngleUnitsToDegrees(mAngle));
    }

    //////////////////////////////////////////////////////////////////////////

    class DI_MISC_API DiBox
    {
    public:
        uint32 left, top, right, bottom;

        DiBox()
            : left(0), top(0), right(1), bottom(1)
        {
        }
    
        DiBox(uint32 l, uint32 t, uint32 r, uint32 b) :
            left(l),
            top(t),   
            right(r),
            bottom(b)
        {
              DI_ASSERT(right >= left && bottom >= top);
        }
      
        bool    Contains(const DiBox &def) const
        {
            return (def.left >= left && def.top >= top  &&
                def.right <= right && def.bottom <= bottom);
        }
        
        uint32  GetWidth() const { return right-left; }

        uint32  GetHeight() const { return bottom-top; }
    };
    
    template< typename T >
    struct TRect
    {
        T left, top, right, bottom;
        
        TRect() : left(0), top(0), right(0), bottom(0) {}
        
        TRect( T const & l, T const & t, T const & r, T const & b )
        : left( l ), top( t ), right( r ), bottom( b )
        {
        }
        
        TRect( TRect const & o )
        : left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom )
        {
        }
        
        TRect & operator=( TRect const & o )
        {
            left = o.left;
            top = o.top;
            right = o.right;
            bottom = o.bottom;
            return *this;
        }
        
        T Width() const
        {
            return right - left;
        }
        
        T Height() const
        {
            return bottom - top;
        }
        
        bool IsNull() const
        {
            return Width() == 0 || Height() == 0;
        }
        
        void SetNull()
        {
            left = right = top = bottom = 0;
        }
        
        TRect & Merge(const TRect& rhs)
        {
            if (IsNull())
            {
                *this = rhs;
            }
            else if (!rhs.IsNull())
            {
                left = DiMath::Min(left, rhs.left);
                right = DiMath::Max(right, rhs.right);
                top = DiMath::Min(top, rhs.top);
                bottom = DiMath::Max(bottom, rhs.bottom);
            }
            
            return *this;
            
        }
        
        TRect Intersect(const TRect& rhs) const
        {
            TRect ret;
            if (IsNull() || rhs.IsNull())
            {
                return ret;
            }
            else
            {
                ret.left = DiMath::Max(left, rhs.left);
                ret.right = DiMath::Min(right, rhs.right);
                ret.top = DiMath::Max(top, rhs.top);
                ret.bottom = DiMath::Min(bottom, rhs.bottom);
            }
            
            if (ret.left > ret.right || ret.top > ret.bottom)
            {
                ret.left = ret.top = ret.right = ret.bottom = 0;
            }
            
            return ret;
        }
    };
    
    
    typedef TRect<float>    DiFloatRect;
    typedef TRect<long>     DiRect;
}

#endif