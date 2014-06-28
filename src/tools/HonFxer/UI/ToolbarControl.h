
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

#ifndef __TOOLBAR_CONTROL_H__
#define __TOOLBAR_CONTROL_H__

#include "FxerPrerequisites.h"
#include "BaseLayout.h"
#include "EditorToolTip.h"

namespace tools
{
	class ToolbarControl : public wraps::BaseLayout
	{
	public:
		ToolbarControl(MyGUI::Widget* _parent = nullptr);

		virtual ~ToolbarControl();

		void setVisible(bool _value);
        
    public:
        
        void addToolButton(const std::string& skin, const std::string& tip);
        
        void addSperator();
        
        void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
        
        void notifyButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

    private:
        MyGUI::Widget* mBar{ nullptr };
        
        int mCurrentWidth{ 10 };
        
        DiMap<MyGUI::Widget*, std::string> mTooltip;
        
        DiMap<MyGUI::Widget*, std::string> mToolCommand;
        
        ToolTip* mTipControl{ nullptr };
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
