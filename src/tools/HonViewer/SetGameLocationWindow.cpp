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

#include "ViewerPch.h"
#include "SetGameLocationWindow.h"
#include "PathLib.h"
#include "Window.h"
#include "HonViewerApp.h"
#include "MessageBox.h"

namespace tools
{
    SetGameLocWindow::SetGameLocWindow() :
		Dialog("SetGameLocation.layout"),
		mButtonOk(nullptr),
		mButtonCancel(nullptr),
        mButtonBrwoser(nullptr),
        mTextLocation(nullptr)
	{
		assignWidget(mButtonOk, "Ok");
        assignWidget(mButtonCancel, "Cancel");
        assignWidget(mButtonBrwoser, "ButtonBrowser");
        assignWidget(mTextLocation, "TextGameLocation");

        mButtonOk->eventMouseButtonClick += MyGUI::newDelegate(this, &SetGameLocWindow::notifyOk);
        mButtonCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SetGameLocWindow::notifyCancel);
        mButtonBrwoser->eventMouseButtonClick += MyGUI::newDelegate(this, &SetGameLocWindow::notifyBrowser);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
            window->eventWindowButtonPressed += MyGUI::newDelegate(this, &SetGameLocWindow::notifyWindowButtonPressed);

		loadSettings();

		mMainWidget->setVisible(false);
	}

    SetGameLocWindow::~SetGameLocWindow()
	{
	}

    void SetGameLocWindow::notifyOk(MyGUI::Widget* _sender)
	{
        auto gameloc = mTextLocation->getCaption();
        DiString exeFile = gameloc.asUTF8_c_str();
        exeFile += "/";
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        exeFile += "hon.exe";
#else
        exeFile += "hon";
#endif
        if (DiPathLib::FileExisted(exeFile))
        {
            DiString path = gameloc.asUTF8_c_str();
            DiString respack = path + "/game/resources0.s2z";
            DiString texpack = path + "/game/textures.s2z";
            HonViewerApp::GetViewerApp()->SetResourceLocation(respack, texpack);
        }
        else
        {
            MyGUI::Message::createMessageBox("Message", "Invalid HON game folder",
                "Cannot locate executable file of HON",
                MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconError);
        }

        eventEndDialog(this, true);
	}

    void SetGameLocWindow::notifyCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
	}

    void SetGameLocWindow::onDoModal()
	{
		loadSettings();

		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, 
            (parentSize.height - windowSize.height) / 2);
	}

    void SetGameLocWindow::onEndModal()
	{
	}

    void SetGameLocWindow::saveSettings()
	{
	}

    void SetGameLocWindow::loadSettings()
	{
	}

    void SetGameLocWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

    void SetGameLocWindow::notifyBrowser(MyGUI::Widget* _sender)
    {
        if (_sender == mButtonBrwoser)
        {
            DiString outFolder;
            void* wndHandle = DiBase::Driver->GetMainRenderWindow()->GetWindow()->GetWndHandle();
            DiPathLib::OpenDirectoryDialog(wndHandle, "Game root directory", DiPathLib::GetApplicationPath(), outFolder);
            mTextLocation->setCaption(outFolder.c_str());
        }
    }

} // namespace tools
