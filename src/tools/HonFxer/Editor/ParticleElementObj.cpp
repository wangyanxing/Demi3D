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
#include "ParticleElementObj.h"
#include "ParticleSystemObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "PropertyTypes.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "ParticleElement.h"
#include "EnumProperties.h"

namespace Demi
{
    DiParticleElementObj::DiParticleElementObj()
    {
    }

    DiParticleElementObj::~DiParticleElementObj()
    {
    }

    void DiParticleElementObj::OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection)
    {
        DiEditorManager::Get()->SetCurrentSelection(this);

        menu->removeAllItems();
        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "removeObj");

        if (!multiSelection)
        {
            {
                menu->addItem("", MyGUI::MenuItemType::Separator);
                auto item = menu->addItem("Create Emitters", MyGUI::MenuItemType::Popup);
                auto subMenu = menu->createItemChild(item);
                subMenu->addItem("Point" , MyGUI::MenuItemType::Normal, "createChild PointEmitter"); 
                subMenu->addItem("Circle", MyGUI::MenuItemType::Normal, "createChild CircleEmitter");
                subMenu->addItem("Box"   , MyGUI::MenuItemType::Normal, "createChild BoxEmitter");
                subMenu->addItem("Line"  , MyGUI::MenuItemType::Normal, "createChild LineEmitter");
            }
            {
                menu->addItem("", MyGUI::MenuItemType::Separator);
                auto item = menu->addItem("Create Controllers", MyGUI::MenuItemType::Popup);
                auto subMenu = menu->createItemChild(item);
                subMenu->addItem("Scale"          , MyGUI::MenuItemType::Normal, "createChild ScaleController");
                subMenu->addItem("Color"          , MyGUI::MenuItemType::Normal, "createChild ColorController");
                subMenu->addItem("TextureRotator" , MyGUI::MenuItemType::Normal, "createChild TextureRotatorController");
                subMenu->addItem("GeometryRotator", MyGUI::MenuItemType::Normal, "createChild GeometryRotatorController");
                subMenu->addItem("Gravity"        , MyGUI::MenuItemType::Normal, "createChild GravityController");
                subMenu->addItem("Jet"            , MyGUI::MenuItemType::Normal, "createChild JetController");
                subMenu->addItem("Randomiser"     , MyGUI::MenuItemType::Normal, "createChild RandomiserController");
                subMenu->addItem("Vortex"         , MyGUI::MenuItemType::Normal, "createChild VortexController");
                subMenu->addItem("LinearForce"    , MyGUI::MenuItemType::Normal, "createChild LinearForceController");
                subMenu->addItem("SineForce"      , MyGUI::MenuItemType::Normal, "createChild SineForceController");
                subMenu->addItem("PlaneCollider"  , MyGUI::MenuItemType::Normal, "createChild PlaneColliderController");
                subMenu->addItem("BoxCollider"    , MyGUI::MenuItemType::Normal, "createChild BoxColliderController");
                subMenu->addItem("SphereCollider" , MyGUI::MenuItemType::Normal, "createChild SphereColliderController");
            }
        }
    }

    void DiParticleElementObj::OnCreate()
    {
        DiBaseEditorObj::OnCreate();
        
        DI_ASSERT(!mParticleElement);
        
        // particle element
        auto parent = dynamic_cast<DiParticleSystemObj*>(GetParent());
        mParticleElement = parent->GetParticleSystem()->CreateElement();
        mParticleElement->SetName(DiEditorManager::Get()->GenerateElementName());
        mParticleElement->SetRenderer("Billboard");
        
        // material
        auto mat = DiMaterial::QuickCreate("fx_v", "fx_p");
        mat->SetDepthWrite(false);
        mParticleElement->SetMaterialName(mat->GetName());
    }
    
    void DiParticleElementObj::OnCreate(const DiAny& param)
    {
        DiBaseEditorObj::OnCreate();
        
        mParticleElement = any_cast<DiParticleElement*>(param);
        
        DI_ASSERT(mParticleElement);
        if(mParticleElement->GetName().empty())
        {
            mParticleElement->SetName(DiEditorManager::Get()->GenerateElementName());
        }
    }

    void DiParticleElementObj::OnDestroy()
    {
        auto parent = dynamic_cast<DiParticleSystemObj*>(GetParent());
        parent->GetParticleSystem()->DestroyElement(mParticleElement);
    }

    void DiParticleElementObj::OnSelect()
    {
        DiBaseEditorObj::OnSelect();
    }

    DiString DiParticleElementObj::GetUICaption()
    {
        return mParticleElement->GetName();
    }
    
    void DiParticleElementObj::SetTexture(DiTexturePtr texture)
    {
        auto mat = mParticleElement->GetMaterial();
        mat->GetShaderParameter()->WriteTexture2D("map", texture);
        if(texture)
        {
            texture->SetAddressing(mTextureAddMode);
        }
    }
    
    DiTexturePtr DiParticleElementObj::GetTexture()
    {
        auto mat = mParticleElement->GetMaterial();
        auto maps = mat->GetShaderParameter()->GetShaderParams(DiShaderParameter::VARIABLE_SAMPLER2D);
        auto it = maps.find("map");
        if(it != maps.end())
        {
            DiTexture* tex = any_cast<DiTexture*>(it->second);
            return DiAssetManager::GetInstance().GetAsset<DiTexture>(tex->GetName());
        }
        return nullptr;
    }
    
    void DiParticleElementObj::RefreshTextureParams()
    {
        auto tex = GetTexture();
        if(tex)
        {
            tex->SetAddressing(mTextureAddMode);
        }
    }
    
    void DiParticleElementObj::SetPosition(const DiVec3& pos)
    {
        DiVec3 v = pos;
        *mPositionProp = v;
    }
    
    DiVec3 DiParticleElementObj::GetPosition()
    {
        return *mPositionProp;
    }
    
    void DiParticleElementObj::InitPropertyTable()
    {
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Particle Element");
        
        g->AddProperty("Name"            , DI_NEW DiStringProperty([&]{ return mParticleElement->GetName(); },
                                                                   [&](DiString& val){ mParticleElement->SetName(val); }));
        
        auto prop = g->AddProperty("Position", DI_NEW DiVec3Property(  [&]{ return mParticleElement->position; },
                                                                   [&](DiVec3& val){
                                                                       mParticleElement->position = val;
                                                                       NotifyTransfromUpdate();
                                                                   }));
        mPositionProp = static_cast<DiVec3Property*>(prop->mProperty);
        
        g->AddProperty("Keep Local"      , DI_NEW DiBoolProperty(  [&]{ return mParticleElement->IsKeepLocal(); },
                                                                   [&](bool& val){ mParticleElement->SetKeepLocal(val); }));
        
        g->AddProperty("Particle Quota"  , DI_NEW DiIntProperty(   [&]{ return mParticleElement->GetVisualParticleQuota(); },
                                                                   [&](int& val){ mParticleElement->SetVisualParticleQuota(val); }));
        
        g->AddProperty("Default Width"   , DI_NEW DiFloatProperty( [&]{ return mParticleElement->GetDefaultWidth(); },
                                                                   [&](float& val){ mParticleElement->SetDefaultWidth(val); }));
        
        g->AddProperty("Default Height"  , DI_NEW DiFloatProperty( [&]{ return mParticleElement->GetDefaultHeight(); },
                                                                   [&](float& val){ mParticleElement->SetDefaultHeight(val); }));
        
        g->AddProperty("Default Depth"   , DI_NEW DiFloatProperty( [&]{ return mParticleElement->GetDefaultDepth(); },
                                                                   [&](float& val){ mParticleElement->SetDefaultDepth(val); }));
        
        g->AddProperty("Max Velocity"    , DI_NEW DiFloatProperty( [&]{ return mParticleElement->GetMaxVelocity(); },
                                                                   [&](float& val){ mParticleElement->SetMaxVelocity(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        g = DI_NEW DiPropertyGroup("Material");
        
        g->AddProperty("Texture"     , DI_NEW DiTextureProperty([&]{ return GetTexture(); },
                                                                [&](DiTexturePtr& val){ SetTexture(val); }));
        
        g->AddProperty("Blend Mode"  , DI_NEW DiEnumProperty([&](){ return make_shared<MaterialBlendModeEnum>(mParticleElement->GetMaterial()->GetBlendMode()); },
                                                             [&](DiBaseEnumPropPtr& val){
                                                             mParticleElement->GetMaterial()->SetBlendMode(val->getEnum<MaterialBlendModeEnum,DiBlendMode>());}));
        
        g->AddProperty("Depth Check" , DI_NEW DiBoolProperty([&]{ return mParticleElement->GetMaterial()->GetDepthCheck(); },
                                                             [&](bool& val){ mParticleElement->GetMaterial()->SetDepthCheck(val); }));
        
        g->AddProperty("Depth Write" , DI_NEW DiBoolProperty([&]{ return mParticleElement->GetMaterial()->GetDepthWrite(); },
                                                             [&](bool& val){ mParticleElement->GetMaterial()->SetDepthWrite(val); }));
        
        g->AddProperty("Wireframe"   , DI_NEW DiBoolProperty([&]{ return mParticleElement->GetMaterial()->IsWireframe(); },
                                                             [&](bool& val){ mParticleElement->GetMaterial()->SetWireframe(val); }));
        
        g->AddProperty("Texture Addressing",DI_NEW DiEnumProperty([&](){ return make_shared<TexAddrModeEnum>(mTextureAddMode); },
                                                             [&](DiBaseEnumPropPtr& val){
                                                                 mTextureAddMode = val->getEnum<TexAddrModeEnum,DiAddMode>();
                                                                 RefreshTextureParams();
                                                             }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}