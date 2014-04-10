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

#ifndef Divector4_h__
#define Divector4_h__


#include "MiscPrerequisites.h"
#include "math/vector3.h"

namespace Demi
{
    class  DI_MISC_API DiVec4
    {
    public:
        float x, y, z, w;

    public:
        inline DiVec4()
            : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
        {
        }

        inline DiVec4( const float fX, const float fY, const float fZ, const float fW )
            : x( fX ), y( fY ), z( fZ ), w( fW)
        {
        }

        inline explicit DiVec4( const float afCoordinate[4] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] ),
              w( afCoordinate[3] )
        {
        }

        inline explicit DiVec4( const int afCoordinate[4] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
            w = (float)afCoordinate[3];
        }

        inline explicit DiVec4( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
        {
        }

        inline explicit DiVec4( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
            , w( scaler )
        {
        }

        inline explicit DiVec4(const DiVec3& rhs)
            : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
        {
        }

        /** Exchange the contents of this vector with another. 
        */
        inline void swap(DiVec4& other)
        {
            std::swap(x, other.x);
            std::swap(y, other.y);
            std::swap(z, other.z);
            std::swap(w, other.w);
        }

        inline float operator [] ( const size_t i ) const
        {
            DI_ASSERT( i < 4 );

            return *(&x+i);
        }

        inline float& operator [] ( const size_t i )
        {
            DI_ASSERT( i < 4 );

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

        inline DiVec4& operator = ( const DiVec4& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;
            w = rkVector.w;

            return *this;
        }

        inline DiVec4& operator = ( const float fScalar)
        {
            x = fScalar;
            y = fScalar;
            z = fScalar;
            w = fScalar;
            return *this;
        }

        inline bool operator == ( const DiVec4& rkVector ) const
        {
            return ( x == rkVector.x &&
                y == rkVector.y &&
                z == rkVector.z &&
                w == rkVector.w );
        }

        inline bool operator != ( const DiVec4& rkVector ) const
        {
            return ( x != rkVector.x ||
                y != rkVector.y ||
                z != rkVector.z ||
                w != rkVector.w );
        }

        inline DiVec4& operator = (const DiVec3& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = 1.0f;
            return *this;
        }

        // arithmetic operations
        inline DiVec4 operator + ( const DiVec4& rkVector ) const
        {
            return DiVec4(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z,
                w + rkVector.w);
        }

        inline DiVec4 operator - ( const DiVec4& rkVector ) const
        {
            return DiVec4(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z,
                w - rkVector.w);
        }

        inline DiVec4 operator * ( const float fScalar ) const
        {
            return DiVec4(
                x * fScalar,
                y * fScalar,
                z * fScalar,
                w * fScalar);
        }

        inline DiVec4 operator * ( const DiVec4& rhs) const
        {
            return DiVec4(
                rhs.x * x,
                rhs.y * y,
                rhs.z * z,
                rhs.w * w);
        }

        inline DiVec4 operator / ( const float fScalar ) const
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return DiVec4(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline DiVec4 operator / ( const DiVec4& rhs) const
        {
            return DiVec4(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w);
        }

        inline const DiVec4& operator + () const
        {
            return *this;
        }

        inline DiVec4 operator - () const
        {
            return DiVec4(-x, -y, -z, -w);
        }

        inline friend DiVec4 operator * ( const float fScalar, const DiVec4& rkVector )
        {
            return DiVec4(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z,
                fScalar * rkVector.w);
        }

        inline friend DiVec4 operator / ( const float fScalar, const DiVec4& rkVector )
        {
            return DiVec4(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z,
                fScalar / rkVector.w);
        }

        inline friend DiVec4 operator + (const DiVec4& lhs, const float rhs)
        {
            return DiVec4(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs,
                lhs.w + rhs);
        }

        inline friend DiVec4 operator + (const float lhs, const DiVec4& rhs)
        {
            return DiVec4(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z,
                lhs + rhs.w);
        }

        inline friend DiVec4 operator - (const DiVec4& lhs, float rhs)
        {
            return DiVec4(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs,
                lhs.w - rhs);
        }

        inline friend DiVec4 operator - (const float lhs, const DiVec4& rhs)
        {
            return DiVec4(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z,
                lhs - rhs.w);
        }

        // arithmetic updates
        inline DiVec4& operator += ( const DiVec4& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;
            w += rkVector.w;

            return *this;
        }

        inline DiVec4& operator -= ( const DiVec4& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;
            w -= rkVector.w;

            return *this;
        }

        inline DiVec4& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            w *= fScalar;
            return *this;
        }

        inline DiVec4& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            w += fScalar;
            return *this;
        }

        inline DiVec4& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            w -= fScalar;
            return *this;
        }

        inline DiVec4& operator *= ( const DiVec4& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;
            w *= rkVector.w;

            return *this;
        }

        inline DiVec4& operator /= ( const float fScalar )
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

        inline DiVec4& operator /= ( const DiVec4& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;
            w /= rkVector.w;

            return *this;
        }

        /** Calculates the dot (float) product of this vector with another.
            @param
                vec Vector with which to calculate the dot product (together
                with this one).
            @returns
                A float representing the dot product value.
        */
        inline float dotProduct(const DiVec4& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }
        /// Check whether this vector contains valid values
        inline bool isNaN() const
        {
            return DiMath::isNaN(x) || DiMath::isNaN(y) || DiMath::isNaN(z) || DiMath::isNaN(w);
        }

        inline bool isZero() const
        {
            return DiMath::RealEqual(x,0) && DiMath::RealEqual(y,0) && DiMath::RealEqual(z,0) && DiMath::RealEqual(w,0);
        }
       
        // special
        static const DiVec4 ZERO;
    };

}

#endif
