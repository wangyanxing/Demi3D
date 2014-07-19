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

#include "FxerPch.h"
#include "EnumProperties.h"

namespace Demi
{
    std::vector<DiString> ColorCtrlOperationEnum::Strings = {"Multiply", "Set"};
    std::vector<DiString> MaterialBlendModeEnum::Strings  = {"Replace", "Add", "Multiply", "Alpha", "Transparent Color", "One Inverse Alpha"};
    std::vector<DiString> TexAddrModeEnum::Strings = {"Wrap", "Mirror", "Clamp", "Border", "Mirror Once"};
    std::vector<DiString> ForceApplicationEnum::Strings = {"Average", "Add"};
    std::vector<DiString> IntersectionTypeEnum::Strings = {"Point", "Box"};
    std::vector<DiString> CollisionTypeEnum::Strings = {"None", "Bounce", "Flow"};
    std::vector<DiString> BillboardTypeTypeEnum::Strings = {"Point", "Oriented Common", "Oriented Self", "Perpendicular Common", "Perpendicular Self", "Oriented Shape"};
}
