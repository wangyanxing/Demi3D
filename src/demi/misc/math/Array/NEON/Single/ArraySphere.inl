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


namespace Demi
{
    
    inline ArrayFloat ArraySphere::intersects( const ArraySphere &s ) const
    {
        ArrayFloat sqRadius  = vaddq_f32( mRadius, s.mRadius );
        sqRadius            = vmulq_f32( sqRadius, sqRadius );
        ArrayFloat sqDist    = mCenter.squaredDistance( s.mCenter );

        return vcleq_f32( sqDist, sqRadius ); // sqDist <= sqRadius
    }
    
    inline ArrayFloat ArraySphere::intersects( const ArrayVector3 &v ) const
    {
        ArrayFloat sqRadius  = vmulq_f32( mRadius, mRadius );
        ArrayFloat sqDist    = mCenter.squaredDistance( v );

        return vcleq_f32( sqDist, sqRadius ); // sqDist <= sqRadius
    }
}
