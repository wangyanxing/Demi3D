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
        DI_ASSERT(!mParticleSystem);
        mParticleSystem = DiEffectManager::GetInstance().CreateParticleSystemTemplate(
            DiEditorManager::Get()->GenerateSystemName());

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();

        mSceneNode = sm->GetRootNode()->CreateChild();
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
    
    void DiParticleSystemObj::InitPropertyTable()
    {
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Particle System");
        
        g->AddProperty("Name"            , DI_NEW DiStringProperty([&]{ return mParticleSystem->GetTemplateName(); },
                                                                   [&](DiString& val){ mParticleSystem->SetTemplateName(val); }));
        
        g->AddProperty("Keep Local"      , DI_NEW DiBoolProperty(  [&]{ return mParticleSystem->IsKeepLocal(); },
                                                                   [&](bool& val){ mParticleSystem->SetKeepLocal(val); }));
        
        g->AddProperty("Scale Velocity"  , DI_NEW DiFloatProperty( [&]{ return mParticleSystem->GetScaleVelocity(); },
                                                                   [&](float& val){ mParticleSystem->SetScaleVelocity(val); }));
        
        g->AddProperty("Scale Time"      , DI_NEW DiFloatProperty( [&]{ return mParticleSystem->GetScaleTime(); },
                                                                   [&](float& val){ mParticleSystem->SetScaleTime(val); }));
        
        g->AddProperty("Scale"           , DI_NEW DiVec3Property(  [&]{ return mParticleSystem->GetScale(); },
                                                                   [&](DiVec3& val){ mParticleSystem->SetScale(val); }));
    
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}