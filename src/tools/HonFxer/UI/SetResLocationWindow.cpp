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
#include "SetResLocationWindow.h"
#include "PathLib.h"
#include "HonFxerApp.h"
#include "K2Configs.h"
#include "ZipArchive.h"
#include "Window.h"
#include "SettingsManager.h"
#include "SettingsSector.h"

namespace tools
{
	SetResLocWindow::SetResLocWindow() :
		Dialog("SetResLocation.layout"),
		mButtonOk(nullptr),
		mButtonCancel(nullptr),
        mButtonBrwoserRes(nullptr),
        mButtonBrwoserTex(nullptr),
        mTextResourseLocation(nullptr),
        mTextTextureLocation(nullptr)
	{
		assignWidget(mButtonOk, "Ok");
        assignWidget(mButtonCancel, "Cancel");
        assignWidget(mButtonBrwoserRes, "ButtonBrowserRes");
        assignWidget(mButtonBrwoserTex, "ButtonBrowserTex");
        assignWidget(mTextResourseLocation, "TextResourceLocation");
        assignWidget(mTextTextureLocation, "TextTextureLocation");

		mButtonOk->eventMouseButtonClick += MyGUI::newDelegate(this, &SetResLocWindow::notifyOk);
		mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SetResLocWindow::notifyCancel);
        mButtonBrwoserRes->eventMouseButtonClick += MyGUI::newDelegate(this, &SetResLocWindow::notifyBrowser);
        mButtonBrwoserTex->eventMouseButtonClick += MyGUI::newDelegate(this, &SetResLocWindow::notifyBrowser);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &SetResLocWindow::notifyWindowButtonPressed);

		loadSettings();

		mMainWidget->setVisible(false);
	}

	SetResLocWindow::~SetResLocWindow()
	{
	}

	void SetResLocWindow::notifyOk(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, true);

        updateLocations();
	}

	void SetResLocWindow::notifyCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
	}

	void SetResLocWindow::onDoModal()
	{
		loadSettings();

		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, 
            (parentSize.height - windowSize.height) / 2);
	}

	void SetResLocWindow::onEndModal()
	{
	}

	void SetResLocWindow::saveSettings()
	{
        DiString respak = mTextResourseLocation->getCaption().asUTF8().c_str();
        DiString texpak = mTextTextureLocation->getCaption().asUTF8().c_str();
        SettingsManager::getInstance().getSector("Settings")->setPropertyValue("ResourcePack", respak.c_str());
        SettingsManager::getInstance().getSector("Settings")->setPropertyValue("TexturePack", texpak.c_str());

	}

	void SetResLocWindow::loadSettings()
	{
        MyGUI::UString resPak = SettingsManager::getInstance().getSector("Settings")->getPropertyValue("ResourcePack");
        MyGUI::UString texpak = SettingsManager::getInstance().getSector("Settings")->getPropertyValue("TexturePack");
        mTextResourseLocation->setCaption(resPak.asUTF8().c_str());
        mTextTextureLocation->setCaption(texpak.asUTF8().c_str());
        updateLocations();
    }

	void SetResLocWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

    void SetResLocWindow::notifyBrowser(MyGUI::Widget* _sender)
    {
        void* wndHandle = DiBase::Driver->GetMainRenderWindow()->GetWindow()->GetWndHandle();
        DiString filter = "Pack files(zip,s2z)|*.zip;*.s2z|All files (*.*)|*.*";

        if (_sender == mButtonBrwoserRes)
        {
            DiVector<DiString> out;
            DiPathLib::OpenFileDialog(wndHandle, "Resource pack (usually resource0.s2z)", DiPathLib::GetApplicationPath(), "", filter, 0, out);
            if (out.size() >= 1)
                mTextResourseLocation->setCaption(out[0].c_str());
        }
        else if (_sender == mButtonBrwoserTex)
        {
            DiVector<DiString> out;
            DiPathLib::OpenFileDialog(wndHandle, "Texture pack (usually texture0.s2z)", DiPathLib::GetApplicationPath(), "", filter, 0, out);
            if (out.size() >= 1)
                mTextTextureLocation->setCaption(out[0].c_str());
        }
    }

    void SetResLocWindow::updateLocations()
    {
        DiString respak = mTextResourseLocation->getCaption().asUTF8().c_str();
        DiString texpak = mTextTextureLocation->getCaption().asUTF8().c_str();
        HonFxerApp::GetFxApp()->SetResourceLocation(respak, texpak);
    }

} // namespace tools
