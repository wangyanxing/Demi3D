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

namespace Demi
{
    HonViewerApp::HonViewerApp()
        : DemiDemo(DemoConfig("Hon Viewer"))
    {
    }

    HonViewerApp::~HonViewerApp()
    {
    }

    void HonViewerApp::Update()
    {
        DemiDemo::Update();
    }

    void HonViewerApp::Close()
    {
        delete mBackground;
        mBackground = nullptr;

        delete mMainPane;
        mMainPane = nullptr;

        DialogManager::getInstance().shutdown();
        delete DialogManager::getInstancePtr();

        MessageBoxManager::getInstance().shutdown();
        delete MessageBoxManager::getInstancePtr();

        DemiDemo::Close();
    }

    void HonViewerApp::OpenImpl()
    {
        DemiDemo::OpenImpl();

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f, 0.1f));

        DiDirLightPtr dirlight = make_shared<DiDirLight>();
        sm->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(-DiVec3(1, 1, 2).normalisedCopy());

        const int size = 1;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                {
                    DiMaterialPtr mat = DiMaterial::QuickCreate("lambert_v", "lambert_p", SHADER_FLAG_SKINNED);
                    mat->SetDiffuse(DiColor(1, 1, 1));

                    DiString name;
                    name.Format("md_%d_%d", i, j);
                    DiAnimModelPtr model = make_shared<DiAnimModel>(name, "robot.model", "robot.motion");
                    //DiModelPtr model = make_shared<DiModel>(name, "robot.model");
                    model->SetMaterial(mat);

                    model->SetAutoUpdateAnims(true);
                    model->GetClipSet()->GetClipController("Walk")->SetEnabled(true);

                    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
                    cullnode->AttachObject(model);
                    cullnode->SetPosition(i * 120.0f, j * 120.0f, k * 120.0f);
                }
            }
        }

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

        mBackground = new BackgroundControl();
        mMainPane = new MainPaneControl();
    }
}