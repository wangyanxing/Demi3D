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

#ifndef EnumProperties_h__
#define EnumProperties_h__

#include "FxerPrerequisites.h"
#include "PropertyTypes.h"
#include "ColorController.h"

namespace Demi
{
    struct ColorCtrlOperationEnum : _DiBaseEnumProperty{
        ColorCtrlOperationEnum() = default;
        ColorCtrlOperationEnum(DiColorController::ColourOperation v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        DiColorController::ColourOperation get(){
            return (DiColorController::ColourOperation)enumValue;
        }
        static std::vector<DiString> Strings;
    };
    
    struct MaterialBlendModeEnum : _DiBaseEnumProperty{
        MaterialBlendModeEnum() = default;
        MaterialBlendModeEnum(DiBlendMode v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        DiBlendMode get(){
            return (DiBlendMode)enumValue;
        }
        static std::vector<DiString> Strings;
    };
}

#endif
