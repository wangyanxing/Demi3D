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

namespace Demi
{
    const ArrayVector3 ArrayVector3::ZERO( 0, 0, 0 );
    const ArrayVector3 ArrayVector3::UNIT_X( 1, 0, 0 );
    const ArrayVector3 ArrayVector3::UNIT_Y( 0, 1, 0 );
    const ArrayVector3 ArrayVector3::UNIT_Z( 0, 0, 1 );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_X( -1, 0, 0 );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_Y( 0, -1, 0 );
    const ArrayVector3 ArrayVector3::NEGATIVE_UNIT_Z( 0, 0, -1 );
    const ArrayVector3 ArrayVector3::UNIT_SCALE( 1, 1, 1 );
}
