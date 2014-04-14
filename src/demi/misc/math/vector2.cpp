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
#include "math/vector2.h"

namespace Demi 
{
    const DiVec2 DiVec2::ZERO( 0, 0);

    const DiVec2 DiVec2::UNIT_X( 1, 0);
    const DiVec2 DiVec2::UNIT_Y( 0, 1);
    const DiVec2 DiVec2::NEGATIVE_UNIT_X( -1,  0);
    const DiVec2 DiVec2::NEGATIVE_UNIT_Y(  0, -1);
    const DiVec2 DiVec2::UNIT_SCALE(1, 1);

    const DiIntVec2 DiIntVec2::ZERO(0,0);
}