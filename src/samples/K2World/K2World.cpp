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
#include "K2TerrainMap.h"

DiK2World* world = nullptr;

void InitScene()
{
    DiPlugin::LoadPlugin("DiK2");
    
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));
    
    DiDirLightPtr dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(-0.5f,-0.866f,0).normalisedCopy());
    
    DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(DiColor(0.5f,0.5f,0.5f));

    world = DI_NEW DiK2World();
    world->Load("maps/test_entity");

    DiVec2 worldsize = world->GetTerrain()->GetWorldSize();

    auto cameraHelper = DemiDemo::GetApp()->GetCameraHelper();
    cameraHelper->SetStyle(CS_FREELOOK);
    cameraHelper->SetTopSpeed(600);
    cameraHelper->GetCamera()->SetPosition(worldsize.x / 2, 500, worldsize.y / 2);
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
        DiPlugin::UnloadPlugin("DiK2");
    });
	app.Open();
    
	return 0;
}