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
#include "DebugHelper.h"

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    
    float scale = 0.5f;
    DiDirLightPtr dirlight;
    auto mat = DiMaterial::QuickCreate("phong_v", "phong_p");
    mat->SetAmbient(DiColor(0.1f, 0.1f, 0.1f));

    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor(0.9f, 0.9f, 0.9f));
    dirlight->SetDirection(DiVec3(0.3f, -0.7f, 0.4).normalisedCopy());
    //dirlight->SetShadowCastEnable(true);

    DiCullNode* node = sm->GetRootNode()->CreateChild();
    DiModelPtr model = make_shared<DiModel>("box", "test2.model");
    //model->SetShadowCastEnable(true);
    model->SetMaterial(mat);
    node->AttachObject(model);
    //node->Scale(10, 10, 10);
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open();

	return 0;
}