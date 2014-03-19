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

#include "K2Model.h"
#include "K2Clip.h"
#include "EnginePlugin.h"

DiK2Model* k2md = nullptr;

#if DEMI_DEBUG
const DiString k2Plugin = "DiK2_d";
#else
const DiString k2Plugin = "DiK2";
#endif

void InitScene()
{
    DiPlugin::LoadPlugin(k2Plugin);

    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    DiDirLightPtr dirlight;

    sm->SetAmbientColor(DiColor(0.2f, 0.2f, 0.2f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());
    //dirlight->SetShadowCastEnable(true); 7

    DiCullNode* node = sm->GetRootNode()->CreateChild();
    k2md = DI_NEW DiK2Model("heroes/aluna");
    k2md->CreateNode(sm);
    k2md->GetAnimation()->Play("idle");
}

void UpdateScene()
{
    k2md->Update(DiBase::Driver->GetDeltaSecond());
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open();

	return 0;
}