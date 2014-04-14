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

#include "MiscPch.h"

#include "Math/Array/ArrayVector3.h"

#if __DEMI_HAVE_SSE

namespace Demi
{
    const ArrayVector3 ArrayVector3::ZERO( _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps() );
    const ArrayVector3 ArrayVector3::UNIT_X( _mm_set1_ps( 1.0f ), _mm_setzero_ps(), _mm_setzero_ps() );
    const ArrayVector3 ArrayVector3::UNIT_Y( _mm_setzero_ps(), _mm_set1_ps( 1.0f ), _mm_setzero_ps() );
    const ArrayVector3 ArrayVector3::UNIT_Z( _mm_setzero_ps(), _mm_setzero_ps(), _mm_set1_ps( 1.0f ) );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_X( _mm_set1_ps( -1.0f ), _mm_setzero_ps(), _mm_setzero_ps() );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_Y( _mm_setzero_ps(), _mm_set1_ps( -1.0f ), _mm_setzero_ps() );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_Z( _mm_setzero_ps(), _mm_setzero_ps(), _mm_set1_ps( -1.0f ) );
    const ArrayVector3 ArrayVector3::UNIT_SCALE( _mm_set1_ps( 1.0f ), _mm_set1_ps( 1.0f ), _mm_set1_ps( 1.0f ) );
}
#endif
