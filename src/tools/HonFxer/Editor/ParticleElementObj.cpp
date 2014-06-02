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

#include "EffectManager.h"
#include "ParticleSystem.h"
#include "ParticleElement.h"

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
        HonFxerApp::GetEditorManager()->SetMenuHost(this);

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
        DI_ASSERT(!mParticleElement);
        
        auto parent = dynamic_cast<DiParticleSystemObj*>(GetParent());
        mParticleElement = parent->GetParticleSystem()->CreateElement();
        mParticleElement->SetName(HonFxerApp::GetEditorManager()->GenerateElementName());
        mParticleElement->SetRenderer("Billboard");
    }

    void DiParticleElementObj::OnDestroy()
    {
        auto parent = dynamic_cast<DiParticleSystemObj*>(GetParent());
        parent->GetParticleSystem()->DestroyElement(mParticleElement);
    }

    void DiParticleElementObj::OnSelect()
    {

    }

    Demi::DiString DiParticleElementObj::GetUICaption()
    {
        return mParticleElement->GetName();
    }
}