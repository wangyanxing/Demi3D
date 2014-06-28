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
#include "EditorToolTip.h"

namespace tools
{
	ToolTip::ToolTip()
	{
        mWrap = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>("ClientTileSkin", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default, "Main");
        mText = mWrap->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(4, 4, 100, 100), MyGUI::Align::Default);
        mWrap->setVisible(false);
	}
    
    ToolTip::~ToolTip()
    {
        MyGUI::Gui::getInstance().destroyWidget(mWrap);
    }
    
    void ToolTip::move(const MyGUI::IntPoint& _point)
    {
        mWrap->setPosition(_point);
    }
    
	void ToolTip::show(const std::string& data, const MyGUI::IntPoint& _point)
	{
		if (data.empty())
			return;
        
        mText->setCaption(data);
        
		MyGUI::ISubWidgetText* text = mText->getSubWidgetText();
		MyGUI::IntSize text_size = text ? text->getTextSize() : MyGUI::IntSize();

		mText->setSize(text_size);
        
        text_size.width += 8;
        text_size.height += 8;
        
		mWrap->setSize(text_size);
        
		mWrap->setVisible(true);
        move(_point);
    }
    
	void ToolTip::hide()
	{
		mWrap->setVisible(false);
	}
    
} // namespace demo
