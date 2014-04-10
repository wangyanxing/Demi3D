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

#include "MiscPch.h"

#include "Math/Array/ArrayVector3.h"

#if __DEMI_HAVE_NEON

namespace Demi
{
    const ArrayVector3 ArrayVector3::ZERO( vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ) );
    const ArrayVector3 ArrayVector3::UNIT_X( vdupq_n_f32( 1.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ) );
    const ArrayVector3 ArrayVector3::UNIT_Y( vdupq_n_f32( 0.0f ), vdupq_n_f32( 1.0f ), vdupq_n_f32( 0.0f ) );
    const ArrayVector3 ArrayVector3::UNIT_Z( vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 1.0f ) );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_X( vdupq_n_f32( -1.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ) );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_Y( vdupq_n_f32( 0.0f ), vdupq_n_f32( -1.0f ), vdupq_n_f32( 0.0f ) );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_Z( vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( -1.0f ) );
    const ArrayVector3 ArrayVector3::UNIT_SCALE( vdupq_n_f32( 1.0f ), vdupq_n_f32( 1.0f ), vdupq_n_f32( 1.0f ) );
}
#endif
