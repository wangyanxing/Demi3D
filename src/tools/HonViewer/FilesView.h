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

#ifndef __FILES_VIEW_H__
#define __FILES_VIEW_H__

#include "ViewerPrerequisites.h"
#include "BaseLayout.h"
#include "MyGUI_TreeControl.h"

namespace tools
{
	class FilesView :
		public wraps::BaseLayout
	{
	public:
        FilesView(MyGUI::Widget* _parent = nullptr);
        virtual ~FilesView();

    public:

        void notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);

        void notifyTreeNodeSelected(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);

    private:

        void scanFiles();

	private:
        MyGUI::TreeControl* mResourcesTree;

        DiFileTree* mResources;
	};

} // namespace tools

#endif // __TOOLS_CONTROL_H__
