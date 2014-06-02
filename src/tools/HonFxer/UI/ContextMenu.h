/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __CONTEXT_MENU_H__
#define __CONTEXT_MENU_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "MyGUI_PopupMenu.h"

namespace Demi
{
    class ContextMenu final
	{
	public:
        ContextMenu(MyGUI::Widget* _parent)
		{
            mMenu = dynamic_cast<MyGUI::PopupMenu*>(_parent->_createSkinWidget(MyGUI::WidgetStyle::Popup, 
                "PopupMenu", "PopupMenu",
                MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default, "Popup"));
			mMenu->setVisible(false);
			mMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &ContextMenu::notifyMenuCtrlAccept);
        }

        ~ContextMenu()
        {
            delete mMenu;
        }

		void setVisible(bool _value)
		{
//            mMenu->removeAllItems();
//            mMenu->addItem("menu1");
//            mMenu->addItem("menu1");
//            mMenu->addItem("menu1", MyGUI::MenuItemType::Separator);
//            mMenu->addItem("menu1");
//            auto item = mMenu->addItem("menu1", MyGUI::MenuItemType::Popup);
//            auto subMenu = mMenu->createItemChild(item);
//            subMenu->addItem("menu2");

			mMenu->setPosition(MyGUI::InputManager::getInstance().getMousePosition());
			mMenu->setVisibleSmooth(_value);
		}

		bool isVisible()
		{
			return mMenu->getVisible();
		}

		/** Event : Menu accept.\n
			signature : void method(wraps::ContextMenu* _sender, const std::string& _id)
			@param _sender
			@param _id
		*/
		MyGUI::delegates::CDelegate2<ContextMenu*, const std::string&> eventMenuAccept;

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
		{
			eventMenuAccept(this, _item->getItemId());
		}

    private:
        MyGUI::PopupMenu* mMenu{ nullptr };

	};

} // namespace wraps

#endif // __CONTEXT_MENU_H__
