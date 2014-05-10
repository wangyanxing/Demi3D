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
#include "K2World.h"
#include "K2Configs.h"
#include "K2Terrain.h"
#include "K2Plugin.h"

DiK2World* world = nullptr;

void InitScene()
{
    DI_INSTALL_PLUGIN(DiK2);
    
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));
    
    DiDirLightPtr dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(-0.5f,-0.866f,0).normalisedCopy());
    
    DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(DiColor(0.5f,0.5f,0.5f));

    world = DI_NEW DiK2World();
    world->Load("Arena_1.s2z");

    DiVec2 worldsize = world->mTerrain->GetWorldSize();

    auto cameraHelper = DemiDemo::GetApp()->GetCameraHelper();
    cameraHelper->SetStyle(CS_FREELOOK);
    cameraHelper->SetTopSpeed(600);
    cameraHelper->GetCamera()->SetPosition(worldsize.x / 2, 500, worldsize.y / 2);
    cameraHelper->GetCamera()->SetPosition(3104.90723f, 1610.28467f, 4649.14502f);
    cameraHelper->GetCamera()->SetOrientation(DiQuat(0.308038354f,-0.225031450f,-0.746425748,-0.545306683f));
    //cameraHelper->Enable(false);

    DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
    DiPostEffect* bloom = peMgr->GetEffect("Bloom");
    if (bloom)
        bloom->SetEnable(true);
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi Sample: Hon World"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
    app.SetShutdownCallback([&](){
        DI_UNINSTALL_PLUGIN(DiK2);

    });
	app.Open(argc, argv);
    
	return 0;
}