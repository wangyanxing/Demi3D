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
    
    DiDirLightPtr dirlight;

    sm->SetAmbientColor(DiColor(0.2f, 0.2f, 0.2f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());
    //dirlight->SetShadowCastEnable(true); 

    DiCullNode* node = sm->GetRootNode()->CreateChild();
    DiModelPtr model = make_shared<DiModel>("box", "arachna_high.model");
    //model->SetShadowCastEnable(true);
    //model->SetMaterial(mat);
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