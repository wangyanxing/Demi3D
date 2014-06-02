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
        HonFxerApp::GetEditorManager()->SetMenuHost(this);

        menu->removeAllItems();
        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "removeObj");
    }

    void DiEmitterBaseObj::OnDestroy()
    {
        auto parent = dynamic_cast<DiParticleElementObj*>(GetParent());
        parent->GetParticleElement()->DestroyEmitter(mEmitter);
    }

    void DiEmitterBaseObj::OnSelect()
    {
    }

    DiString DiEmitterBaseObj::GetUICaption()
    {
        return mEmitter->GetName();
    }

    void DiEmitterBaseObj::OnCreate()
    {
        auto parent = dynamic_cast<DiParticleElementObj*>(GetParent());
        mEmitter = parent->GetParticleElement()->CreateEmitter(GetEmitterType());
        mEmitter->SetName(HonFxerApp::GetEditorManager()->GenerateEmitterName(GetEmitterType()));
    }
}