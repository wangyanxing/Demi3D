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

        void notifyPopupMenuAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);

	private:
		MyGUI::MenuBar* mBar;
	};

} // namespace tools

#endif // __MAIN_MENU_CONTROL_H__
