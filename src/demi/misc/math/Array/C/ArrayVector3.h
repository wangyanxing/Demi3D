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

/// This file is adapted from Ogre 2.0 (working version)
#ifndef __C_ArrayVector3_H__
#define __C_ArrayVector3_H__

#ifndef __ArrayVector3_H__
    #error "Don't include this file directly. include Math/Array/ArrayVector3.h"
#endif

#include "DiVec3.h"

#include "Math/Array/Mathlib.h"

namespace Demi
{
    class ArrayInterQuaternion;


    /** Cache-friendly array of 3-dimensional represented as a SoA array.
        @remarks
            ArrayVector3 is a SIMD & cache-friendly version of DiVec3.
            An operation on an ArrayVector3 is done on 4 vectors at a
            time (the actual amount is defined by ARRAY_PACKED_REALS)
            Assuming ARRAY_PACKED_REALS == 4, the memory layout will
            be as following:
             mChunkBase     mChunkBase + 3
            XXXX YYYY ZZZZ      XXXX YYYY ZZZZ
            Extracting one vector (XYZ) needs 48 bytes, which is within
            the 64 byte size of common cache lines.
            Architectures where the cache line == 32 bytes may want to
            set ARRAY_PACKED_REALS = 2 depending on their needs
    */

    class DI_MISC_API ArrayVector3
    {
    public:
        Arrayfloat       mChunkBase[3];

        ArrayVector3() {}
        ArrayVector3( Arrayfloat chunkX, Arrayfloat chunkY, Arrayfloat chunkZ )
        {
            mChunkBase[0] = chunkX;
            mChunkBase[1] = chunkY;
            mChunkBase[2] = chunkZ;
        }

        void getAsDiVec3( DiVec3 &out, size_t index ) const
        {
            //Be careful of not writing to these regions or else strict aliasing rule gets broken!!!
            const float *aliasedfloat = reinterpret_cast<const float*>( mChunkBase );
            out.x = aliasedfloat[ARRAY_PACKED_REALS * 0 + index];        //X
            out.y = aliasedfloat[ARRAY_PACKED_REALS * 1 + index];        //Y
            out.z = aliasedfloat[ARRAY_PACKED_REALS * 2 + index];        //Z
        }

        /// Prefer using @see getAsDiVec3() because this function may have more
        /// overhead (the other one is faster)
        DiVec3 getAsDiVec3( size_t index ) const
        {
            //Be careful of not writing to these regions or else strict aliasing rule gets broken!!!
            const float *aliasedfloat = reinterpret_cast<const float*>( mChunkBase );
            return DiVec3( aliasedfloat[ARRAY_PACKED_REALS * 0 + index],        //X
                            aliasedfloat[ARRAY_PACKED_REALS * 1 + index],        //Y
                            aliasedfloat[ARRAY_PACKED_REALS * 2 + index] );  //Z
        }

        void setFromDiVec3( const DiVec3 &v, size_t index )
        {
            float *aliasedfloat = reinterpret_cast<float*>( mChunkBase );
            aliasedfloat[ARRAY_PACKED_REALS * 0 + index] = v.x;
            aliasedfloat[ARRAY_PACKED_REALS * 1 + index] = v.y;
            aliasedfloat[ARRAY_PACKED_REALS * 2 + index] = v.z;
        }

        /// Sets all packed vectors to the same value as the scalar input vector
        void setAll( const DiVec3 &v )
        {
            mChunkBase[0] = v.x;
            mChunkBase[1] = v.y;
            mChunkBase[2] = v.z;
        }

        /// Copies only one vector, by looking at the indexes
        /*void copyScalar( size_t ourIndex, const ArrayVector3 &copy, size_t copyIndex )
        {
            DiVec3 tmp;
            copy.getAsDiVec3( tmp );
            this->setFromDiVec3( tmp );
        }*/

        inline ArrayVector3& operator = ( const float fScalar )
        {
            mChunkBase[0] = fScalar;
            mChunkBase[1] = fScalar;
            mChunkBase[2] = fScalar;

            return *this;
        }

        // Arithmetic operations
        inline const ArrayVector3& operator + () const;
        inline ArrayVector3 operator - () const;

        inline friend ArrayVector3 operator + ( const ArrayVector3 &lhs, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator + ( float fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator + ( const ArrayVector3 &lhs, float fScalar );

        inline friend ArrayVector3 operator + ( Arrayfloat fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator + ( const ArrayVector3 &lhs, Arrayfloat fScalar );

        inline friend ArrayVector3 operator - ( const ArrayVector3 &lhs, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator - ( float fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator - ( const ArrayVector3 &lhs, float fScalar );

        inline friend ArrayVector3 operator - ( Arrayfloat fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator - ( const ArrayVector3 &lhs, Arrayfloat fScalar );

        inline friend ArrayVector3 operator * ( const ArrayVector3 &lhs, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator * ( float fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator * ( const ArrayVector3 &lhs, float fScalar );

        inline friend ArrayVector3 operator * ( Arrayfloat fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator * ( const ArrayVector3 &lhs, Arrayfloat fScalar );

        inline friend ArrayVector3 operator / ( const ArrayVector3 &lhs, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator / ( float fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator / ( const ArrayVector3 &lhs, float fScalar );

        inline friend ArrayVector3 operator / ( Arrayfloat fScalar, const ArrayVector3 &rhs );
        inline friend ArrayVector3 operator / ( const ArrayVector3 &lhs, Arrayfloat fScalar );

        inline void operator += ( const ArrayVector3 &a );
        inline void operator += ( const Arrayfloat fScalar );

        inline void operator -= ( const ArrayVector3 &a );
        inline void operator -= ( const Arrayfloat fScalar );

        inline void operator *= ( const ArrayVector3 &a );
        inline void operator *= ( const Arrayfloat fScalar );

        inline void operator /= ( const ArrayVector3 &a );
        inline void operator /= ( const Arrayfloat fScalar );

        /// @copydoc DiVec3::length()
        inline Arrayfloat length() const;

        /// @copydoc DiVec3::squaredLength()
        inline Arrayfloat squaredLength() const;

        /// @copydoc DiVec3::distance()
        inline Arrayfloat distance( const ArrayVector3& rhs ) const;

        /// @copydoc DiVec3::squaredDistance()
        inline Arrayfloat squaredDistance( const ArrayVector3& rhs ) const;

        /// @copydoc DiVec3::dotProduct()
        inline Arrayfloat dotProduct( const ArrayVector3& vec ) const;

        /// @copydoc DiVec3::absDotProduct()
        inline Arrayfloat absDotProduct( const ArrayVector3& vec ) const;

        /// Unlike DiVec3::normalise(), this function does not return the length of the vector
        /// because such value was not cached and was never available @see DiVec3::normalise()
        inline void normalise( void );

        /// @copydoc DiVec3::crossProduct()
        inline ArrayVector3 crossProduct( const ArrayVector3& rkVector ) const;

        /// @copydoc DiVec3::midPoint()
        inline ArrayVector3 midPoint( const ArrayVector3& vec ) const;

        /// @copydoc DiVec3::makeFloor()
        inline void makeFloor( const ArrayVector3& cmp );

        /// @copydoc DiVec3::makeCeil()
        inline void makeCeil( const ArrayVector3& cmp );

        /// Returns the smallest value between x, y, z; min( x, y, z )
        inline Arrayfloat getMinComponent() const;

        /// Returns the biggest value between x, y, z; max( x, y, z )
        inline Arrayfloat getMaxComponent() const;

        /** Converts the vector to (sign(x), sign(y), sign(z))
        @remarks
            For reference, sign( x ) = x >= 0 ? 1.0 : -1.0
            sign( -0.0f ) may return 1 or -1 depending on implementation
            @par
            SSE2 implementation: Does distinguish between -0 & 0
            C implementation: Does not distinguish between -0 & 0
        */
        inline void setToSign();

        /// @copydoc DiVec3::perpendicular()
        inline ArrayVector3 perpendicular( void ) const;

        /// @copydoc DiVec3::normalisedCopy()
        inline ArrayVector3 normalisedCopy( void ) const;

        /// @copydoc DiVec3::reflect()
        inline ArrayVector3 reflect( const ArrayVector3& normal ) const;

        /** Calculates the inverse of the vectors: 1.0f / v;
            But if original is zero, the zero is left (0 / 0 = 0).
            Example:
            Bfore inverseLeaveZero:
                x = 0; y = 2; z = 3;
            After inverseLeaveZero
                x = 0; y = 0.5; z = 0.3333;
        */
        inline void inverseLeaveZeroes( void );

        /// @see DiVec3::isNaN()
        /// @return
        ///     Return value differs from DiVec3's counterpart. We return an int
        ///     bits 0-4 are set for each NaN of each vector inside.
        ///     if the int is non-zero, there is a NaN.
        inline int isNaN( void ) const;

        /// @copydoc DiVec3::primaryAxis()
        inline ArrayVector3 primaryAxis( void ) const;

        /** Takes each Vector and returns one returns a single vector
        @remarks
            This is useful when calculating bounding boxes, since it can be done independently
            in SIMD form, and once it is done, merge the results from the simd vectors into one
        @return
            Vector.x = min( vector[0].x, vector[1].x, vector[2].x, vector[3].x )
            Vector.y = min( vector[0].y, vector[1].y, vector[2].y, vector[3].y )
            Vector.z = min( vector[0].z, vector[1].z, vector[2].z, vector[3].z )
        */
        inline DiVec3 collapseMin( void ) const;

        /** Takes each Vector and returns one returns a single vector
        @remarks
            This is useful when calculating bounding boxes, since it can be done independently
            in SIMD form, and once it is done, merge the results from the simd vectors into one
        @return
            Vector.x = max( vector[0].x, vector[1].x, vector[2].x, vector[3].x )
            Vector.y = max( vector[0].y, vector[1].y, vector[2].y, vector[3].y )
            Vector.z = max( vector[0].z, vector[1].z, vector[2].z, vector[3].z )
        */
        inline DiVec3 collapseMax( void ) const;

        /** Conditional move update. @See MathlibC::Cmov4
            Changes each of the four vectors contained in 'this' with
            the replacement provided
            @remarks
                If mask param contains anything other than 0's or 0xffffffff's
                the result is undefined.
                Use this version if you want to decide whether to keep current
                result or overwrite with a replacement (performance optimization).
                i.e. a = Cmov4( a, b )
                If this vector hasn't been assigned yet any value and want to
                decide between two ArrayVector3s, i.e. a = Cmov4( b, c ) then
                @see Cmov4( const ArrayVector3 &arg1, const ArrayVector3 &arg2, Arrayfloat mask );
                instead.
            @param
                Vectors to be used as replacement if the mask is zero.
            @param
                mask filled with either 0's or 0xFFFFFFFF
            @return
                this[i] = mask[i] != 0 ? this[i] : replacement[i]
        */
        inline void Cmov4( ArrayMaskR mask, const ArrayVector3 &replacement );

        /** Conditional move update. @See MathlibC::CmovRobust
            Changes each of the four vectors contained in 'this' with
            the replacement provided
            @remarks
                If mask param contains anything other than 0's or 0xffffffff's
                the result is undefined.
                Use this version if you want to decide whether to keep current
                result or overwrite with a replacement (performance optimization).
                i.e. a = CmovRobust( a, b )
                If this vector hasn't been assigned yet any value and want to
                decide between two ArrayVector3s, i.e. a = Cmov4( b, c ) then
                @see Cmov4( const ArrayVector3 &arg1, const ArrayVector3 &arg2, Arrayfloat mask );
                instead.
            @param
                Vectors to be used as replacement if the mask is zero.
            @param
                mask filled with either 0's or 0xFFFFFFFF
            @return
                this[i] = mask[i] != 0 ? this[i] : replacement[i]
        */
        inline void CmovRobust( ArrayMaskR mask, const ArrayVector3 &replacement );

        /** Conditional move. @See MathlibC::Cmov4
            Selects between arg1 & arg2 according to mask
            @remarks
                If mask param contains anything other than 0's or 0xffffffff's
                the result is undefined.
                If you wanted to do a = cmov4( a, b ), then consider using the update version
                @see Cmov4( Arrayfloat mask, const ArrayVector3 &replacement );
                instead.
            @param
                First array of Vectors
            @param
                Second array of Vectors
            @param
                mask filled with either 0's or 0xFFFFFFFF
            @return
                this[i] = mask[i] != 0 ? arg1[i] : arg2[i]
        */
        inline static ArrayVector3 Cmov4( const ArrayVector3 &arg1, const ArrayVector3 &arg2, ArrayMaskR mask );

        static const ArrayVector3 ZERO;
        static const ArrayVector3 UNIT_X;
        static const ArrayVector3 UNIT_Y;
        static const ArrayVector3 UNIT_Z;
        static const ArrayVector3 NEGATIVE_UNIT_X;
        static const ArrayVector3 NEGATIVE_UNIT_Y;
        static const ArrayVector3 NEGATIVE_UNIT_Z;
        static const ArrayVector3 UNIT_SCALE;
    };
    /** @} */
    /** @} */

}

#include "ArrayVector3.inl"

#endif
