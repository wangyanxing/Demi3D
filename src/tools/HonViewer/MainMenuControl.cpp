/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "ViewerPch.h"
#include "MainMenuControl.h"
#include "Localise.h"
#include "CommandManager.h"

namespace tools
{
	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mBar(nullptr)
	{
		assignWidget(mBar, "Menu");

		createMainMenu();
	}

	MainMenuControl::~MainMenuControl()
	{
	}

	void MainMenuControl::createMainMenu()
	{
        mBar->eventMenuCtrlAccept += newDelegate(this, &MainMenuControl::notifyPopupMenuAccept);
    }

	void MainMenuControl::setVisible(bool _value)
	{
		mBar->setVisible(_value);
	}

    void MainMenuControl::notifyPopupMenuAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
    {
        MyGUI::UString* data = _item->getItemData<MyGUI::UString>(false);
        if (data != nullptr)
            CommandManager::getInstance().setCommandData(*data);

        const std::string& command = _item->getItemId();
        if (command.size() > 8 && command.substr(0, 8) == "Command_")
        {
            CommandManager::getInstance().executeCommand(command);
        }
    }

} // namespace tools
