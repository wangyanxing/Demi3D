
#pragma once

/********************************************************************
    File:       vector3.h
    Creator:    demiwangya
*********************************************************************/

#include "MiscPrerequisites.h"
#include "float.h"
#include "quaternion.h"
#include "mathmisc.h"

namespace Demi 
{
    inline int32 incm3(int32 i) { return i+1 & (i-2)>>31; }
    inline int32 decm3(int32 i) { return i-1 + ((i-1)>>31&3); }

    class DI_MISC_API DiVec3
    {
    public:
        float x, y, z;

    public:
        inline DiVec3()
        {
        }

        inline DiVec3( const float fX, const float fY, const float fZ )
            : x( fX ), y( fY ), z( fZ )
        {
        }

        inline explicit DiVec3( const float afCoordinate[3] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] )
        {
        }

        inline explicit DiVec3( const int afCoordinate[3] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
        }

        inline explicit DiVec3( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

        inline explicit DiVec3( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }


        /** Exchange the contents of this vector with another. 
        */
        inline void swap(DiVec3& other)
        {
            std::swap(x, other.x);
            std::swap(y, other.y);
            std::swap(z, other.z);
        }

        inline float operator [] ( const size_t i ) const
        {
            DI_ASSERT( i < 3 );

            return *(&x+i);
        }

        inline float& operator [] ( const size_t i )
        {
            DI_ASSERT( i < 3 );

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
        inline DiVec3& operator = ( const DiVec3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

        inline DiVec3& operator = ( const float fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const DiVec3& rkVector ) const
        {
            return (DiMath::RealEqual(x,rkVector.x) && DiMath::RealEqual(y,rkVector.y) && DiMath::RealEqual(z,rkVector.z));
        }

        inline bool operator != ( const DiVec3& rkVector ) const
        {
            return (!DiMath::RealEqual(x,rkVector.x) || !DiMath::RealEqual(y,rkVector.y) || !DiMath::RealEqual(z,rkVector.z));
        }

        // arithmetic operations
        inline DiVec3 operator + ( const DiVec3& rkVector ) const
        {
            return DiVec3(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline DiVec3 operator - ( const DiVec3& rkVector ) const
        {
            return DiVec3(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline DiVec3 operator * ( const float fScalar ) const
        {
            return DiVec3(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline DiVec3 operator * ( const DiVec3& rhs) const
        {
            return DiVec3(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline DiVec3 operator / ( const float fScalar ) const
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return DiVec3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline DiVec3 operator / ( const DiVec3& rhs) const
        {
            return DiVec3(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const DiVec3& operator + () const
        {
            return *this;
        }

        inline DiVec3 operator - () const
        {
            return DiVec3(-x, -y, -z);
        }

        inline friend DiVec3 operator * ( const float fScalar, const DiVec3& rkVector )
        {
            return DiVec3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend DiVec3 operator / ( const float fScalar, const DiVec3& rkVector )
        {
            return DiVec3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend DiVec3 operator + (const DiVec3& lhs, const float rhs)
        {
            return DiVec3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend DiVec3 operator + (const float lhs, const DiVec3& rhs)
        {
            return DiVec3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend DiVec3 operator - (const DiVec3& lhs, const float rhs)
        {
            return DiVec3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend DiVec3 operator - (const float lhs, const DiVec3& rhs)
        {
            return DiVec3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline DiVec3& operator += ( const DiVec3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline DiVec3& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline DiVec3& operator -= ( const DiVec3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline DiVec3& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline DiVec3& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline DiVec3& operator *= ( const DiVec3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline DiVec3& operator /= ( const float fScalar )
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline DiVec3& operator /= ( const DiVec3& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

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
            return DiMath::Sqrt( x * x + y * y + z * z );
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
            return x * x + y * y + z * z;
        }

        /** Returns the distance to another vector.
            @warning
                This operation requires a square root and is expensive in
                terms of CPU operations. If you don't need to know the exact
                distance (e.g. for just comparing distances) use squaredDistance()
                instead.
        */
        inline float distance(const DiVec3& rhs) const
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
        inline float squaredDistance(const DiVec3& rhs) const
        {
            return (*this - rhs).squaredLength();
        }

        inline DiVec3 abs() const
        {
            return DiVec3(DiMath::Abs(x), DiMath::Abs(y), DiMath::Abs(z));
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
        inline float dotProduct(const DiVec3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        /** Calculates the absolute dot (float) product of this vector with another.
            @remarks
                This function work similar dotProduct, except it use absolute value
                of each component of the vector to computing.
            @param
                vec DiVector with which to calculate the absolute dot product (together
                with this one).
            @returns
                A float representing the absolute dot product value.
        */
        inline float absDotProduct(const DiVec3& vec) const
        {
            return fabs(x * vec.x) + fabs(y * vec.y) + fabs(z * vec.z);
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
            float fLength = DiMath::Sqrt( x * x + y * y + z * z );

            // Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }

        /** Calculates the cross-product of 2 vectors, i.e. the vector that
            lies perpendicular to them both.
            @remarks
                The cross-product is normally used to calculate the normal
                vector of a plane, by calculating the cross-product of 2
                non-equivalent vectors which lie on the plane (e.g. 2 edges
                of a triangle).
            @param
                vec DiVector which, together with this one, will be used to
                calculate the cross-product.
            @returns
                A vector which is the result of the cross-product. This
                vector will <b>NOT</b> be normalised, to maximise efficiency
                - call Vector3::normalise on the result if you wish this to
                be done. As for which side the resultant vector will be on, the
                returned vector will be on the side from which the arc from 'this'
                to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
                = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
                This is because OGRE uses a right-handed coordinate system.
            @par
                For a clearer explanation, look a the left and the bottom edges
                of your monitor's screen. Assume that the first vector is the
                left edge and the second vector is the bottom edge, both of
                them starting from the lower-left corner of the screen. The
                resulting vector is going to be perpendicular to both of them
                and will go <i>inside</i> the screen, towards the cathode tube
                (assuming you're using a CRT monitor, of course).
        */
        inline DiVec3 crossProduct( const DiVec3& rkVector ) const
        {
            return DiVec3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        /** Returns a vector at a point half way between this and the passed
            in vector.
        */
        inline DiVec3 midPoint( const DiVec3& vec ) const
        {
            return DiVec3(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

        /** Returns true if the vector's float components are all greater
            that the ones of the vector it is compared against.
        */
        inline bool operator < ( const DiVec3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        /** Returns true if the vector's float components are all smaller
            that the ones of the vector it is compared against.
        */
        inline bool operator > ( const DiVec3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
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
        inline void makeFloor( const DiVec3& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
            if( cmp.z < z ) z = cmp.z;
        }

        /** Sets this vector's components to the maximum of its own and the
            ones of the passed in vector.
            @remarks
                'Maximum' in this case means the combination of the highest
                value of x, y and z from both vectors. Highest is taken just
                numerically, not magnitude, so 1 > -3.
        */
        inline void makeCeil( const DiVec3& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
            if( cmp.z > z ) z = cmp.z;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
            @remarks
                This method will return a vector which is perpendicular to this
                vector. There are an infinite number of possibilities but this
                method will guarantee to generate one of them. If you need more
                control you should use the Quaternion class.
        */
        inline DiVec3 perpendicular(void) const
        {
            static const float fSquareZero = (float)(1e-06 * 1e-06);

            DiVec3 perp = this->crossProduct( DiVec3::UNIT_X );

            // Check length
            if( perp.squaredLength() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a float, so we have
                   to use another axis.
                */
                perp = this->crossProduct( DiVec3::UNIT_Y );
            }
            perp.normalise();

            return perp;
        }
        /** Generates a DI_NEW random vector which deviates from this vector by a
            given angle in a random direction.
            @remarks
                This method assumes that the random number generator has already
                been seeded appropriately.
            @param
                angle The angle at which to deviate
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
        inline DiVec3 randomDeviant(
            const DiRadian& angle,
            const DiVec3& up = DiVec3::ZERO ) const
        {
            DiVec3 newUp;

            if (up == DiVec3::ZERO)
            {
                // Generate an up vector
                newUp = this->perpendicular();
            }
            else
            {
                newUp = up;
            }

            // Rotate up vector by random amount around this
            DiQuat q;
            q.FromAngleAxis( DiRadian(DiMath::UnitRandom() * DiMath::HALF_PI), *this );
            newUp = q * newUp;

            // Finally rotate this by given angle around randomised up
            q.FromAngleAxis( angle, newUp );
            return q * (*this);
        }

        /** Gets the angle between 2 vectors.
        @remarks
            Vectors do not have to be unit-length but must represent directions.
        */
        inline DiRadian angleBetween(const DiVec3& dest)
        {
            float lenProduct = length() * dest.length();

            // Divide by zero check
            if(lenProduct < 1e-6f)
                lenProduct = 1e-6f;

            float f = dotProduct(dest) / lenProduct;

            f = DiMath::Clamp(f, (float)-1.0, (float)1.0);
            return DiRadian(DiMath::ACos(f));

        }
        /** Gets the shortest arc quaternion to rotate this vector to the destination
            vector.
        @remarks
            If you call this with a dest vector that is close to the inverse
            of this vector, we will rotate 180 degrees around the 'fallbackAxis'
            (if specified, or a generated axis if not) since in this case
            ANY axis of rotation is valid.
        */
        DiQuat getRotationTo(const DiVec3& dest,
            const DiVec3& fallbackAxis = DiVec3::ZERO) const
        {
            // Based on Stan Melax's article in Game Programming Gems
            DiQuat q;
            // Copy, since cannot modify local
            DiVec3 v0 = *this;
            DiVec3 v1 = dest;
            v0.normalise();
            v1.normalise();

            float d = v0.dotProduct(v1);
            // If dot == 1, vectors are the same
            if (d >= 1.0f)
            {
                return DiQuat::IDENTITY;
            }
            if (d < (1e-6f - 1.0f))
            {
                if (fallbackAxis != DiVec3::ZERO)
                {
                    // rotate 180 degrees about the fallback axis
                    q.FromAngleAxis(DiRadian(DiMath::PI), fallbackAxis);
                }
                else
                {
                    // Generate an axis
                    DiVec3 axis = DiVec3::UNIT_X.crossProduct(*this);
                    if (axis.isZeroLength()) // pick another if colinear
                        axis = DiVec3::UNIT_Y.crossProduct(*this);
                    axis.normalise();
                    q.FromAngleAxis(DiRadian(DiMath::PI), axis);
                }
            }
            else
            {
                float s = DiMath::Sqrt( (1+d)*2 );
                float invs = 1 / s;

                DiVec3 c = v0.crossProduct(v1);

                q.x = c.x * invs;
                q.y = c.y * invs;
                q.z = c.z * invs;
                q.w = s * 0.5f;
                q.normalise();
            }
            return q;
        }

        /** Returns true if this vector is zero length. */
        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));

        }

        inline bool isZero() const
        {
            return DiMath::RealEqual(x,0) && DiMath::RealEqual(y,0) && DiMath::RealEqual(z,0);
        }

        /** As normalise, except that this vector is unaffected and the
            normalised vector is returned as a copy. */
        inline DiVec3 normalisedCopy(void) const
        {
            DiVec3 ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline DiVec3 reflect(const DiVec3& normal) const
        {
            return DiVec3( *this - ( 2 * this->dotProduct(normal) * normal ) );
        }

        /** Returns whether this vector is within a positional tolerance
            of another vector.
        @param rhs The vector to compare with
        @param tolerance The amount that each element of the vector may vary by
            and still be considered equal
        */
        inline bool positionEquals(const DiVec3& rhs, float tolerance = 1e-03) const
        {
            return DiMath::RealEqual(x, rhs.x, tolerance) &&
                DiMath::RealEqual(y, rhs.y, tolerance) &&
                DiMath::RealEqual(z, rhs.z, tolerance);

        }

        /** Returns whether this vector is within a positional tolerance
            of another vector, also take scale of the vectors into account.
        @param rhs The vector to compare with
        @param tolerance The amount (related to the scale of vectors) that distance
            of the vector may vary by and still be considered close
        */
        inline bool positionCloses(const DiVec3& rhs, float tolerance = 1e-03f) const
        {
            return squaredDistance(rhs) <=
                (squaredLength() + rhs.squaredLength()) * tolerance;
        }

        /** Returns whether this vector is within a directional tolerance
            of another vector.
        @param rhs The vector to compare with
        @param tolerance The maximum angle by which the vectors may vary and
            still be considered equal
        @note Both vectors should be normalised.
        */
        inline bool directionEquals(const DiVec3& rhs,
            const DiRadian& tolerance) const
        {
            float dot = dotProduct(rhs);
            DiRadian angle = DiRadian(DiMath::ACos(dot));

            return fabs(angle.valueRadians()) <= tolerance.valueRadians();

        }

        /// Check whether this vector contains valid values
        inline bool isNaN() const
        {
            return DiMath::isNaN(x) || DiMath::isNaN(y) || DiMath::isNaN(z);
        }

        inline DiVec3 GetOrthogonal() const
        {
            int i = DiMath::IsNeg(0.9f * 0.9f*squaredLength()-x*x);
            DiVec3 res;
            res[i]=0; 
            res[incm3(i)] = (*this)[decm3(i)]; 
            res[decm3(i)] = -(*this)[incm3(i)];
            return res;
        }
        // special points
        static const DiVec3 ZERO;
        static const DiVec3 UNIT_X;
        static const DiVec3 UNIT_Y;
        static const DiVec3 UNIT_Z;
        static const DiVec3 NEGATIVE_UNIT_X;
        static const DiVec3 NEGATIVE_UNIT_Y;
        static const DiVec3 NEGATIVE_UNIT_Z;
        static const DiVec3 UNIT_SCALE;
    };

    class DI_MISC_API DiIntVec3
    {
    public:
        DiIntVec3()
        {
        }

        DiIntVec3(int xx, int yy, int zz) :
            x(xx),y(yy),z(zz)
        {
        }

        DiIntVec3(const DiIntVec3& rhs) :
            x(rhs.x),y(rhs.y),z(rhs.z)
        {
        }

        
        bool operator == (const DiIntVec3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
        
        bool operator != (const DiIntVec3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

        DiIntVec3 operator + (const DiIntVec3& rhs) const { return DiIntVec3(x + rhs.x, y + rhs.y, z + rhs.z); }
        
        DiIntVec3 operator - () const { return DiIntVec3(-x, -y, -z); }
        
        DiIntVec3 operator - (const DiIntVec3& rhs) const { return DiIntVec3(x - rhs.x, y - rhs.y, z - rhs.z); }
        
        DiIntVec3 operator * (int rhs) const { return DiIntVec3(x * rhs, y * rhs, z * rhs); }
        
        DiIntVec3 operator / (int rhs) const { return DiIntVec3(x / rhs, y / rhs, z / rhs); }

        DiIntVec3& operator += (const DiIntVec3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        DiIntVec3& operator -= (const DiIntVec3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        DiIntVec3& operator *= (int rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        DiIntVec3& operator /= (int rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }

        const int* GetData() const { return &x; }

        int x;
        int y;
        int z;

        static const DiIntVec3 ZERO;
    };
}