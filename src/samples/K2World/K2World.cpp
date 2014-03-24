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

DiK2World* world = nullptr;
DiK2Model* md0 = nullptr;

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

    world = DI_NEW DiK2World(sm);
    world->Load("maps/te2");

    md0 = world->GetModel(22);

    DemiDemo::GetApp()->GetInputManager()->registerKeyReleaseEvent("changeClip",
        [&](const OIS::KeyEvent& e){
        DiVec3 angle[] =
        {
            DiVec3(0, 0, 0),
            DiVec3(0, 90, 0),
            DiVec3(0, 180, 0),
            DiVec3(0, 270, 0),
        };

        int id = 0;
        switch (e.key)
        {
        case OIS::KC_1:
        case OIS::KC_2:
        case OIS::KC_3:
        case OIS::KC_4:
            id = e.key - OIS::KC_1;
            DiQuat q = DiK2Configs::ConvertAngles(angle[id]);
            md0->GetNode()->SetOrientation(q);
            break;
        }
    });
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