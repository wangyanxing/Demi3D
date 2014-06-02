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
#include "ResTreeControl.h"
#include "MyGUI_TreeControlItem.h"
#include "ContextMenu.h"

namespace tools
{
    ResTreeControl::ResTreeControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("FxResTreeControl.layout", _parent)
	{
        assignWidget(mResourcesTree, "ResourcesTree");

        mResourcesTree->eventTreeNodePrepare += newDelegate(this, &ResTreeControl::NotifyTreeNodePrepare);
        mResourcesTree->eventTreeNodeSelected += newDelegate(this, &ResTreeControl::NotifyTreeNodeSelected);
        mResourcesTree->eventTreeNodeContextMenu += newDelegate(this, &ResTreeControl::NotifyTreeNodeContextMenu);

        MyGUI::TreeControl::Node* root = mResourcesTree->getRoot();
        root->removeAll();
        MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node("Scene", "Folder");
        root->add(pNode);

        mContextMenu = DI_NEW ContextMenu(mResourcesTree);
        mContextMenu->eventMenuAccept = MyGUI::newDelegate(this, &ResTreeControl::NotifyMenuCtrlAccept);
	}

    ResTreeControl::~ResTreeControl()
	{
        SAFE_DELETE(mContextMenu);
	}

    void ResTreeControl::NotifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
    }

    void ResTreeControl::NotifyTreeNodeSelected(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
    }

    void ResTreeControl::NotifyMenuCtrlAccept(ContextMenu* _sender, const std::string& _id)
    {
    }

    void ResTreeControl::NotifyTreeNodeContextMenu(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
        mContextMenu->setVisible(true);
    }

} // namespace tools
