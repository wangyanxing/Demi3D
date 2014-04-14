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
#ifndef __NEON_ArrayAabb_H__
#define __NEON_ArrayAabb_H__

#ifndef __ArrayAabb_H__
    #error "Don't include this file directly. include Math/Array/ArrayAabb.h"
#endif

#include "Math/Array/Mathlib.h"
#include "Math/Array/ArrayVector3.h"
#include "Math/Array/ArrayMatrix4.h"
#include "Math/Simple/Aabb.h"

namespace Demi
{

    /** Cache-friendly array of Aabb represented as a SoA array.
        @remarks
            ArrayAabb is a SIMD & cache-friendly version of AxisAlignedBox.
            (AABB = Axis aligned bounding box) @See ArrayVector3 for
            more information.
        @par
            For performance reasons given the mathematical properties,
            this version stores the box in the form "center + halfSize"
            instead of the form "minimum, maximum" that is present in
            AxisAlignedBox:
                * Merging is slightly more expensive
                * intersects() is much cheaper
                * Naturally deals with infinite boxes (no need for branches)
                * Transform is cheaper (a common operation)
        @par
            Extracting one aabb needs 84 bytes, while all 4 aabbs
            need 96 bytes, both cases are always two cache lines.
            Architectures where the cache line == 32 bytes may want to
            set ARRAY_PACKED_REALS = 2 depending on their needs
    */

    class DI_MISC_API ArrayAabb
    {
    public:
        ArrayVector3    mCenter;
        ArrayVector3    mHalfSize;

        ArrayAabb( const ArrayVector3 &center, const ArrayVector3 &halfSize ) :
                mCenter( center ), mHalfSize( halfSize )
        {
        }

        void getAsAabb( Aabb &out, size_t index ) const
        {
            //Be careful of not writing to these regions or else strict aliasing rule gets broken!!!
            const float *aliasedfloat = reinterpret_cast<const float*>( &mCenter );
            out.mCenter.x = aliasedfloat[ARRAY_PACKED_REALS * 0 + index];        //X
            out.mCenter.y = aliasedfloat[ARRAY_PACKED_REALS * 1 + index];        //Y
            out.mCenter.z = aliasedfloat[ARRAY_PACKED_REALS * 2 + index];        //Z
            out.mHalfSize.x = aliasedfloat[ARRAY_PACKED_REALS * 3 + index];      //X
            out.mHalfSize.y = aliasedfloat[ARRAY_PACKED_REALS * 4 + index];      //Y
            out.mHalfSize.z = aliasedfloat[ARRAY_PACKED_REALS * 5 + index];      //Z
        }

        /// Prefer using @see getAsAabb() because this function may have more
        /// overhead (the other one is faster)
        Aabb getAsAabb( size_t index ) const
        {
            Aabb retVal;
            getAsAabb( retVal, index );
            return retVal;
        }

        void setFromAabb( const Aabb &aabb, size_t index )
        {
            float *aliasedfloat = reinterpret_cast<float*>( &mCenter );
            aliasedfloat[ARRAY_PACKED_REALS * 0 + index] = aabb.mCenter.x;       //X
            aliasedfloat[ARRAY_PACKED_REALS * 1 + index] = aabb.mCenter.y;       //Y
            aliasedfloat[ARRAY_PACKED_REALS * 2 + index] = aabb.mCenter.z;       //Z
            aliasedfloat[ARRAY_PACKED_REALS * 3 + index] = aabb.mHalfSize.x;     //X
            aliasedfloat[ARRAY_PACKED_REALS * 4 + index] = aabb.mHalfSize.y;     //Y
            aliasedfloat[ARRAY_PACKED_REALS * 5 + index] = aabb.mHalfSize.z;     //Z
        }

        /// Gets the minimum corner of the box.
        inline ArrayVector3 getMinimum() const;

        /// Gets the maximum corner of the box.
        inline ArrayVector3 getMaximum() const;

        /** Merges the passed in box into the current box. The result is the
            box which encompasses both.
        */
        inline void merge( const ArrayAabb& rhs );

        /// Extends the box to encompass the specified point (if needed).
        inline void merge( const ArrayVector3& points );

        /** Transforms the box according to the matrix supplied.
        @remarks
        By calling this method you get the axis-aligned box which
        surrounds the transformed version of this box. Therefore each
        corner of the box is transformed by the matrix, then the
        extents are mapped back onto the axes to produce another
        AABB. Useful when you have a local AABB for an object which
        is then transformed.
        @note
        The matrix must be an affine matrix. @see DiMat4::isAffine.
        */
        inline void transformAffine( const ArrayMatrix4 &matrix );

        /// Returns whether or not this box intersects another.
        inline ArrayMaskR intersects( const ArrayAabb& b2 ) const;

        /// Calculate the area of intersection of this box and another
        inline ArrayAabb intersection( const ArrayAabb& b2 ) const;

        /// Calculate the volume of this box
        inline Arrayfloat volume(void) const;

        /// Tests whether another box contained by this box.
        inline ArrayMaskR contains( const ArrayAabb &other ) const;

        /// Tests whether the given point contained by this box.
        inline ArrayMaskR contains( const ArrayVector3 &v ) const;

        /// Returns the minimum distance between a given point and any part of the box.
        inline Arrayfloat distance( const ArrayVector3 &v ) const;

        static const ArrayAabb BOX_INFINITE;

        //Contains all zeroes. Used for inactive objects to avoid causing unnecessary NaNs
        static const ArrayAabb BOX_ZERO;
    };
    /** @} */
    /** @} */

}

#include "ArrayAabb.inl"

#endif