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
#ifndef __C_ArrayQuaternion_H__
#define __C_ArrayQuaternion_H__

#ifndef __ArrayQuaternion_H__
    #error "Don't include this file directly. include Math/Array/ArrayQuaternion.h"
#endif

#include "Quaternion.h"

#include "Math/Array/Mathlib.h"
#include "Math/Array/ArrayVector3.h"

#include "ArrayQuaternion.h"

namespace Demi
{

    /** Cache-friendly array of Quaternion represented as a SoA array.
        @remarks
            ArrayQuaternion is a SIMD & cache-friendly version of Quaternion.
            An operation on an ArrayQuaternion is done on 4 quaternions at a
            time (the actual amount is defined by ARRAY_PACKED_REALS)
            Assuming ARRAY_PACKED_REALS == 4, the memory layout will
            be as following:
               mChunkBase             mChunkBase + 4
            WWWW XXXX YYYY ZZZZ     WWWW XXXX YYYY ZZZZ
            Extracting one quat (XYZW) needs 64 bytes, which is within
            the 64 byte size of common cache lines.
            Architectures where the cache line == 32 bytes may want to
            set ARRAY_PACKED_REALS = 2 depending on their needs
    */

    class DI_MISC_API ArrayQuaternion
    {
    public:
        float w, x, y, z;

        ArrayQuaternion() {}
        ArrayQuaternion( const Arrayfloat &chunkW, const Arrayfloat &chunkX,
                                const Arrayfloat &chunkY, const Arrayfloat &chunkZ )
        {
            w = chunkW;
            x = chunkX;
            y = chunkY;
            z = chunkZ;
        }

        void getAsQuaternion( Quaternion &out, size_t index ) const
        {
            //Be careful of not writing to these regions or else strict aliasing rule gets broken!!!
            const float *aliasedfloat = reinterpret_cast<const float*>( &w );
            out.w = aliasedfloat[ARRAY_PACKED_REALS * 0 + index];        //W
            out.x = aliasedfloat[ARRAY_PACKED_REALS * 1 + index];        //X
            out.y = aliasedfloat[ARRAY_PACKED_REALS * 2 + index];        //Y
            out.z = aliasedfloat[ARRAY_PACKED_REALS * 3 + index];        //Z
        }

        /// Prefer using @see getAsQuaternion() because this function may have more
        /// overhead (the other one is faster)
        Quaternion getAsQuaternion( size_t index ) const
        {
            //Be careful of not writing to these regions or else strict aliasing rule gets broken!!!
            const float *aliasedfloat = reinterpret_cast<const float*>( &w );
            return Quaternion( aliasedfloat[ARRAY_PACKED_REALS * 0 + index], //W
                            aliasedfloat[ARRAY_PACKED_REALS * 1 + index],        //X
                            aliasedfloat[ARRAY_PACKED_REALS * 2 + index],        //Y
                            aliasedfloat[ARRAY_PACKED_REALS * 3 + index] );  //Z
        }

        void setFromQuaternion( const Quaternion &v, size_t index )
        {
            float *aliasedfloat = reinterpret_cast<float*>( &w );
            aliasedfloat[ARRAY_PACKED_REALS * 0 + index] = v.w;
            aliasedfloat[ARRAY_PACKED_REALS * 1 + index] = v.x;
            aliasedfloat[ARRAY_PACKED_REALS * 2 + index] = v.y;
            aliasedfloat[ARRAY_PACKED_REALS * 3 + index] = v.z;
        }

        /// @copydoc Quaternion::FromAngleAxis
        inline void FromAngleAxis( const ArrayDiRadian& rfAngle, const ArrayVector3& rkAxis );

        /// @copydoc Quaternion::ToAngleAxis
        inline void ToAngleAxis( ArrayDiRadian &rfAngle, ArrayVector3 &rkAxis ) const;

        inline friend ArrayQuaternion operator * ( const ArrayQuaternion &lhs, const ArrayQuaternion &rhs );

        inline friend ArrayQuaternion operator + ( const ArrayQuaternion &lhs, const ArrayQuaternion &rhs );
        inline friend ArrayQuaternion operator - ( const ArrayQuaternion &lhs, const ArrayQuaternion &rhs );
        inline friend ArrayQuaternion operator * ( const ArrayQuaternion &lhs, Arrayfloat scalar );
        inline friend ArrayQuaternion operator * ( Arrayfloat scalar, const ArrayQuaternion &lhs );
        inline void operator += ( const ArrayQuaternion &a );
        inline void operator -= ( const ArrayQuaternion &a );
        inline void operator *= ( const Arrayfloat fScalar );

        /// @copydoc Quaternion::xAxis
        inline ArrayVector3 xAxis( void ) const;
        /// @copydoc Quaternion::yAxis
        inline ArrayVector3 yAxis( void ) const;
        /// @copydoc Quaternion::zAxis
        inline ArrayVector3 zAxis( void ) const;

        /// @copydoc Quaternion::Dot
        inline Arrayfloat Dot( const ArrayQuaternion& rkQ ) const;

        /// @copydoc Quaternion::Norm
        inline Arrayfloat Norm( void ) const; //Returns the squared length, doesn't modify

        /// Unlike Quaternion::normalise(), this function does not return the length of the vector
        /// because such value was not cached and was never available @see Quaternion::normalise()
        inline void normalise( void );

        inline ArrayQuaternion Inverse( void ) const;       // apply to non-zero quaternion
        inline ArrayQuaternion UnitInverse( void ) const;   // apply to unit-length quaternion
        inline ArrayQuaternion Exp( void ) const;
        inline ArrayQuaternion Log( void ) const;

        /// Rotation of a vector by a quaternion
        inline ArrayVector3 operator * ( const ArrayVector3 &v ) const;

        /** Rotates a vector by multiplying the quaternion to the vector, and modifies it's contents
            by storing the results there.
            @remarks
                This function is the same as doing:
                    ArrayVector v;
                    ArrayQuaternion q;
                    v = q * v;
                In fact, the operator overloading will make above code work perfectly. However, because
                we don't trust all compilers in optimizing this performance-sensitive function (in fact
                MSVC 2008 doesn't inline the op. and generates an unnecessary ArrayVector3) this
                function will take the input vector, and store the results back on that vector.
                This is very common when concatenating transformations on an ArrayVector3, whose
                memory reside in the heap (it makes better usage of the memory). Long story short,
                prefer calling this function to using an operator when just updating an ArrayVector3 is
                involved. (It's fine using operators for ArrayVector3s)
            @param
                
        */
        static inline void mul( const ArrayQuaternion &inQ, ArrayVector3 &inOutVec );

        /// @See Quaternion::Slerp
        /// @remarks
        ///     shortestPath is always true
        static inline ArrayQuaternion Slerp( Arrayfloat fT, const ArrayQuaternion &rkP,
                                                    const ArrayQuaternion &rkQ );

        /// @See Quaternion::nlerp
        /// @remarks
        ///     shortestPath is always false
        static inline ArrayQuaternion nlerp( Arrayfloat fT, const ArrayQuaternion& rkP, 
                                                    const ArrayQuaternion& rkQ );

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
                decide between two ArrayQuaternions, i.e. a = Cmov4( b, c ) then
                @see Cmov4( const ArrayQuaternion &arg1, const ArrayQuaternion &arg2, Arrayfloat mask );
                instead.
            @param
                Vectors to be used as replacement if the mask is zero.
            @param
                mask filled with either 0's or 0xFFFFFFFF
            @return
                this[i] = mask[i] != 0 ? this[i] : replacement[i]
        */
        inline void Cmov4( ArrayMaskR mask, const ArrayQuaternion &replacement );

        /** Conditional move. @See MathlibC::Cmov4
            Selects between arg1 & arg2 according to mask
            @remarks
                If mask param contains anything other than 0's or 0xffffffff's
                the result is undefined.
                If you wanted to do a = cmov4( a, b ), then consider using the update version
                @see Cmov4( Arrayfloat mask, const ArrayQuaternion &replacement );
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
        inline static ArrayQuaternion Cmov4( const ArrayQuaternion &arg1, const ArrayQuaternion &arg2, ArrayMaskR mask );

        static const ArrayQuaternion ZERO;
        static const ArrayQuaternion IDENTITY;
    };
    /** @} */
    /** @} */

}

#include "ArrayQuaternion.inl"

#endif
