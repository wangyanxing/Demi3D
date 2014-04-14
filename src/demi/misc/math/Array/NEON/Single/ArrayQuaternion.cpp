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

#include "Math/Array/ArrayQuaternion.h"

#if __DEMI_HAVE_NEON

namespace Demi
{
    const ArrayQuaternion ArrayQuaternion::ZERO( vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ) );
    const ArrayQuaternion ArrayQuaternion::IDENTITY( vdupq_n_f32( 1.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ), vdupq_n_f32( 0.0f ) );
}

#endif
