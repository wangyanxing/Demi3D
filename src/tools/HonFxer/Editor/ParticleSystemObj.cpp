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
#include "ParticleSystemObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "PropertyTypes.h"
#include "EnumProperties.h"
#include "RefModelObj.h"
#include "PropertyItem.h"

namespace Demi
{
    DiParticleSystemObj::DiParticleSystemObj()
    {
    }

    DiParticleSystemObj::~DiParticleSystemObj()
    {
    }

    void DiParticleSystemObj::OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection)
    {
        DiEditorManager::Get()->SetCurrentSelection(this);

        menu->removeAllItems();
        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "removeObj");

        if (!multiSelection)
        {
            menu->addItem("", MyGUI::MenuItemType::Separator);
            menu->addItem("Create Element", MyGUI::MenuItemType::Normal, "createChild ParticleElement");
        }
    }

    void DiParticleSystemObj::OnCreate()
    {
        DiBaseEditorObj::OnCreate();
        
        DI_ASSERT(!mParticleSystem);
        mParticleSystem = DiEffectManager::GetInstance().CreateParticleSystemTemplate(
            DiEditorManager::Get()->GenerateSystemName());

        mSceneNode->AttachObject(mParticleSystem);
        mParticleSystem->Start();
    }
    
    void DiParticleSystemObj::OnCreate(const DiAny& param)
    {
        DiBaseEditorObj::OnCreate();
        
        mParticleSystem = any_cast<DiParticleSystemPtr>(param);
        DI_ASSERT(mParticleSystem);
        
        if(!mParticleSystem)
        {
            DI_ERROR("Cannot init the DiParticleSystemObj with a null particle system pointer");
            return;
        }
        
        mSceneNode->AttachObject(mParticleSystem);
        mParticleSystem->Start();
    }

    void DiParticleSystemObj::OnDestroy()
    {
        mParticleSystem->Stop();
        DiEffectManager::GetInstance().DestroyParticleSystemTemplate(mParticleSystem->GetTemplateName());
        mParticleSystem.reset();
    }

    void DiParticleSystemObj::OnSelect()
    {
        DiBaseEditorObj::OnSelect();
    }

    DiString DiParticleSystemObj::GetUICaption()
    {
        return mParticleSystem->GetName();
    }

    void DiParticleSystemObj::Update(float dt)
    {
        DiBaseEditorObj::Update(dt);
    }
    
    void DiParticleSystemObj::ChangeRefModel()
    {
        if(!mParticleSystem)
            return;
        
        DiK2ModelPtr md = nullptr;

        auto root = DiEditorManager::Get()->GetRootObject();
        size_t children = root->GetNumChildren();
        for(size_t i = 0; i < children; ++i)
        {
            if(root->GetChild(i)->GetUICaption() == mAttachedRefModel)
            {
                md = dynamic_cast<DiRefModelObj*>(root->GetChild(i))->GetModel();
                break;
            }
        }
        
        mParticleSystem->AttachModel(md);
        DiEditorManager::Get()->TriggerEvent("AttachRefModel");
    }
    
    void DiParticleSystemObj::InitPropertyTable()
    {
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Particle System");
        
        g->AddProperty("Name"            , DI_NEW DiStringProperty([&]{ return mParticleSystem->GetTemplateName(); },
                                                                   [&](DiString& val){ mParticleSystem->SetTemplateName(val); }));
        
        DiEditProperty* prop = g->AddProperty("Position", DI_NEW DiVec3Property([&]{ return mSceneNode->GetPosition(); },
                                                                     [&](DiVec3& val){
                                                                         mSceneNode->SetPosition(val);
                                                                         NotifyTransfromUpdate();
                                                                     }));
        mPositionProp = static_cast<DiVec3Property*>(prop->mProperty);
        
        g->AddProperty("Keep Local"      , DI_NEW DiBoolProperty(  [&]{ return mParticleSystem->IsKeepLocal(); },
                                                                   [&](bool& v){ mParticleSystem->SetKeepLocal(v); }));
        
        g->AddProperty("Scale Velocity"  , DI_NEW DiFloatProperty( [&]{ return mParticleSystem->GetScaleVelocity(); },
                                                                   [&](float& v){ mParticleSystem->SetScaleVelocity(v); }));
        
        g->AddProperty("Scale Time"      , DI_NEW DiFloatProperty( [&]{ return mParticleSystem->GetScaleTime(); },
                                                                   [&](float& v){ mParticleSystem->SetScaleTime(v); }));
        
        g->AddProperty("Scale"           , DI_NEW DiVec3Property(  [&]{ return mParticleSystem->GetScale(); },
                                                                   [&](DiVec3& v){ mParticleSystem->SetScale(v); }));
        
        g->AddProperty("Attached Model"  , DI_NEW DiDynEnumProperty([&]
            {
                DiDynEnumPropPtr ret;
                if(!mAttachedRefModel.empty())
                    ret = make_shared<RefModelsNamesEnum>(mAttachedRefModel);
                else
                    ret = make_shared<RefModelsNamesEnum>();
                if(ret)
                    ret->eventType = "RefModel";
                return ret;
            },
            [&](DiDynEnumPropPtr& v){
                mAttachedRefModel = v->value;
                ChangeRefModel();
            }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}