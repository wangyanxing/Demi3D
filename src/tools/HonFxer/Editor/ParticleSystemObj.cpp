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
        HonFxerApp::GetEditorManager()->SetMenuHost(this);

        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "removeObj");

        if (!multiSelection)
        {
            menu->addItem("", MyGUI::MenuItemType::Separator);
            menu->addItem("Create Element", MyGUI::MenuItemType::Popup, "createChild ParticleElement");
        }
    }

    void DiParticleSystemObj::OnCreate()
    {
        DI_ASSERT(!mParticleSystem);
        mParticleSystem = DiEffectManager::GetInstance().CreateParticleSystemTemplate(
            HonFxerApp::GetEditorManager()->GenerateSystemName());
    }

    void DiParticleSystemObj::OnDestroy()
    {
        DiEffectManager::GetInstance().DestroyParticleSystemTemplate(mParticleSystem->GetTemplateName());
    }

    void DiParticleSystemObj::OnSelect()
    {

    }
}