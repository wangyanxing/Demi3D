/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

namespace Demi
{
    inline void Aabb::setExtents( const DiVec3& min, const DiVec3& max )
    {
        assert( (min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
                "The minimum corner of the box must be less than or equal to maximum corner" );
        mCenter = (max + min) * 0.5f;
        mHalfSize   = (max - min) * 0.5f;
    }
    
    inline Aabb Aabb::newFromExtents( const DiVec3& min, const DiVec3& max )
    {
        assert( (min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
                "The minimum corner of the box must be less than or equal to maximum corner" );
        Aabb retVal;
        retVal.mCenter      = (max + min) * 0.5f;
        retVal.mHalfSize    = (max - min) * 0.5f;
        return retVal;
    }
    
    inline DiVec3 Aabb::getMinimum() const
    {
        return mCenter - mHalfSize;
    }
    
    inline DiVec3 Aabb::getMaximum() const
    {
        return mCenter + mHalfSize;
    }
    
    inline DiVec3 Aabb::getSize() const
    {
        return mHalfSize * 2.0f;
    }
    
    inline void Aabb::merge( const Aabb& rhs )
    {
        DiVec3 maxval( mCenter + mHalfSize );
        maxval.makeCeil(rhs.mCenter + rhs.mHalfSize);

        DiVec3 minval( mCenter - mHalfSize );
        minval.makeFloor(rhs.mCenter - rhs.mHalfSize);

        if (maxval.x != std::numeric_limits<float>::infinity() &&
            maxval.y != std::numeric_limits<float>::infinity() &&
            maxval.z != std::numeric_limits<float>::infinity())
        {
            mCenter = (maxval + minval) * 0.5f;
        }
        mHalfSize = (maxval - minval) * 0.5f;
    }
    
    inline void Aabb::merge( const DiVec3& points )
    {
        DiVec3 maxval(mCenter + mHalfSize);
        maxval.makeCeil(points);

        DiVec3 minval(mCenter - mHalfSize);
        minval.makeFloor(points);

        if (maxval.x != std::numeric_limits<float>::infinity() &&
            maxval.y != std::numeric_limits<float>::infinity() &&
            maxval.z != std::numeric_limits<float>::infinity())
        {
            mCenter = (maxval + minval) * 0.5f;
        }
        mHalfSize = (maxval - minval) * 0.5f;
    }
    
    inline bool Aabb::intersects( const Aabb& b2 ) const
    {
        DiVec3 dist( mCenter - b2.mCenter );
        DiVec3 sumHalfSizes( mHalfSize + b2.mHalfSize );

        // ( abs( center.x - center2.x ) <= halfSize.x + halfSize2.x &&
        //   abs( center.y - center2.y ) <= halfSize.y + halfSize2.y &&
        //   abs( center.z - center2.z ) <= halfSize.z + halfSize2.z )
        //TODO: Profile whether '&&' or '&' is faster. Probably varies per architecture.
        return ( DiMath::Abs( dist.x ) <= sumHalfSizes.x ) &
            (DiMath::Abs(dist.y) <= sumHalfSizes.y) &
            (DiMath::Abs(dist.z) <= sumHalfSizes.z);
    }
    
    inline float Aabb::volume(void) const
    {
        const DiVec3 size = mHalfSize * 2.0f;
        return size.x * size.y * size.z; // w * h * d
    }
    
    inline bool Aabb::contains( const Aabb &other ) const
    {
        DiVec3 dist( mCenter - other.mCenter );

        // In theory, "abs( dist.x ) < mHalfSize - other.mHalfSize" should be more pipeline-
        // friendly because the processor can do the subtraction while the abs() is being performed,
        // however that variation won't handle the case where both boxes are infinite (will produce
        // nan instead and return false, when it should return true)

        //TODO: Profile whether '&&' or '&' is faster. Probably varies per architecture.
        return (DiMath::Abs(dist.x) + other.mHalfSize.x <= mHalfSize.x) &
            (DiMath::Abs(dist.y) + other.mHalfSize.y <= mHalfSize.y) &
            (DiMath::Abs(dist.z) + other.mHalfSize.z <= mHalfSize.z);
    }
    
    inline bool Aabb::contains( const DiVec3 &v ) const
    {
        DiVec3 dist( mCenter - v );

        // ( abs( dist.x ) <= mHalfSize.x &&
        //   abs( dist.y ) <= mHalfSize.y &&
        //   abs( dist.z ) <= mHalfSize.z )
        return (DiMath::Abs(dist.x) <= mHalfSize.x) &
            (DiMath::Abs(dist.y) <= mHalfSize.y) &
            (DiMath::Abs(dist.z) <= mHalfSize.z);
    }
    
    inline float Aabb::distance( const DiVec3 &v ) const
    {
        DiVec3 dist( mCenter - v );

        // x = abs( dist.x ) - halfSize.x
        // y = abs( dist.y ) - halfSize.y
        // z = abs( dist.z ) - halfSize.z
        // return max( min( x, y, z ), 0 ); //Return minimum between xyz, clamp to zero

        dist.x = DiMath::Abs( dist.x ) - mHalfSize.x;
        dist.y = DiMath::Abs( dist.y ) - mHalfSize.y;
        dist.z = DiMath::Abs( dist.z ) - mHalfSize.z;

        return DiMath::Max(DiMath::Min(DiMath::Min(dist.x, dist.y), dist.z), 1.0f);
    }
    
    inline void Aabb::transformAffine( const DiMat4 &m )
    {
        assert( m.isAffine() );

        mCenter = m.transformAffine( mCenter );

        mHalfSize = DiVec3(
                DiMath::Abs(m[0][0]) * mHalfSize.x + DiMath::Abs(m[0][1]) * mHalfSize.y + DiMath::Abs(m[0][2]) * mHalfSize.z, 
                DiMath::Abs(m[1][0]) * mHalfSize.x + DiMath::Abs(m[1][1]) * mHalfSize.y + DiMath::Abs(m[1][2]) * mHalfSize.z,
                DiMath::Abs(m[2][0]) * mHalfSize.x + DiMath::Abs(m[2][1]) * mHalfSize.y + DiMath::Abs(m[2][2]) * mHalfSize.z );
    }
    
    inline float Aabb::getRadius() const
    {
        return sqrtf( mHalfSize.dotProduct( mHalfSize ) );
    }
    
    inline float Aabb::getRadiusOrigin() const
    {
        DiVec3 v( mCenter );
        v.makeAbs();
        v += mHalfSize;         
        return v.length();
    }
}
