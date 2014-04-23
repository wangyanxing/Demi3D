/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_MENU_CONTROL_H__
#define __MAIN_MENU_CONTROL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace tools
{
	class MainMenuControl :
		public wraps::BaseLayout
	{
	public:
		MainMenuControl(MyGUI::Widget* _parent = nullptr);
		virtual ~MainMenuControl();

		void setVisible(bool _value);

	private:
		void createMainMenu();


	private:
		MyGUI::MenuBar* mBar;
		MyGUI::MenuControl* mPopupMenuWidgets;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
