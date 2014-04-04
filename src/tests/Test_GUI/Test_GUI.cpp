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

#include "Demi.h"
#include "DemoFrame.h"
#include "EnginePlugin.h"

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
    //dirlight->SetShadowCastEnable(true);

    // visbile mesh for the point light
    DiSimpleShapePtr lightSphere = make_shared<DiSimpleShape>("lightSphere");
    lightSphere->CreateSphere(5, 16, 8);
    DiMaterialPtr m = DiMaterial::QuickCreate("basic_v", "basic_p");
    m->SetDiffuse(DiColor(1, 0.8f, 0));
    lightSphere->SetMaterial(m);

    DiCullNode* nd = sm->GetRootNode()->CreateChild();
    nd->AttachObject(lightSphere);
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open(argc, argv);

	return 0;
}