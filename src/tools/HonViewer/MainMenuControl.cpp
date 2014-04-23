/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "ViewerPch.h"
#include "MainMenuControl.h"
#include "Localise.h"

namespace tools
{
	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mBar(nullptr),
		mPopupMenuWidgets(nullptr)
	{
		assignWidget(mBar, "Menu");

		createMainMenu();
	}

	MainMenuControl::~MainMenuControl()
	{
	}

	void MainMenuControl::createMainMenu()
	{
		mPopupMenuWidgets = mBar->findItemById("Widgets", true)->getItemChild();

		mPopupMenuWidgets->setPopupAccept(true);

	}

	void MainMenuControl::setVisible(bool _value)
	{
		mBar->setVisible(_value);
	}

} // namespace tools
