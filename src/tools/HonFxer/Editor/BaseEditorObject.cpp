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
#include "BaseEditorObject.h"
#include "HonFxerApp.h"
#include "EditorManager.h"

namespace Demi
{
    DiBaseEditorObj::DiBaseEditorObj()
    {
    }

    DiBaseEditorObj::~DiBaseEditorObj()
    {
    }

    void DiBaseEditorObj::OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection)
    {
        HonFxerApp::GetEditorManager()->SetMenuHost(this);

        menu->removeAllItems();

        menu->addItem("Create ParticleSystem", MyGUI::MenuItemType::Normal, "createChild ParticleSystem");
        //menu->addItem("Create Model", MyGUI::MenuItemType::Normal, "createChild Model");
    }

    DiBaseEditorObj* DiBaseEditorObj::_CreateChild(const DiString& type)
    {
        DI_LOG("Creating child object [type = %s]", type.c_str());
        auto ret = HonFxerApp::GetEditorManager()->CreateEditorObject(type);
        
        ret->mParent = this;
        ret->OnCreate();

        mChildren.push_back(ret);
        return ret;
    }

    void DiBaseEditorObj::RemoveChild(const DiBaseEditorObj* child)
    {
        auto it = std::find_if(mChildren.begin(), mChildren.end(),
            [child](const DiBaseEditorObj* o) { return o == child; });
        if (it != mChildren.end())
        {
            (*it)->OnDestroy();
            SAFE_DELETE(*it);
            mChildren.erase(it);
        }
        else
        {
            DI_WARNING("Cannot remove the editor object");
        }
    }
}