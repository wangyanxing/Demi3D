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

#include "K2Clip.h"
#include "Node.h"
#include "EditorManager.h"
#include "ParticleEmitter.h"
#include "BaseEditorObject.h"

namespace Demi
{
    std::vector<DiString> ColorCtrlOperationEnum::Strings = {"Multiply", "Set"};
    std::vector<DiString> MaterialBlendModeEnum::Strings  = {"Replace", "Add", "Multiply", "Alpha", "Transparent Color", "One Inverse Alpha"};
    std::vector<DiString> TexAddrModeEnum::Strings = {"Wrap", "Mirror", "Clamp", "Border", "Mirror Once"};
    std::vector<DiString> ForceApplicationEnum::Strings = {"Average", "Add"};
    std::vector<DiString> IntersectionTypeEnum::Strings = {"Point", "Box"};
    std::vector<DiString> CollisionTypeEnum::Strings = {"None", "Bounce", "Flow"};
    std::vector<DiString> BillboardTypeEnum::Strings = {"Point", "Oriented Common", "Oriented Self",
                                                        "Perpendicular Common", "Perpendicular Self", "Oriented Shape"};
    
    DiString K2ModelBoneNamesEnum::msNoneBone = "*None*";
    DiString RefModelsNamesEnum::msNoneName   = "*None*";
    
    K2ModelBoneNamesEnum::K2ModelBoneNamesEnum(DiParticleEmitter* emitter, const DiString& v)
    {
        value = v;
        mEmitter = emitter;
        
        RefreshValues();
    }
        
    void K2ModelBoneNamesEnum::RefreshValues()
    {
        mStrings.clear();
        mStrings.push_back(msNoneBone);
        
        auto skeleton = mEmitter->GetAttachedSkeleton();
        if(!skeleton)
        {
            return;
        }
        
        skeleton->IterateBoneNames([this](const DiString& name, DiNode*){
            mStrings.push_back(name);
        });
        
        auto it = std::find(mStrings.begin(), mStrings.end(), value);
        if(it == mStrings.end())
        {
            value = msNoneBone;
        }
    }
    
    RefModelsNamesEnum::RefModelsNamesEnum(const DiString& v)
    {
        value = v;
        RefreshValues();
    }
    
    void RefModelsNamesEnum::RefreshValues()
    {
        mStrings.clear();
        mStrings.push_back(msNoneName);
        
        auto root = DiEditorManager::Get()->GetRootObject();
        size_t size = root->GetNumChildren();
        for(size_t i = 0; i < size; ++i)
        {
            auto cd = root->GetChild(i);
            if(cd->GetType() == "ReferenceModel")
            {
                auto name = cd->GetUICaption();
                mStrings.push_back(name);
            }
        }
        
        auto it = std::find(mStrings.begin(), mStrings.end(), value);
        if(it == mStrings.end())
        {
            value = msNoneName;
        }
    }
}
