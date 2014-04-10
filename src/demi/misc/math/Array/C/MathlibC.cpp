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

#include "OgrePlatformInformation.h"

#define __Mathlib_H__ //Neded to directly include OgreMathlibC

#include "Math/Array/ArrayConfig.h"
#include "Math/Array/C/MathlibC.h"
#include "Math/Array/BooleanMask.h"

namespace Demi
{
    const ArrayReal MathlibC::HALF      = 0.5f;
    const ArrayReal MathlibC::ONE       = 1.0f;
    const ArrayReal MathlibC::THREE     = 3.0f;
    const ArrayReal MathlibC::NEG_ONE   = -1.0f;
    const ArrayReal MathlibC::fEpsilon  = 1e-6f;
    const ArrayReal MathlibC::fSqEpsilon= 1e-12f;
    const ArrayReal MathlibC::OneMinusEpsilon= 1.0f - 1e-6f;
    const ArrayReal MathlibC::FLOAT_MIN = std::numeric_limits<Real>::min();
    const ArrayReal MathlibC::SIGN_MASK = -0.0f;
    const ArrayReal MathlibC::INFINITEA = std::numeric_limits<Real>::infinity();
    const ArrayReal MathlibC::MAX_NEG   = -std::numeric_limits<Real>::max();
    const ArrayReal MathlibC::MAX_POS   = std::numeric_limits<Real>::max();

    static const Real _PI = Real( 4.0 * atan( 1.0 ) );
    //We can't use Math::fDeg2Rad & Math::fRad2Deg directly because
    //it's not guaranteed to have been initialized first
    const ArrayReal MathlibC::PI        = _PI;
    const ArrayReal MathlibC::TWO_PI    = 2.0f * _PI;
    const ArrayReal MathlibC::fDeg2Rad  = _PI / 180.0f;
    const ArrayReal MathlibC::fRad2Deg  = 180.0f / _PI;

    const ArrayReal MathlibC::ONE_DIV_2PI= 1.0f / (2.0f * _PI);
}
