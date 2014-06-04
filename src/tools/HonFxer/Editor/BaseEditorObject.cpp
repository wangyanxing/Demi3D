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
#include "ResTreeControl.h"
#include "MainWorkspaceControl.h"
#include "MainPaneControl.h"

namespace Demi
{
    DiBaseEditorObj::DiBaseEditorObj()
    {
    }

    DiBaseEditorObj::~DiBaseEditorObj()
    {
        for (auto p : mPropGroups)
            DI_DELETE p;
        mPropGroups.clear();

        for (auto ch : mChildren)
            DI_DELETE ch;
        mChildren.clear();

        OnDestroyUI();
        OnDestroy();
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
        ret->OnCreateUI();

        mChildren.push_back(ret);
        return ret;
    }

    void DiBaseEditorObj::RemoveChild(const DiBaseEditorObj* child)
    {
        auto it = std::find_if(mChildren.begin(), mChildren.end(),
            [child](const DiBaseEditorObj* o) { return o == child; });
        if (it != mChildren.end())
        {
            mChildren.erase(it);
        }
        else
        {
            DI_WARNING("Cannot remove the editor object");
        }
    }

    void DiBaseEditorObj::OnCreateUI()
    {
        auto resTree = HonFxerApp::GetFxApp()->GetMainPane()->GetWorkspaceControl()->GetResourceTree();
        auto treeCtrl = resTree->GetTreeCtrl();

        auto root = !mParent ? treeCtrl->getRoot() : mParent->GetUINode();
        mUINode = new MyGUI::TreeControl::Node(GetUICaption().c_str(), GetUINodeType().c_str());
        mUINode->setData((DiBaseEditorObj*)this);
        root->add(mUINode);
    }

    void DiBaseEditorObj::OnDestroyUI()
    {
        mUINode->getParent()->remove(mUINode, true);
    }

    void DiBaseEditorObj::Update(float dt)
    {
        for (auto ch : mChildren)
        {
            ch->Update(dt);
        }
    }
}