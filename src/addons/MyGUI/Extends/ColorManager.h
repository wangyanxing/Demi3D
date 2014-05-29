/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __COLOUR_MANAGER_H__
#define __COLOUR_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "ColorPanel.h"

namespace tools
{
    class MYGUI_EXPORT ColourManager :
		public MyGUI::Singleton<ColourManager>
	{
	public:
		ColourManager();
		virtual ~ColourManager();

		void initialise();
		void shutdown();

	private:
		void commandChangeColourBackground(const MyGUI::UString& _commandName, bool& _result);

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyPreviewColour(const MyGUI::Colour& _value);

		void showColourDialog();

		void setColour(const MyGUI::Colour& _color);
		MyGUI::Colour getColour();

	private:
		ColourPanel* mColourPanel;
		MyGUI::Colour mPreviewColour;
		std::string mCurrentColourType;
	};

} // namespace tools

#endif // __COLOUR_MANAGER_H__
