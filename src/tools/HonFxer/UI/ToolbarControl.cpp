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
#include "ToolbarControl.h"
#include "Localise.h"
#include "CommandManager.h"
#include "EditorToolTip.h"

namespace tools
{
	ToolbarControl::ToolbarControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("FxToolbarControl.layout", _parent)
	{
        addToolButton("ToolNewFile", "New File");
        addToolButton("ToolOpenFile", "Open File");
        addToolButton("ToolSaveFile", "Save File");
        addToolButton("ToolReset", "Reset Effects");
        
        addSperator();
        
        addToolButton("ToolUndo", "Undo");
        addToolButton("ToolRedo", "Redo");
        
        addSperator();
        
        addToolButton("ToolCut", "Cut");
        addToolButton("ToolCopy", "Copy");
        addToolButton("ToolPaste", "Paste");
        
        addSperator();

        addToolButton("ToolSelect", "Select");
        addToolButton("ToolMove", "Move");
        addToolButton("ToolRotate", "Rotate");
        addToolButton("ToolScale", "Scale");
        
        addSperator();
        
        addToolButton("ToolPlay", "Play effect");
        addToolButton("ToolPause", "Pause effect");
        addToolButton("ToolStop", "Stop effect");
        
        addSperator();
        
        addToolButton("ToolDelete", "Delete current selection");
        
        mTipControl = DI_NEW ToolTip();
	}

	ToolbarControl::~ToolbarControl()
	{
        DI_DELETE mTipControl;
	}

	void ToolbarControl::setVisible(bool _value)
	{
		mBar->setVisible(_value);
	}
    
    void ToolbarControl::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
    {
        auto tipStr = mTooltip[_sender];
        
        if (_info.type == MyGUI::ToolTipInfo::Show)
		{
            mTipControl->show(tipStr, _info.point);
        }
		else if (_info.type == MyGUI::ToolTipInfo::Hide)
		{
			mTipControl->hide();
		}
		else if (_info.type == MyGUI::ToolTipInfo::Move)
		{
			mTipControl->move(_info.point);
		}
    }
    
    void ToolbarControl::addToolButton(const std::string& skin, const std::string& tip)
    {
        auto button = mMainWidget->createWidget<MyGUI::Button>(skin, MyGUI::IntCoord(mCurrentWidth, 7, 25, 25), MyGUI::Align::Default);
        
        if(!tip.empty())
        {
            button->setNeedToolTip(true);
            button->eventToolTip = newDelegate(this, &ToolbarControl::notifyToolTip);
            mTooltip[button] = tip;
        }
        
        button->eventMouseButtonPressed += MyGUI::newDelegate(this, &ToolbarControl::notifyButtonPressed);
        
        std::string command = "Command_";
        command += skin;
        mToolCommand[button] = command;
        
        mCurrentWidth += 30;
    }
    
    void ToolbarControl::notifyButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
    {
        auto command = mToolCommand[_sender];
        if(command.empty())
        {
            return;
        }
        
        CommandManager::getInstance().executeCommand(command);
    }
    
    void ToolbarControl::addSperator()
    {
        mMainWidget->createWidget<MyGUI::Button>("ToolSperator", MyGUI::IntCoord(mCurrentWidth, 7, 25, 25), MyGUI::Align::Default);
        mCurrentWidth += 30;
    }

} // namespace tools
