/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "ViewerPch.h"
#include "ColorManager.h"
#include "SettingsManager.h"
#include "CommandManager.h"

template <> tools::ColourManager* MyGUI::Singleton<tools::ColourManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ColourManager>::mClassTypeName("ColourManager");

namespace tools
{

	ColourManager::ColourManager() :
		mColourPanel(nullptr)
	{
        CommandManager::getInstance().registerCommand("Command_ChangeColourBackground", MyGUI::newDelegate(this, &ColourManager::commandChangeColourBackground));
	}

	ColourManager::~ColourManager()
	{
	}

	void ColourManager::initialise()
	{
		mColourPanel = new ColourPanel();
		mColourPanel->eventEndDialog = MyGUI::newDelegate(this, &ColourManager::notifyEndDialog);
		mColourPanel->eventPreviewColour = MyGUI::newDelegate(this, &ColourManager::notifyPreviewColour);
	}

	void ColourManager::shutdown()
	{
		delete mColourPanel;
		mColourPanel = nullptr;
	}

	void ColourManager::commandChangeColourBackground(const MyGUI::UString& _commandName, bool& _result)
	{
		mCurrentColourType = "ColorBackground";
		showColourDialog();

		_result = true;
	}

	void ColourManager::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mColourPanel->endModal();

		if (_result)
			setColour(mColourPanel->getColour());
		else
			setColour(mPreviewColour);
	}

	void ColourManager::notifyPreviewColour(const MyGUI::Colour& _value)
	{
		setColour(_value);
	}

	void ColourManager::showColourDialog()
	{
		mPreviewColour = getColour();
		mColourPanel->setColour(mPreviewColour);
		mColourPanel->doModal();
	}

	void ColourManager::setColour(const MyGUI::Colour& _color)
	{
		SettingsManager::getInstance().getSector("Settings")->setPropertyValue(mCurrentColourType, _color);
	}

	MyGUI::Colour ColourManager::getColour()
	{
		return SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>(mCurrentColourType);
	}

} // namespace tools
