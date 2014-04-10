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
#include "MiscPch.h"
#include "math/vector3.h"

namespace Demi 
{
    const DiVec3 DiVec3::ZERO( 0, 0, 0 );
    const DiVec3 DiVec3::UNIT_X( 1, 0, 0 );
    const DiVec3 DiVec3::UNIT_Y( 0, 1, 0 );
    const DiVec3 DiVec3::UNIT_Z( 0, 0, 1 );
    const DiVec3 DiVec3::NEGATIVE_UNIT_X( -1,  0,  0 );
    const DiVec3 DiVec3::NEGATIVE_UNIT_Y(  0, -1,  0 );
    const DiVec3 DiVec3::NEGATIVE_UNIT_Z(  0,  0, -1 );
    const DiVec3 DiVec3::UNIT_SCALE(1, 1, 1);

    const DiIntVec3 DiIntVec3::ZERO(0,0,0);

}