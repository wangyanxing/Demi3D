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
#include "HonFxerApp.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_TreeControl.h"
#include "MyGUI_TreeControlItem.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "Localise.h"
#include "PathLib.h"
#include "ColorManager.h"
#include "ZipArchive.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "MainPaneControl.h"
#include "EditorManager.h"

namespace Demi
{
    HonFxerApp::HonFxerApp()
        : DemiDemo(DemoConfig("Hon Fxer",1400, 800))
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32 && !DEMI_DEBUG
        ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
    }

    HonFxerApp::~HonFxerApp()
    {
    }

    void HonFxerApp::Update()
    {
        DemiDemo::Update();

        if (mMainPane)
            mMainPane->Update();

        if (Driver)
            DiEditorManager::Get()->Update();
    }

    void HonFxerApp::Close()
    {
        DI_DELETE(DiEditorManager::Get());
        SAFE_DELETE(mMainPane);

        DialogManager::getInstance().shutdown();
        delete DialogManager::getInstancePtr();

        MessageBoxManager::getInstance().shutdown();
        delete MessageBoxManager::getInstancePtr();

        SettingsManager::getInstance().shutdown();
        delete SettingsManager::getInstancePtr();

        ColourManager::getInstance().shutdown();
        delete ColourManager::getInstancePtr();

        CommandManager::getInstance().shutdown();
        delete CommandManager::getInstancePtr();

        MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
        MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>("Layer");

        DI_UNINSTALL_PLUGIN(DiK2);
        DI_UNINSTALL_PLUGIN(DiFx);
        DemiDemo::Close();
    }

    void HonFxerApp::OpenImpl()
    {
        DemiDemo::OpenImpl();
        DI_INSTALL_PLUGIN(DiFx);
        DI_INSTALL_PLUGIN(DiK2);

        Driver->GetMainRenderWindow()->SetForceRenderToCanvas(true);
        
        DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
        peMgr->SetManualOutputTarget(DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas());

        DiBase::Driver->GetMainRenderWindow()->GetRenderBuffer()->SetClearColor(DiColor(0.2f, 0.2f, 0.2f));

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));
        DiDirLightPtr dirlight;
        dirlight = make_shared<DiDirLight>();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());

        //////////////////////////////////////////////////////////////////////////

        MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");
        MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");
        MyGUI::ResourceManager::getInstance().load("FxEditorLayers.xml");

        new CommandManager();
        CommandManager::getInstance().initialise();

        DiString userSettings = DiPathLib::GetApplicationPath() + "FxSettings.xml";
        new SettingsManager();
        SettingsManager::getInstance().initialise(userSettings.c_str());

        std::string mLocale = "English";
        MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);

        new MessageBoxManager();
        MessageBoxManager::getInstance().initialise();

        new ColourManager();
        ColourManager::getInstance().initialise();

        MyGUI::ResourceManager::getInstance().load("FxInitialise.xml");

        MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
        factory.registerFactory<MyGUI::TreeControl>("Widget");
        factory.registerFactory<MyGUI::TreeControlItem>("Widget");
        MyGUI::ResourceManager::getInstance().load("TreeControlSkin.xml");
        MyGUI::ResourceManager::getInstance().load("TreeItemIcons.xml");

        CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &HonFxerApp::Command_QuitApp));
        CommandManager::getInstance().registerCommand("Command_Export", MyGUI::newDelegate(this, &HonFxerApp::Command_Export));
        CommandManager::getInstance().registerCommand("Command_ResLocation", MyGUI::newDelegate(this, &HonFxerApp::Command_ResLocation));
        CommandManager::getInstance().registerCommand("Command_GameLocation", MyGUI::newDelegate(this, &HonFxerApp::Command_GameLocation));
        CommandManager::getInstance().registerCommand("Command_ViewHelp", MyGUI::newDelegate(this, &HonFxerApp::Command_ViewHelp));

        mMainPane = new MainPaneControl();

        //mModelViewer = DI_NEW K2ModelViewer();

        new DialogManager();
        DialogManager::getInstance().initialise();

        DI_NEW DiEditorManager();
        
        
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        DiBase::CommandMgr->ExecuteCommand("createChild ParticleSystem");
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        DiBase::CommandMgr->ExecuteCommand("createChild ParticleElement");
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        DiBase::CommandMgr->ExecuteCommand("createChild PointEmitter");
        DiBase::CommandMgr->ExecuteCommand("selectLast");
        
        //DiBase::CommandMgr->ExecuteCommand("removeObj");
        
        //mSetResLocWindow = new SetResLocWindow();
        //mSetResLocWindow->eventEndDialog = MyGUI::newDelegate(this, &HonFxerApp::NotifySetResLocWindowEndDialog);

        //mSetGameLocWindow = new SetGameLocWindow();
        //mSetGameLocWindow->eventEndDialog = MyGUI::newDelegate(this, &HonFxerApp::NotifySetGameLocWindowEndDialog);

        //mHelpWindow = new ViewerHelper();
    }

    HonFxerApp* HonFxerApp::GetFxApp()
    {
        return (HonFxerApp*)sTheApp;
    }

    void HonFxerApp::mouseMoved(const OIS::MouseEvent& evt)
    {
        bool incanvas = mMainPane->GetWorkspaceControl()->GetRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mouseMoved(evt);
        }
    }

    void HonFxerApp::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool incanvas = mMainPane->GetWorkspaceControl()->GetRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mousePressed(evt,id);
        }
    }

    void HonFxerApp::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool incanvas = mMainPane->GetWorkspaceControl()->GetRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mouseReleased(evt, id);
        }
    }

    void HonFxerApp::Command_QuitApp(const MyGUI::UString& _commandName, bool& _result)
    {
        if (DialogManager::getInstance().getAnyDialog())
            DialogManager::getInstance().endTopDialog();
        else
        {
            if (MessageBoxManager::getInstance().hasAny())
                MessageBoxManager::getInstance().endTop(MyGUI::MessageBoxStyle::Cancel);
            else
                mQuit = true;
        }

        _result = true;
    }

    void HonFxerApp::Command_GameLocation(const MyGUI::UString& _commandName, bool& _result)
    {
        if (!CheckCommand())
            return;

        //mSetGameLocWindow->doModal();

        _result = true;
    }

    void HonFxerApp::keyPressed(const OIS::KeyEvent &arg)
    {
        mCameraHelper->OnKeyDown(arg);
    }

    void HonFxerApp::Command_Export(const MyGUI::UString& _commandName, bool& _result)
    {
        MyGUI::Message::createMessageBox("Message", "Sorry",
            "This feature hasn't been implemented yet.", 
            MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);
    }

    void HonFxerApp::Command_ResLocation(const MyGUI::UString& _commandName, bool& _result)
    {
        if (!CheckCommand())
            return;

        //mSetResLocWindow->doModal();

        _result = true;
    }

    void HonFxerApp::NotifySetResLocWindowEndDialog(Dialog* _dialog, bool _result)
    {
//         MYGUI_ASSERT(mSetResLocWindow == _dialog, "mSetResLocWindow == _sender");
// 
//         if (_result)
//         {
//             mSetResLocWindow->saveSettings();
//         }
// 
//         mSetResLocWindow->endModal();
    }

    bool HonFxerApp::CheckCommand()
    {
        if (DialogManager::getInstance().getAnyDialog())
            return false;

        if (MessageBoxManager::getInstance().hasAny())
            return false;

        return true;
    }

    void HonFxerApp::NotifySetGameLocWindowEndDialog(Dialog* _dialog, bool _result)
    {
//         MYGUI_ASSERT(mSetGameLocWindow == _dialog, "mSetGameLocWindow == _sender");
// 
//         if (_result)
//         {
//             mSetGameLocWindow->saveSettings();
//         }
// 
//         mSetGameLocWindow->endModal();
    }

    void HonFxerApp::SetResourceLocation(const DiString& resPack, const DiString& texPack)
    {
//         if (!texPack.empty())
//         {
//             GetModelViewer()->SetK2ResourcePack(resPack, texPack);
//         }
//         else
//             GetModelViewer()->SetK2ResourcePack(resPack);
    }

    void HonFxerApp::Command_ViewHelp(const MyGUI::UString& _commandName, bool& _result)
    {
//         if (!CheckCommand())
//             return;
// 
//         mHelpWindow->doModal();
// 
//         _result = true;
    }
}