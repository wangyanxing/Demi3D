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

#define __Mathlib_H__ //Neded to directly include OgreMathlibC

#include "Math/Array/ArrayConfig.h"
#include "Math/Array/C/MathlibC.h"
#include "Math/Array/BooleanMask.h"

namespace Demi
{
    const ArrayFloat MathlibC::HALF      = 0.5f;
    const ArrayFloat MathlibC::ONE       = 1.0f;
    const ArrayFloat MathlibC::THREE     = 3.0f;
    const ArrayFloat MathlibC::NEG_ONE   = -1.0f;
    const ArrayFloat MathlibC::fEpsilon  = 1e-6f;
    const ArrayFloat MathlibC::fSqEpsilon= 1e-12f;
    const ArrayFloat MathlibC::OneMinusEpsilon= 1.0f - 1e-6f;
    const ArrayFloat MathlibC::FLOAT_MIN = std::numeric_limits<float>::min();
    const ArrayFloat MathlibC::SIGN_MASK = -0.0f;
    const ArrayFloat MathlibC::INFINITEA = std::numeric_limits<float>::infinity();
    const ArrayFloat MathlibC::MAX_NEG = -std::numeric_limits<float>::max();
    const ArrayFloat MathlibC::MAX_POS = std::numeric_limits<float>::max();

    static const float _PI = float(4.0 * atan(1.0));
    //We can't use Math::fDeg2Rad & Math::fRad2Deg directly because
    //it's not guaranteed to have been initialized first
    const ArrayFloat MathlibC::PI        = _PI;
    const ArrayFloat MathlibC::TWO_PI    = 2.0f * _PI;
    const ArrayFloat MathlibC::fDeg2Rad  = _PI / 180.0f;
    const ArrayFloat MathlibC::fRad2Deg  = 180.0f / _PI;

    const ArrayFloat MathlibC::ONE_DIV_2PI= 1.0f / (2.0f * _PI);
}
