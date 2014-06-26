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

#ifndef __SET_GAME_LOCATION_WINDOW_H__
#define __SET_GAME_LOCATION_WINDOW_H__

#include "FxerPrerequisites.h"
#include "BaseLayout.h"
#include "Dialog.h"

namespace tools
{
	class SetGameLocWindow :
		public Dialog
	{
	public:
        SetGameLocWindow();
        virtual ~SetGameLocWindow();

		void loadSettings();
		void saveSettings();

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
        void notifyBrowser(MyGUI::Widget* _sender);
        void notifyOk(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

	private:
		MyGUI::Button* mButtonOk;
		MyGUI::Button* mButtonCancel;
        MyGUI::Button* mButtonBrwoser;
        MyGUI::TextBox* mTextLocation;
    };

} // namespace tools

#endif // __SETTINGS_WINDOW_H__
