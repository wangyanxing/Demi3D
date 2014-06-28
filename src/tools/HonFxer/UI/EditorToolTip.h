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

#ifndef __TOOL_TIP_H__
#define __TOOL_TIP_H__

#include "FxerPrerequisites.h"

namespace tools
{
	class ToolTip
	{
	public:
		ToolTip();
        ~ToolTip();
        
		void show(const std::string& _data, const MyGUI::IntPoint& _point);
		void hide();
		void move(const MyGUI::IntPoint& _point);
        
	private:
		MyGUI::TextBox* mText{nullptr};
		MyGUI::Widget* mWrap{nullptr};
	};
    
} // namespace demo

#endif // __TOOL_TIP_H__
