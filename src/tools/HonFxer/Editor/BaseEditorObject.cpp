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
        menu->removeAllItems();
        menu->addItem("Delete", MyGUI::MenuItemType::Normal, "Delete");
    }

    DiBaseEditorObj* DiBaseEditorObj::_CreateChild(const DiString& type)
    {
        DI_LOG("Creating child object [type = %s]", type.c_str());
        auto ret = HonFxerApp::GetEditorManager()->CreateEditorObject(type);
        ret->mParent = this;
        mChildren.push_back(ret);
        return ret;
    }
}