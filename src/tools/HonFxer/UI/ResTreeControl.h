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

#ifndef __FXER_RES_TREE_CONTROL_H__
#define __FXER_RES_TREE_CONTROL_H__

#include "FxerPrerequisites.h"
#include "BaseLayout.h"
#include "MyGUI_TreeControl.h"

namespace tools
{
	class ResTreeControl : public wraps::BaseLayout
	{
	public:
        ResTreeControl(MyGUI::Widget* _parent = nullptr);

        virtual ~ResTreeControl();

    public:

        void NotifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);

        void NotifyTreeNodeSelected(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);

        void NotifyTreeNodeContextMenu(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);

        void NotifyMenuCtrlAccept(ContextMenu* _sender, const std::string& _id);

        MyGUI::TreeControl* GetTreeCtrl(){ return mResourcesTree; }

    private:

        MyGUI::TreeControl* mResourcesTree{ nullptr };

        ContextMenu* mContextMenu{ nullptr };
	};

} // namespace tools

#endif // __TOOLS_CONTROL_H__
