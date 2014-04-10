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
#ifndef __C_ArrayMatrix4_H__
#define __C_ArrayMatrix4_H__

#ifndef __ArrayMatrix4_H__
    #error "Don't include this file directly. include Math/Array/ArrayMatrix4.h"
#endif

#include "DiMat4.h"

#include "Math/Array/Mathlib.h"
#include "Math/Array/ArrayVector3.h"
#include "Math/Array/ArrayQuaternion.h"

namespace Demi
{
    class SimpleDiMat4;


    /** Cache-friendly container of 4x4 matrices represented as a SoA array.
        @remarks
            ArrayMatrix4 is a SIMD & cache-friendly version of DiMat4.
            An operation on an ArrayMatrix4 is done on 4 vectors at a
            time (the actual amount is defined by ARRAY_PACKED_REALS)
            Assuming ARRAY_PACKED_REALS == 4, the memory layout will
            be as following:
             mChunkBase mChunkBase + 3
             a00b00c00d00    a01b01c01d01
            Extracting one DiMat4 needs 256 bytes, which needs 4 line
            fetches for common cache lines of 64 bytes.
            Make sure extractions are made sequentially to avoid cache
            trashing and excessive bandwidth consumption, and prefer
            working on @See ArrayVector3 & @See ArrayQuaternion instead
            Architectures where the cache line == 32 bytes may want to
            set ARRAY_PACKED_REALS = 2 depending on their needs
    */

    class DI_MISC_API ArrayMatrix4
    {
    public:
        Arrayfloat       mChunkBase[16];

        ArrayMatrix4() {}
        ArrayMatrix4( const ArrayMatrix4 &copy )
        {
            //Using a loop minimizes instruction count (better i-cache)
            //Doing 4 at a time per iteration maximizes instruction pairing
            //Unrolling the whole loop is i-cache unfriendly and
            //becomes unmaintainable (16 lines!?)
            for( size_t i=0; i<16; i+=4 )
            {
                mChunkBase[i  ] = copy.mChunkBase[i  ];
                mChunkBase[i+1] = copy.mChunkBase[i+1];
                mChunkBase[i+2] = copy.mChunkBase[i+2];
                mChunkBase[i+3] = copy.mChunkBase[i+3];
            }
        }

        void getAsDiMat4( DiMat4 &out, size_t index ) const
        {
            //Be careful of not writing to these regions or else strict aliasing rule gets broken!!!
            const float * RESTRICT_ALIAS aliasedfloat = reinterpret_cast<const float*>( mChunkBase );
            float * RESTRICT_ALIAS matrix = reinterpret_cast<float*>( out._m );
            for( size_t i=0; i<16; i+=4 )
            {
                matrix[i  ] = aliasedfloat[ARRAY_PACKED_REALS * (i  ) + index];
                matrix[i+1] = aliasedfloat[ARRAY_PACKED_REALS * (i+1) + index];
                matrix[i+2] = aliasedfloat[ARRAY_PACKED_REALS * (i+2) + index];
                matrix[i+3] = aliasedfloat[ARRAY_PACKED_REALS * (i+3) + index];
            }
        }

        /// STRONGLY Prefer using @see getAsDiMat4() because this function may have more
        /// overhead (the other one is faster)
        DiMat4 getAsDiMat4( size_t index ) const
        {
            DiMat4 retVal;
            getAsDiMat4( retVal, index );

            return retVal;
        }

        void setFromDiMat4( const DiMat4 &m, size_t index )
        {
            float * RESTRICT_ALIAS aliasedfloat = reinterpret_cast<float*>( mChunkBase );
            const float * RESTRICT_ALIAS matrix = reinterpret_cast<const float*>( m._m );
            for( size_t i=0; i<16; i+=4 )
            {
                aliasedfloat[ARRAY_PACKED_REALS * (i  ) + index] = matrix[i  ];
                aliasedfloat[ARRAY_PACKED_REALS * (i+1) + index] = matrix[i+1];
                aliasedfloat[ARRAY_PACKED_REALS * (i+2) + index] = matrix[i+2];
                aliasedfloat[ARRAY_PACKED_REALS * (i+3) + index] = matrix[i+3];
            }
        }

        /// Sets all packed matrices to the same value as the scalar input matrix
        void setAll( const DiMat4 &m )
        {
            mChunkBase[0]  = m._m[0];
            mChunkBase[1]  = m._m[1];
            mChunkBase[2]  = m._m[2];
            mChunkBase[3]  = m._m[3];
            mChunkBase[4]  = m._m[4];
            mChunkBase[5]  = m._m[5];
            mChunkBase[6]  = m._m[6];
            mChunkBase[7]  = m._m[7];
            mChunkBase[8]  = m._m[8];
            mChunkBase[9]  = m._m[9];
            mChunkBase[10] = m._m[10];
            mChunkBase[11] = m._m[11];
            mChunkBase[12] = m._m[12];
            mChunkBase[13] = m._m[13];
            mChunkBase[14] = m._m[14];
            mChunkBase[15] = m._m[15];
        }

        static ArrayMatrix4 createAllFromDiMat4( const DiMat4 &m )
        {
            ArrayMatrix4 retVal;
            retVal.setAll( m );
            return retVal;
        }

        /** Assigns the value of the other matrix. Does not reference the
            ptr address, but rather perform a memory copy
            @param
                rkmatrix The other matrix
        */
        inline ArrayMatrix4& operator = ( const ArrayMatrix4& rkMatrix )
        {
            for( size_t i=0; i<16; i+=4 )
            {
                mChunkBase[i  ] = rkMatrix.mChunkBase[i  ];
                mChunkBase[i+1] = rkMatrix.mChunkBase[i+1];
                mChunkBase[i+2] = rkMatrix.mChunkBase[i+2];
                mChunkBase[i+3] = rkMatrix.mChunkBase[i+3];
            }
            return *this;
        }

        // Concatenation
        inline friend ArrayMatrix4 operator * ( const ArrayMatrix4 &lhs, const ArrayMatrix4 &rhs );

        inline ArrayVector3 operator * ( const ArrayVector3 &rhs ) const;

        /// Prefer the update version 'a *= b' A LOT over 'a = a * b'
        /// (copying from an ArrayMatrix4 is 256 bytes!)
        inline void operator *= ( const ArrayMatrix4 &rhs );

        /** Converts the given quaternion to a 3x3 matrix representation and fill our values
            @remarks
                Similar to @see Quaternion::ToRotationMatrix, this function will take the input
                quaternion and overwrite the first 3x3 subset of this matrix. The 4th row &
                columns are left untouched.
                This function is defined in ArrayMatrix4 to avoid including this header into
                ArrayQuaternion. The idea is that ArrayMatrix4 requires ArrayQuaternion, and
                ArrayQuaternion requires ArrayVector3. Simple dependency order
            @param
                The quaternion to convert from.
        */
        inline void fromQuaternion( const ArrayQuaternion &q );

        /// @copydoc DiMat4::makeTransform()
        inline void makeTransform( const ArrayVector3 &position, const ArrayVector3 &scale,
                                    const ArrayQuaternion &orientation );

        /** Converts these matrices contained in this ArrayMatrix to AoS form and stores them in dst
        @remarks
            'dst' must be aligned and assumed to have enough memory for ARRAY_PACKED_REALS matrices
        */
        inline void storeToAoS( DiMat4 * RESTRICT_ALIAS dst ) const;

        /** Converts ARRAY_PACKED_REALS matrices into this ArrayMatrix
        @remarks
            'src' must be aligned and assumed to have enough memory for ARRAY_PACKED_REALS matrices
        */
        inline void loadFromAoS( const DiMat4 * RESTRICT_ALIAS src );
        inline void loadFromAoS( const SimpleDiMat4 * RESTRICT_ALIAS src );

        /// @copydoc DiMat4::isAffine()
        inline bool isAffine() const;

        static const ArrayMatrix4 IDENTITY;
    };

    /** Simple wrap up to load an AoS matrix 4x4 using SSE. The main reason of this class
        is to force MSVC to use 4 movaps to load arrays of DiMat4s (which are waaay more
        efficient that whatever lea+mov junk it tries to produce)
    */
    class DI_MISC_API SimpleDiMat4
    {
    public:
        Arrayfloat       mChunkBase[16];

        /// Assumes src is aligned
        void load( const DiMat4 &src )
        {
            mChunkBase[0]  = src._m[0];
            mChunkBase[1]  = src._m[1];
            mChunkBase[2]  = src._m[2];
            mChunkBase[3]  = src._m[3];
            mChunkBase[4]  = src._m[4];
            mChunkBase[5]  = src._m[5];
            mChunkBase[6]  = src._m[6];
            mChunkBase[7]  = src._m[7];
            mChunkBase[8]  = src._m[8];
            mChunkBase[9]  = src._m[9];
            mChunkBase[10] = src._m[10];
            mChunkBase[11] = src._m[11];
            mChunkBase[12] = src._m[12];
            mChunkBase[13] = src._m[13];
            mChunkBase[14] = src._m[14];
            mChunkBase[15] = src._m[15];
        }
    };

    /** @} */
    /** @} */

}

#include "ArrayMatrix4.inl"

#endif
