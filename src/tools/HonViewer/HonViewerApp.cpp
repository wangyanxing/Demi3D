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
#include "K2ModelView.h"

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

        mMainPane->update();
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

        DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(DiColor(0.35f, 0.35f, 0.35f));
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

        std::string mLocale = "English";
        MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);

        new MessageBoxManager();
        MessageBoxManager::getInstance().initialise();

        new DialogManager();
        DialogManager::getInstance().initialise();

        MyGUI::ResourceManager::getInstance().load("Initialise.xml");

        MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
        factory.registerFactory<MyGUI::TreeControl>("Widget");
        factory.registerFactory<MyGUI::TreeControlItem>("Widget");
        MyGUI::ResourceManager::getInstance().load("TreeControlSkin.xml");
        MyGUI::ResourceManager::getInstance().load("TreeItemIcons.xml");

        //mBackground = new BackgroundControl();
        mMainPane = new MainPaneControl();

        mModelViewer = DI_NEW K2ModelViewer();
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