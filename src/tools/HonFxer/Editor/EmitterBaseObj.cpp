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
#include "EmitterBaseObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "ParticleEmitter.h"
#include "PropertyItem.h"
#include "K2Clip.h"
#include "K2Model.h"
#include "EnumProperties.h"

namespace Demi
{
    DiEmitterBaseObj::DiEmitterBaseObj()
    {
    }

    DiEmitterBaseObj::~DiEmitterBaseObj()
    {
    }

    void DiEmitterBaseObj::OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection)
    {
        DiEditorManager::Get()->SetCurrentSelection(this);

        menu->removeAllItems();
        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "removeObj");
    }

    void DiEmitterBaseObj::OnDestroy()
    {
        mEmitter->SetParentElement(nullptr);
        auto parent = dynamic_cast<DiParticleElementObj*>(GetParent());
        parent->GetParticleElement()->DestroyEmitter(mEmitter);
    }

    void DiEmitterBaseObj::OnSelect()
    {
        DiBaseEditorObj::OnSelect();
    }

    DiString DiEmitterBaseObj::GetUICaption()
    {
        return mEmitter->GetName();
    }
    
    DiK2Skeleton* DiEmitterBaseObj::GetAttachedSkeleton()
    {
        return mEmitter ? mEmitter->GetAttachedSkeleton() : nullptr;
    }

    void DiEmitterBaseObj::OnCreate()
    {
        DiBaseEditorObj::OnCreate();
        
        auto parent = dynamic_cast<DiParticleElementObj*>(GetParent());
        mEmitter = parent->GetParticleElement()->CreateEmitter(GetEmitterType());
        mEmitter->SetName(DiEditorManager::Get()->GenerateEmitterName(GetEmitterType()));
    }
    
    void DiEmitterBaseObj::OnCreate(const DiAny& param)
    {
        DiBaseEditorObj::OnCreate();
        
        mEmitter = any_cast<DiParticleEmitter*>(param);
        
        DI_ASSERT(mEmitter);
        if(mEmitter->GetName().empty())
        {
            mEmitter->SetName(DiEditorManager::Get()->GenerateEmitterName(GetEmitterType()));
        }
    }
    
    void DiEmitterBaseObj::ChangeAttachBone()
    {
        
    }
    
    void DiEmitterBaseObj::InitPropertyTable()
    {
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Emitter");
     
        g->AddProperty("Name"            , DI_NEW DiStringProperty([&]{ return mEmitter->GetName(); },
                                                                [&](DiString& val){ mEmitter->SetName(val); }));

        g->AddProperty("Emission Rate"   , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynEmissionRate(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynEmissionRate(val); }));

        g->AddProperty("Angle"           , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynAngle(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynAngle(val); }));

        g->AddProperty("Life"            , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynTotalTimeToLive(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynTotalTimeToLive(val); }));

        g->AddProperty("Mass"            , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynParticleMass(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynParticleMass(val); }));

        g->AddProperty("Velocity"        , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynVelocity(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynVelocity(val); }));

        g->AddProperty("Duration"        , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynDuration(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynDuration(val); }));

        g->AddProperty("Repeat Delay"    , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynRepeatDelay(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynRepeatDelay(val); }));

        g->AddProperty("Dimensions"      , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynParticleAllDimensions(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynParticleAllDimensions(val); }));

        g->AddProperty("Width"           , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynParticleWidth(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynParticleWidth(val); }));

        g->AddProperty("Height"          , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynParticleHeight(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynParticleHeight(val); }));

        g->AddProperty("Depth"           , DI_NEW DiDynProperty([&]{ return mEmitter->GetDynParticleDepth(); },
                                                                [&](DiDynamicAttribute*& val){ mEmitter->SetDynParticleDepth(val); }));

        g->AddProperty("Original Enabled", DI_NEW DiBoolProperty([&]{ return mEmitter->GetOriginalEnabled(); },
                                                                 [&](bool& val){ mEmitter->SetOriginalEnabled(val); }));

        auto prop = g->AddProperty("Position", DI_NEW DiVec3Property([&]
                 {
                     return mEmitter->position;
                 },
                 [&](DiVec3& val){
                     mEmitter->position = val;
                     NotifyTransfromUpdate();
                 }));
        mPositionProp = static_cast<DiVec3Property*>(prop->mProperty);

        g->AddProperty("Keep Local"      , DI_NEW DiBoolProperty([&]{ return mEmitter->IsKeepLocal(); },
                                                                 [&](bool& val){ mEmitter->SetKeepLocal(val); }));

        g->AddProperty("Orientation"     , DI_NEW DiQuatProperty([&]{ return mEmitter->GetParticleOrientation(); },
                                                                 [&](DiQuat& val){ mEmitter->SetParticleOrientation(val); }));

        g->AddProperty("Color"           , DI_NEW DiColorProperty([&]{ return mEmitter->GetParticleColour(); },
                                                                  [&](DiColor& val){ mEmitter->SetParticleColour(val); }));

        g->AddProperty("Auto Direction"  , DI_NEW DiBoolProperty([&]{ return mEmitter->IsAutoDirection(); },
                                                                 [&](bool& val){ mEmitter->SetAutoDirection(val); }));

        g->AddProperty("Force Emission"  , DI_NEW DiBoolProperty([&]{ return mEmitter->IsForceEmission(); },
                                                                 [&](bool& val){ mEmitter->SetForceEmission(val); }));
     
        g->AddProperty("Bones"           , DI_NEW DiDynEnumProperty([&]
                {
                    DiDynEnumPropPtr ret;
                    if(!mBoneName.empty())
                        ret = make_shared<K2ModelBoneNamesEnum>(mEmitter, mBoneName);
                    else
                        ret = make_shared<K2ModelBoneNamesEnum>(mEmitter);
                    if(ret)
                        ret->eventType = "AttachRefModel";
                    return ret;
                },
                [&](DiDynEnumPropPtr& v){
                    mBoneName = v->value;
                    ChangeAttachBone();
                }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}