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
#include "BaseForceController.h"
#include "BaseColliderController.h"
#include "BillboardSet.h"

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
    
    struct TexAddrModeEnum : _DiBaseEnumProperty{
        TexAddrModeEnum() = default;
        TexAddrModeEnum(DiAddMode v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        DiAddMode get(){
            return (DiAddMode)enumValue;
        }
        static std::vector<DiString> Strings;
    };
    
    struct ForceApplicationEnum : _DiBaseEnumProperty{
        ForceApplicationEnum() = default;
        ForceApplicationEnum(DiBaseForceController::ForceApplication v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        DiBaseForceController::ForceApplication get(){
            return (DiBaseForceController::ForceApplication)enumValue;
        }
        static std::vector<DiString> Strings;
    };
    
    struct IntersectionTypeEnum : _DiBaseEnumProperty{
        IntersectionTypeEnum() = default;
        IntersectionTypeEnum(DiBaseColliderController::IntersectionType v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        DiBaseColliderController::IntersectionType get(){
            return (DiBaseColliderController::IntersectionType)enumValue;
        }
        static std::vector<DiString> Strings;
    };
    
    struct CollisionTypeEnum : _DiBaseEnumProperty{
        CollisionTypeEnum() = default;
        CollisionTypeEnum(DiBaseColliderController::CollisionType v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        DiBaseColliderController::CollisionType get(){
            return (DiBaseColliderController::CollisionType)enumValue;
        }
        static std::vector<DiString> Strings;
    };
    
    struct BillboardTypeEnum : _DiBaseEnumProperty{
        BillboardTypeEnum() = default;
        BillboardTypeEnum(BillboardType v){enumValue=v;}
        std::vector<DiString>& GetStrings(){
            return Strings;
        }
        BillboardType get(){
            return (BillboardType)enumValue;
        }
        static std::vector<DiString> Strings;
    };

    struct K2ModelBoneNamesEnum : _DiBaseDynEnumProperty{
        K2ModelBoneNamesEnum() = default;
        
        K2ModelBoneNamesEnum(DiParticleEmitter* emitter, const DiString& v = msNoneBone);
        
        std::vector<DiString>& GetStrings(){
            return mStrings;
        }
        
        void RefreshValues();
        
        std::vector<DiString> mStrings;
        DiParticleEmitter* mEmitter{ nullptr };
        
        static DiString msNoneBone;
    };
 
    struct RefModelsNamesEnum : _DiBaseDynEnumProperty{
        
        RefModelsNamesEnum(const DiString& v = msNoneName);
        
        std::vector<DiString>& GetStrings(){
            return mStrings;
        }
        
        void RefreshValues();
        
        std::vector<DiString> mStrings;
        
        static DiString msNoneName;
    };
}

#endif
