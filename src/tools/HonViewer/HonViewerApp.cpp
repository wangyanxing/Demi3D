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
#include "HonViewerApp.h"

#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_TreeControl.h"
#include "MyGUI_TreeControlItem.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "Localise.h"
#include "BackgroundControl.h"
#include "MainPaneControl.h"
#include "PathLib.h"
#include "ColorManager.h"
#include "K2ModelView.h"
#include "SettingsManager.h"
#include "CommandManager.h"

namespace Demi
{
    HonViewerApp::HonViewerApp()
        : DemiDemo(DemoConfig("Hon Viewer"))
        , mMainPane(nullptr)
        , mModelViewer(nullptr)
    {
    }

    HonViewerApp::~HonViewerApp()
    {
    }

    void HonViewerApp::Update()
    {
        DemiDemo::Update();

        if (mMainPane)
            mMainPane->update();
        if (mModelViewer)
            mModelViewer->Update(Driver->GetDeltaSecond());
    }

    void HonViewerApp::Close()
    {
        //delete mBackground;
        //mBackground = nullptr;

        SAFE_DELETE(mModelViewer);
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

        DemiDemo::Close();
    }

    void HonViewerApp::OpenImpl()
    {
        DemiDemo::OpenImpl();

        DI_INSTALL_PLUGIN(DiK2);

        // use software skinning for editor
        DiBase::CommandMgr->SetIntVar("force_softskin", 0);
        
        DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
        peMgr->SetManualOutputTarget(DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas());

        DiBase::Driver->GetMainRenderWindow()->GetRenderBuffer()->SetClearColor(DiColor(0.2f, 0.2f, 0.2f));

        //DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(DiColor(0,0,0));

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
        MyGUI::ResourceManager::getInstance().load("EditorLayers.xml");

        new CommandManager();
        CommandManager::getInstance().initialise();

        DiString userSettings = DiPathLib::GetApplicationPath() + "ViewerSettings.xml";
        new SettingsManager();
        SettingsManager::getInstance().initialise(userSettings.c_str());

        std::string mLocale = "English";
        MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);

        new MessageBoxManager();
        MessageBoxManager::getInstance().initialise();

        new DialogManager();
        DialogManager::getInstance().initialise();

        new ColourManager();
        ColourManager::getInstance().initialise();

        MyGUI::ResourceManager::getInstance().load("Initialise.xml");

        MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
        factory.registerFactory<MyGUI::TreeControl>("Widget");
        factory.registerFactory<MyGUI::TreeControlItem>("Widget");
        MyGUI::ResourceManager::getInstance().load("TreeControlSkin.xml");
        MyGUI::ResourceManager::getInstance().load("TreeItemIcons.xml");

        //mBackground = new BackgroundControl();
        mMainPane = new MainPaneControl();

        mModelViewer = DI_NEW K2ModelViewer();
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        //DiString zip("L:/Games/Savage2Res/resources0.zip");
        //DiString zipTex("L:/Games/Savage2Res/textures-high.zip");
        DiString zip("L:/Games/Heroes of Newerth/game/resources0.s2z");
        DiString zipTex("L:/Games/Heroes of Newerth/game/textures.s2z");
        //DiString zip("L:/Games/hon_res_misc/hon_res_misc.zip");
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        DiString zip("/Users/wangya/Demi/media_hon/buildings.zip");
#endif
        DiK2Configs::TEXTURE_PACK_PREFIX_FOLDER = "00000000/";

        mModelViewer->SetK2ResourcePack("L:/Games/HON_res/world/cliffs.zip");

        //mModelViewer->SetK2ResourcePack(zip, zipTex);
        //mModelViewer->SetK2ResourcePack(zip);
    }

    HonViewerApp* HonViewerApp::GetViewerApp()
    {
        return (HonViewerApp*)sTheApp;
    }

    void HonViewerApp::mouseMoved(const OIS::MouseEvent& evt)
    {
        bool incanvas = mMainPane->getMainWorkspaceControl()->getRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mouseMoved(evt);
        }
    }

    void HonViewerApp::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool incanvas = mMainPane->getMainWorkspaceControl()->getRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mousePressed(evt,id);
        }
    }

    void HonViewerApp::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool incanvas = mMainPane->getMainWorkspaceControl()->getRenderWndControl()->mouseInCanvas();
        if (incanvas)
        {
            DemiDemo::mouseReleased(evt, id);
        }
    }
}