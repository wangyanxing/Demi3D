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

#if __DEMI_HAVE_NEON
#define __Mathlib_H__ //Needed to directly include MathlibNEORealN

#include "Math/Array/ArrayConfig.h"
#include "Math/Array/NEON/Single/MathlibNEON.h"
#include "Math/Array/NEON/Single/neon_mathfun.h"
#include "Math/Array/BooleanMask.h"

namespace Demi
{
    const ArrayFloat MathlibNEON::HALF       = vdupq_n_f32( 0.5f );
    const ArrayFloat MathlibNEON::ONE        = vdupq_n_f32( 1.0f );
    const ArrayFloat MathlibNEON::THREE      = vdupq_n_f32( 3.0f );
    const ArrayFloat MathlibNEON::NEG_ONE    = vdupq_n_f32( -1.0f );
    const ArrayFloat MathlibNEON::fEpsilon   = vdupq_n_f32( 1e-6f );
    const ArrayFloat MathlibNEON::fSqEpsilon = vdupq_n_f32( 1e-12f );
    const ArrayFloat MathlibNEON::OneMinusEpsilon= vdupq_n_f32( 1.0f - 1e-6f );
    const ArrayFloat MathlibNEON::FLOAT_MIN  = vdupq_n_f32( std::numeric_limits<float>::min() );
    const ArrayFloat MathlibNEON::SIGN_MASK  = vdupq_n_f32( -0.0f );
    const ArrayFloat MathlibNEON::INFINITEA  = vdupq_n_f32( std::numeric_limits<float>::infinity() );
    const ArrayFloat MathlibNEON::MAX_NEG    = vdupq_n_f32( -std::numeric_limits<float>::max() );
    const ArrayFloat MathlibNEON::MAX_POS    = vdupq_n_f32( std::numeric_limits<float>::max() );
    const ArrayFloat MathlibNEON::LAST_AFFINE_COLUMN = (ArrayFloat) { 1, 0, 0, 0 };

    static const float _PI = float( 4.0 * atan( 1.0 ) );
    //We can't use Math::fDeg2Rad & Math::fRad2Deg directly because
    //it's not guaranteed to have been initialized first
    const ArrayFloat MathlibNEON::PI         = vdupq_n_f32( _PI );
    const ArrayFloat MathlibNEON::TWO_PI     = vdupq_n_f32( 2.0f * _PI );
    const ArrayFloat MathlibNEON::fDeg2Rad   = vdupq_n_f32( _PI / 180.0f );
    const ArrayFloat MathlibNEON::fRad2Deg   = vdupq_n_f32( 180.0f / _PI );

    const ArrayFloat MathlibNEON::ONE_DIV_2PI= vdupq_n_f32( 1.0f / (2.0f * _PI) );

    
    ArrayFloat MathlibNEON::Sin4( ArrayFloat x )
    {
        // Map arbitrary angle x to the range [-pi; +pi] without using division.
        // Code taken from MSDN's HLSL trick. Architectures with fused mad (i.e. NEON)
        // can replace the add, the sub, & the two muls for two mad
        ArrayFloat integralPart;
        x = vaddq_f32( vmulq_f32( x, ONE_DIV_2PI ), HALF );
        x = Modf4( x, integralPart );
        x = vsubq_f32( vmulq_f32( x, TWO_PI ), PI );

        return sin_ps( x );
    }
    
    ArrayFloat MathlibNEON::Cos4( ArrayFloat x )
    {
        // Map arbitrary angle x to the range [-pi; +pi] without using division.
        // Code taken from MSDN's HLSL trick. Architectures with fused mad (i.e. NEON)
        // can replace the add, the sub, & the two muls for two mad
        ArrayFloat integralPart;
        x = vaddq_f32( vmulq_f32( x, ONE_DIV_2PI ), HALF );
        x = Modf4( x, integralPart );
        x = vsubq_f32( vmulq_f32( x, TWO_PI ), PI );

        return cos_ps( x );
    }
    
    void MathlibNEON::SinCos4( ArrayFloat x, ArrayFloat &outSin, ArrayFloat &outCos )
    {
        // TODO: Improve accuracy by mapping to the range [-pi/4, pi/4] and swap
        // between cos & sin depending on which quadrant it fell:
        // Quadrant | sin     |  cos
        // n = 0 ->  sin( x ),  cos( x )
        // n = 1 ->  cos( x ), -sin( x )
        // n = 2 -> -sin( x ), -cos( x )
        // n = 3 -> -sin( x ),  sin( x )
        // See ARGUMENT REDUCTION FOR HUGE ARGUMENTS:
        // Good to the Last Bit
        // K. C. Ng and themembers of the FP group of SunPro
        // http://www.derekroconnor.net/Software/Ng--ArgReduction.pdf

        // -- Perhaps we can leave this to GSoC students? --

        // Map arbitrary angle x to the range [-pi; +pi] without using division.
        // Code taken from MSDN's HLSL trick. Architectures with fused mad (i.e. NEON)
        // can replace the add, the sub, & the two muls for two mad
        ArrayFloat integralPart;
        x = vaddq_f32( vmulq_f32( x, ONE_DIV_2PI ), HALF );
        x = Modf4( x, integralPart );
        x = vsubq_f32( vmulq_f32( x, TWO_PI ), PI );

        sincos_ps( x, &outSin, &outCos );
    }

    const ArrayFloat BooleanMask4::mMasks[NUM_MASKS] =
    {
        (ArrayFloat) { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },//MASK_NONE
        (ArrayFloat) { 0xffffffff, 0x00000000, 0x00000000, 0x00000000 },//MASK_X
        (ArrayFloat) { 0x00000000, 0xffffffff, 0x00000000, 0x00000000 },//MASK_Y
        (ArrayFloat) { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000 },//MASK_XY
        (ArrayFloat) { 0x00000000, 0x00000000, 0xffffffff, 0x00000000 },//MASK_Z
        (ArrayFloat) { 0xffffffff, 0x00000000, 0xffffffff, 0x00000000 },//MASK_XZ
        (ArrayFloat) { 0x00000000, 0xffffffff, 0xffffffff, 0x00000000 },//MASK_YZ
        (ArrayFloat) { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 },//MASK_XYZ
        (ArrayFloat) { 0x00000000, 0x00000000, 0x00000000, 0xffffffff },//MASK_W
        (ArrayFloat) { 0xffffffff, 0x00000000, 0x00000000, 0xffffffff },//MASK_XW
        (ArrayFloat) { 0x00000000, 0xffffffff, 0x00000000, 0xffffffff },//MASK_YW
        (ArrayFloat) { 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff },//MASK_XYW
        (ArrayFloat) { 0x00000000, 0x00000000, 0xffffffff, 0xffffffff },//MASK_ZW
        (ArrayFloat) { 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff },//MASK_XZW
        (ArrayFloat) { 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff },//MASK_YZW
        (ArrayFloat) { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff } //MASK_XYZW
    };
}
#endif
