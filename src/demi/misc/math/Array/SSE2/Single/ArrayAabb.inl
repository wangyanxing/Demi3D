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

/// This file is adapted from Ogre 2.0 (unstable version)

namespace Demi
{
    
    inline ArrayVector3 ArrayAabb::getMinimum() const
    {
        return mCenter - mHalfSize;
    }
    
    inline ArrayVector3 ArrayAabb::getMaximum() const
    {
        return mCenter + mHalfSize;
    }
    
    inline void ArrayAabb::merge( const ArrayAabb& rhs )
    {
        ArrayVector3 maxval( mCenter + mHalfSize );
        maxval.makeCeil(rhs.mCenter + rhs.mHalfSize);

        ArrayVector3 minval(mCenter - mHalfSize);
        minval.makeFloor(rhs.mCenter - rhs.mHalfSize);

        mCenter = (maxval + minval) * Mathlib::HALF;
        mHalfSize = (maxval - minval) * Mathlib::HALF;
    }
    
    inline void ArrayAabb::merge( const ArrayVector3& points )
    {
        ArrayVector3 maxval(mCenter + mHalfSize);
        maxval.makeCeil(points);

        ArrayVector3 minval(mCenter - mHalfSize);
        minval.makeFloor(points);

        mCenter = (maxval + minval) * Mathlib::HALF;
        mHalfSize = (maxval - minval) * Mathlib::HALF;
    }
    
    inline ArrayMaskR ArrayAabb::intersects( const ArrayAabb& b2 ) const
    {
        ArrayVector3 dist( mCenter - b2.mCenter );
        ArrayVector3 sumHalfSizes( mHalfSize + b2.mHalfSize );

        // ( abs( center.x - center2.x ) <= halfSize.x + halfSize2.x &&
        //   abs( center.y - center2.y ) <= halfSize.y + halfSize2.y &&
        //   abs( center.z - center2.z ) <= halfSize.z + halfSize2.z )
        Arrayfloat maskX = _mm_cmple_ps( MathlibSSE2::Abs4( dist.mChunkBase[0] ),
                                        sumHalfSizes.mChunkBase[0] );
        Arrayfloat maskY = _mm_cmple_ps( MathlibSSE2::Abs4( dist.mChunkBase[1] ),
                                        sumHalfSizes.mChunkBase[1] );
        Arrayfloat maskZ = _mm_cmple_ps( MathlibSSE2::Abs4( dist.mChunkBase[2] ),
                                        sumHalfSizes.mChunkBase[2] );
        
        return _mm_and_ps( _mm_and_ps( maskX, maskY ), maskZ );
    }
    
    inline Arrayfloat ArrayAabb::volume(void) const
    {
        Arrayfloat w = _mm_add_ps( mHalfSize.mChunkBase[0], mHalfSize.mChunkBase[0] ); // x * 2
        Arrayfloat h = _mm_add_ps( mHalfSize.mChunkBase[1], mHalfSize.mChunkBase[1] ); // y * 2
        Arrayfloat d = _mm_add_ps( mHalfSize.mChunkBase[2], mHalfSize.mChunkBase[2] ); // z * 2

        return _mm_mul_ps( _mm_mul_ps( w, h ), d ); // w * h * d
    }
    
    inline Arrayfloat ArrayAabb::contains( const ArrayAabb &other ) const
    {
        ArrayVector3 dist( mCenter - other.mCenter );

        // In theory, "abs( dist.x ) < mHalfSize - other.mHalfSize" should be more pipeline-
        // friendly because the processor can do the subtraction while the abs4() is being performed,
        // however that variation won't handle the case where both boxes are infinite (will produce
        // nan instead and return false, when it should return true)

        // ( abs( dist.x ) + other.mHalfSize.x <= mHalfSize.x &&
        //   abs( dist.y ) + other.mHalfSize.y <= mHalfSize.y &&
        //   abs( dist.z ) + other.mHalfSize.z <= mHalfSize.z )
        Arrayfloat maskX = _mm_cmple_ps( _mm_add_ps( MathlibSSE2::Abs4( dist.mChunkBase[0] ),
                                        other.mHalfSize.mChunkBase[0] ), mHalfSize.mChunkBase[0] );
        Arrayfloat maskY = _mm_cmple_ps( _mm_add_ps( MathlibSSE2::Abs4( dist.mChunkBase[1] ),
                                        other.mHalfSize.mChunkBase[1] ), mHalfSize.mChunkBase[1] );
        Arrayfloat maskZ = _mm_cmple_ps( _mm_add_ps( MathlibSSE2::Abs4( dist.mChunkBase[2] ),
                                        other.mHalfSize.mChunkBase[2] ), mHalfSize.mChunkBase[2] );

        return _mm_and_ps( _mm_and_ps( maskX, maskY ), maskZ );
    }
    
    inline Arrayfloat ArrayAabb::contains( const ArrayVector3 &v ) const
    {
        ArrayVector3 dist( mCenter - v );

        // ( abs( dist.x ) <= mHalfSize.x &&
        //   abs( dist.y ) <= mHalfSize.y &&
        //   abs( dist.z ) <= mHalfSize.z )
        Arrayfloat maskX = _mm_cmple_ps( MathlibSSE2::Abs4( dist.mChunkBase[0] ),
                                        mHalfSize.mChunkBase[0] );
        Arrayfloat maskY = _mm_cmple_ps( MathlibSSE2::Abs4( dist.mChunkBase[1] ),
                                        mHalfSize.mChunkBase[1] );
        Arrayfloat maskZ = _mm_cmple_ps( MathlibSSE2::Abs4( dist.mChunkBase[2] ),
                                        mHalfSize.mChunkBase[2] );

        return _mm_and_ps( _mm_and_ps( maskX, maskY ), maskZ );
    }
    
    inline Arrayfloat ArrayAabb::distance( const ArrayVector3 &v ) const
    {
        ArrayVector3 dist( mCenter - v );

        // x = abs( dist.x ) - halfSize.x
        // y = abs( dist.y ) - halfSize.y
        // z = abs( dist.z ) - halfSize.z
        // return max( min( x, y, z ), 0 ); //Return minimum between xyz, clamp to zero
        dist.mChunkBase[0] = _mm_sub_ps( MathlibSSE2::Abs4( dist.mChunkBase[0] ),
                                                mHalfSize.mChunkBase[0] );
        dist.mChunkBase[1] = _mm_sub_ps( MathlibSSE2::Abs4( dist.mChunkBase[1] ),
                                                mHalfSize.mChunkBase[1] );
        dist.mChunkBase[2] = _mm_sub_ps( MathlibSSE2::Abs4( dist.mChunkBase[2] ),
                                                mHalfSize.mChunkBase[2] );

        return _mm_max_ps( _mm_min_ps( _mm_min_ps( dist.mChunkBase[0],
                    dist.mChunkBase[1] ), dist.mChunkBase[2] ), _mm_setzero_ps() );
    }
    
    inline void ArrayAabb::transformAffine( const ArrayMatrix4 &m )
    {
        assert( m.isAffine() );

        mCenter = m * mCenter;

        Arrayfloat x = _mm_mul_ps( Mathlib::Abs4( m.mChunkBase[2] ), mHalfSize.mChunkBase[2] );  // abs( m02 ) * z +
        x = _mm_madd_ps( Mathlib::Abs4( m.mChunkBase[1] ), mHalfSize.mChunkBase[1], x );            // abs( m01 ) * y +
        x = _mm_madd_ps( Mathlib::Abs4( m.mChunkBase[0] ), mHalfSize.mChunkBase[0], x );            // abs( m00 ) * x

        Arrayfloat y = _mm_mul_ps( Mathlib::Abs4( m.mChunkBase[6] ), mHalfSize.mChunkBase[2] );  // abs( m12 ) * z +
        y = _mm_madd_ps( Mathlib::Abs4( m.mChunkBase[5] ), mHalfSize.mChunkBase[1], y );            // abs( m11 ) * y +
        y = _mm_madd_ps( Mathlib::Abs4( m.mChunkBase[4] ), mHalfSize.mChunkBase[0], y );            // abs( m10 ) * x

        Arrayfloat z = _mm_mul_ps( Mathlib::Abs4( m.mChunkBase[10] ), mHalfSize.mChunkBase[2] ); // abs( m22 ) * z +
        z = _mm_madd_ps( Mathlib::Abs4( m.mChunkBase[9] ), mHalfSize.mChunkBase[1], z );            // abs( m21 ) * y +
        z = _mm_madd_ps( Mathlib::Abs4( m.mChunkBase[8] ), mHalfSize.mChunkBase[0], z );            // abs( m20 ) * x

        //Handle infinity boxes not becoming NaN. Null boxes containing -Inf will still have NaNs
        //(which is ok since we need them to say 'false' to intersection tests)
        x = MathlibSSE2::CmovRobust( MathlibSSE2::INFINITEA, x,
                                    _mm_cmpeq_ps( mHalfSize.mChunkBase[0], MathlibSSE2::INFINITEA ) );
        y = MathlibSSE2::CmovRobust( MathlibSSE2::INFINITEA, y,
                                    _mm_cmpeq_ps( mHalfSize.mChunkBase[1], MathlibSSE2::INFINITEA ) );
        z = MathlibSSE2::CmovRobust( MathlibSSE2::INFINITEA, z,
                                    _mm_cmpeq_ps( mHalfSize.mChunkBase[2], MathlibSSE2::INFINITEA ) );

        mHalfSize = ArrayVector3( x, y, z );
    }
    
}
