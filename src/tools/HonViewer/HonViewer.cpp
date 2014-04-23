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

#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "MessageBoxManager.h"
#include "DialogManager.h"
#include "Localise.h"
#include "BackgroundControl.h"
#include "MainPaneControl.h"

using namespace tools;

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    DiDirLightPtr dirlight;
    sm->SetAmbientColor(DiColor(0.2f, 0.2f, 0.2f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());

    // visbile mesh for the point light
    DiSimpleShapePtr lightSphere = make_shared<DiSimpleShape>("lightSphere");
    lightSphere->CreateSphere(5, 16, 8);
    DiMaterialPtr m = DiMaterial::QuickCreate("basic_v", "basic_p");
    m->SetDiffuse(DiColor(1, 0.8f, 0));
    lightSphere->SetMaterial(m);

    DiCullNode* nd = sm->GetRootNode()->CreateChild();
    nd->AttachObject(lightSphere);

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

    new BackgroundControl();
    new MainPaneControl();
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Hon Viewer"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open(argc, argv);

	return 0;
}