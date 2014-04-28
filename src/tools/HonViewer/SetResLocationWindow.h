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

#ifndef __SET_RES_LOCATION_WINDOW_H__
#define __SET_RES_LOCATION_WINDOW_H__

#include "ViewerPrerequisites.h"
#include "BaseLayout/BaseLayout.h"
#include "Tools/Dialog.h"

namespace tools
{
	class SetResLocWindow :
		public Dialog
	{
	public:
		SetResLocWindow();
		virtual ~SetResLocWindow();

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
        
        void updateLocations();

	private:
		MyGUI::Button* mButtonOk;
		MyGUI::Button* mButtonCancel;
        MyGUI::Button* mButtonBrwoserRes;
        MyGUI::Button* mButtonBrwoserTex;
        MyGUI::TextBox* mTextResourseLocation;
        MyGUI::TextBox* mTextTextureLocation;
    };

} // namespace tools

#endif // __SETTINGS_WINDOW_H__
