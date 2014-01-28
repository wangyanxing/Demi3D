
/********************************************************************
File:       vector2.h
Creator:    demiwangya
*********************************************************************/

#pragma once

#include "MiscPrerequisites.h"
#include "float.h"
#include "mathmisc.h"

namespace Demi
{
    class DI_MISC_API DiVec2
    {
    public:
        float x, y;

    public:
        inline DiVec2()
            : x(0.0f), y(0.0f)
        {
        }

        inline DiVec2(const float fX, const float fY )
            : x( fX ), y( fY )
        {
        }

        inline explicit DiVec2( const float scaler )
            : x( scaler), y( scaler )
        {
        }

        inline explicit DiVec2( const float afCoordinate[2] )
            : x( afCoordinate[0] ),
            y( afCoordinate[1] )
        {
        }

        inline explicit DiVec2( const int afCoordinate[2] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
        }

        inline explicit DiVec2( float* const r )
            : x( r[0] ), y( r[1] )
        {
        }

        /** Exchange the contents of this vector with another. 
        */
        inline void swap(DiVec2& other)
        {
            std::swap(x, other.x);
            std::swap(y, other.y);
        }

        inline float operator [] ( const size_t i ) const
        {
            DI_ASSERT( i < 2 );

            return *(&x+i);
        }

        inline float& operator [] ( const size_t i )
        {
            DI_ASSERT( i < 2 );

            return *(&x+i);
        }

        /// Pointer accessor for direct copying
        inline float* ptr()
        {
            return &x;
        }
        /// Pointer accessor for direct copying
        inline const float* ptr() const
        {
            return &x;
        }

        /** Assigns the value of the other vector.
        @param
        rkVector The other vector
        */
        inline DiVec2& operator = ( const DiVec2& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }

        inline DiVec2& operator = ( const float fScalar)
        {
            x = fScalar;
            y = fScalar;

            return *this;
        }

        inline bool operator == ( const DiVec2& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y );
        }

        inline bool operator != ( const DiVec2& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y  );
        }

        // arithmetic operations
        inline DiVec2 operator + ( const DiVec2& rkVector ) const
        {
            return DiVec2(
                x + rkVector.x,
                y + rkVector.y);
        }

        inline DiVec2 operator - ( const DiVec2& rkVector ) const
        {
            return DiVec2(
                x - rkVector.x,
                y - rkVector.y);
        }

        inline DiVec2 operator * ( const float fScalar ) const
        {
            return DiVec2(
                x * fScalar,
                y * fScalar);
        }

        inline DiVec2 operator * ( const DiVec2& rhs) const
        {
            return DiVec2(
                x * rhs.x,
                y * rhs.y);
        }

        inline DiVec2 operator / ( const float fScalar ) const
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return DiVec2(
                x * fInv,
                y * fInv);
        }

        inline DiVec2 operator / ( const DiVec2& rhs) const
        {
            return DiVec2(
                x / rhs.x,
                y / rhs.y);
        }

        inline const DiVec2& operator + () const
        {
            return *this;
        }

        inline DiVec2 operator - () const
        {
            return DiVec2(-x, -y);
        }

        // overloaded operators to help Vector2
        inline DI_MISC_API friend DiVec2 operator * ( const float fScalar, const DiVec2& rkVector )
        {
            return DiVec2(
                fScalar * rkVector.x,
                fScalar * rkVector.y);
        }

        inline DI_MISC_API friend DiVec2 operator / ( const float fScalar, const DiVec2& rkVector )
        {
            return DiVec2(
                fScalar / rkVector.x,
                fScalar / rkVector.y);
        }

        inline DI_MISC_API friend DiVec2 operator + (const DiVec2& lhs, const float rhs)
        {
            return DiVec2(
                lhs.x + rhs,
                lhs.y + rhs);
        }

        inline DI_MISC_API friend DiVec2 operator + (const float lhs, const DiVec2& rhs)
        {
            return DiVec2(
                lhs + rhs.x,
                lhs + rhs.y);
        }

        inline DI_MISC_API friend DiVec2 operator - (const DiVec2& lhs, const float rhs)
        {
            return DiVec2(
                lhs.x - rhs,
                lhs.y - rhs);
        }

        inline DI_MISC_API friend DiVec2 operator - (const float lhs, const DiVec2& rhs)
        {
            return DiVec2(
                lhs - rhs.x,
                lhs - rhs.y);
        }
        // arithmetic updates
        inline DiVec2& operator += ( const DiVec2& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }

        inline DiVec2& operator += ( const float fScaler )
        {
            x += fScaler;
            y += fScaler;

            return *this;
        }

        inline DiVec2& operator -= ( const DiVec2& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline DiVec2& operator -= ( const float fScaler )
        {
            x -= fScaler;
            y -= fScaler;

            return *this;
        }

        inline DiVec2& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;

            return *this;
        }

        inline DiVec2& operator *= ( const DiVec2& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline DiVec2& operator /= ( const float fScalar )
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline DiVec2& operator /= ( const DiVec2& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;

            return *this;
        }

        /** Returns the length (magnitude) of the vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        length (e.g. for just comparing lengths) use squaredLength()
        instead.
        */
        inline float length () const
        {
            return DiMath::Sqrt( x * x + y * y );
        }

        /** Returns the square of the length(magnitude) of the vector.
        @remarks
        This  method is for efficiency - calculating the actual
        length of a vector requires a square root, which is expensive
        in terms of the operations required. This method returns the
        square of the length of the vector, i.e. the same as the
        length but before the square root is taken. Use this if you
        want to find the longest / shortest vector without incurring
        the square root.
        */
        inline float squaredLength () const
        {
            return x * x + y * y;
        }
        /** Returns the distance to another vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        distance (e.g. for just comparing distances) use squaredDistance()
        instead.
        */
        inline float distance(const DiVec2& rhs) const
        {
            return (*this - rhs).length();
        }

        /** Returns the square of the distance to another vector.
        @remarks
        This method is for efficiency - calculating the actual
        distance to another vector requires a square root, which is
        expensive in terms of the operations required. This method
        returns the square of the distance to another vector, i.e.
        the same as the distance but before the square root is taken.
        Use this if you want to find the longest / shortest distance
        without incurring the square root.
        */
        inline float squaredDistance(const DiVec2& rhs) const
        {
            return (*this - rhs).squaredLength();
        }

        /** Calculates the dot (float) product of this vector with another.
        @remarks
        The dot product can be used to calculate the angle between 2
        vectors. If both are unit vectors, the dot product is the
        cosine of the angle; otherwise the dot product must be
        divided by the product of the lengths of both vectors to get
        the cosine of the angle. This result can further be used to
        calculate the distance of a point from a plane.
        @param
        vec DiVector with which to calculate the dot product (together
        with this one).
        @returns
        A float representing the dot product value.
        */
        inline float dotProduct(const DiVec2& vec) const
        {
            return x * vec.x + y * vec.y;
        }

        /** Normalises the vector.
        @remarks
        This method normalises the vector such that it's
        length / magnitude is 1. The result is called a unit vector.
        @note
        This function will not crash for zero-sized vectors, but there
        will be no changes made to their components.
        @returns The previous length of the vector.
        */
        inline float normalise()
        {
            float fLength = DiMath::Sqrt( x * x + y * y);

            // Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
            }

            return fLength;
        }



        /** Returns a vector at a point half way between this and the passed
        in vector.
        */
        inline DiVec2 midPoint( const DiVec2& vec ) const
        {
            return DiVec2(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f );
        }

        /** Returns true if the vector's float components are all greater
        that the ones of the vector it is compared against.
        */
        inline bool operator < ( const DiVec2& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
                return true;
            return false;
        }

        /** Returns true if the vector's float components are all smaller
        that the ones of the vector it is compared against.
        */
        inline bool operator > ( const DiVec2& rhs ) const
        {
            if( x > rhs.x && y > rhs.y )
                return true;
            return false;
        }

        /** Sets this vector's components to the minimum of its own and the
        ones of the passed in vector.
        @remarks
        'Minimum' in this case means the combination of the lowest
        value of x, y and z from both vectors. Lowest is taken just
        numerically, not magnitude, so -1 < 0.
        */
        inline void makeFloor( const DiVec2& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
        }

        /** Sets this vector's components to the maximum of its own and the
        ones of the passed in vector.
        @remarks
        'Maximum' in this case means the combination of the highest
        value of x, y and z from both vectors. Highest is taken just
        numerically, not magnitude, so 1 > -3.
        */
        inline void makeCeil( const DiVec2& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
        @remarks
        This method will return a vector which is perpendicular to this
        vector. There are an infinite number of possibilities but this
        method will guarantee to generate one of them. If you need more
        control you should use the Quaternion class.
        */
        inline DiVec2 perpendicular(void) const
        {
            return DiVec2 (-y, x);
        }
        /** Calculates the 2 dimensional cross-product of 2 vectors, which results
        in a single floating point value which is 2 times the area of the triangle.
        */
        inline float crossProduct( const DiVec2& rkVector ) const
        {
            return x * rkVector.y - y * rkVector.x;
        }
        /** Generates a DI_NEW random vector which deviates from this vector by a
        given angle in a random direction.
        @remarks
        This method assumes that the random number generator has already
        been seeded appropriately.
        @param
        angle The angle at which to deviate in radians
        @param
        up Any vector perpendicular to this one (which could generated
        by cross-product of this vector and any other non-colinear
        vector). If you choose not to provide this the function will
        derive one on it's own, however if you provide one yourself the
        function will be faster (this allows you to reuse up vectors if
        you call this method more than once)
        @returns
        A random vector which deviates from this vector by angle. This
        vector will not be normalised, normalise it if you wish
        afterwards.
        */
        inline DiVec2 randomDeviant(
            float angle) const
        {

            angle *=  DiMath::UnitRandom() * DiMath::HALF_PI;
            float cosa = cos(angle);
            float sina = sin(angle);
            return  DiVec2(cosa * x - sina * y,
                sina * x + cosa * y);
        }

        /** Returns true if this vector is zero length. */
        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (1e-06 * 1e-06));

        }

        inline bool isZero() const
        {
            return DiMath::RealEqual(x,0) && DiMath::RealEqual(y,0);
        }

        /** As normalise, except that this vector is unaffected and the
        normalised vector is returned as a copy. */
        inline DiVec2 normalisedCopy(void) const
        {
            DiVec2 ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline DiVec2 reflect(const DiVec2& normal) const
        {
            return DiVec2( *this - ( 2 * this->dotProduct(normal) * normal ) );
        }
        /// Check whether this vector contains valid values
        inline bool isNaN() const
        {
            return DiMath::isNaN(x) || DiMath::isNaN(y);
        }

        // special points
        static const DiVec2 ZERO;
        static const DiVec2 UNIT_X;
        static const DiVec2 UNIT_Y;
        static const DiVec2 NEGATIVE_UNIT_X;
        static const DiVec2 NEGATIVE_UNIT_Y;
        static const DiVec2 UNIT_SCALE;
    };

    class DI_MISC_API DiIntVec2
    {
    public:
        /// Construct undefined.
        DiIntVec2()
        {
        }

        /// Construct from coordinates.
        DiIntVec2(int xx, int yy) :
         x(xx), y(yy)
        {
        }

        /// Copy-construct from another vector.
        DiIntVec2(const DiIntVec2& rhs) :
        x(rhs.x),
            y(rhs.y)
        {
        }

        /// Test for equality with another vector.
        bool operator == (const DiIntVec2& rhs) const { return x == rhs.x && y == rhs.y; }
        /// Test for inequality with another vector.
        bool operator != (const DiIntVec2& rhs) const { return x != rhs.x || y != rhs.y; }
        /// Add a vector.
        DiIntVec2 operator + (const DiIntVec2& rhs) const { return DiIntVec2(x + rhs.x, y + rhs.y); }

        DiIntVec2 operator + (int v) const { return DiIntVec2(x + v, y + v); }

        /// Return negation.
        DiIntVec2 operator - () const { return DiIntVec2(-x, -y); }
        /// Subtract a vector.
        DiIntVec2 operator - (const DiIntVec2& rhs) const { return DiIntVec2(x - rhs.x, y - rhs.y); }

        DiIntVec2 operator - (int v) const { return DiIntVec2(x - v, y - v); }

        /// Multiply with a scalar.
        DiIntVec2 operator * (int rhs) const { return DiIntVec2(x * rhs, y * rhs); }
        /// Divide by a scalar.
        DiIntVec2 operator / (int rhs) const { return DiIntVec2(x / rhs, y / rhs); }

        /// Add-assign a vector.
        DiIntVec2& operator += (const DiIntVec2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        /// Subtract-assign a vector.
        DiIntVec2& operator -= (const DiIntVec2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        /// Multiply-assign a scalar.
        DiIntVec2& operator *= (int rhs)
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        /// Divide-assign a scalar.
        DiIntVec2& operator /= (int rhs)
        {
            x /= rhs;
            y /= rhs;
            return *this;
        }

        inline bool operator < ( const DiIntVec2& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
            {
                return true;
            }
            return false;
        }

        inline bool operator > ( const DiIntVec2& rhs ) const
        {
            if( x > rhs.x && y > rhs.y )
            {
                return true;
            }
            return false;
        }

        /// Return integer data.
        const int* GetData() const { return &x; }

        /// X coordinate.
        int x;
        /// Y coordinate.
        int y;

        /// Zero vector.
        static const DiIntVec2 ZERO;
    };
}