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
DiK2Model* k2md2 = nullptr;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   if DEMI_DEBUG
const DiString k2Plugin = "DiK2_d";
#   else
const DiString k2Plugin = "DiK2";
#   endif
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
    //dirlight->SetShadowCastEnable(true);
    
    sm->GetCamera()->Move(DiVec3(0,50,70));

    DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->SetClearColor(DiColor(0.5f,0.5f,0.5f));

    k2md = DI_NEW DiK2Model("heroes/aluna/model.mdf");
    k2md->CreateNode(sm);
    k2md->GetNode()->Translate(-50, 0, 0);
    k2md->GetAnimation()->Play("idle");
    
    k2md2 = DI_NEW DiK2Model("items/couriers/panda/model.mdf");
    k2md2->CreateNode(sm);
    k2md2->GetNode()->Translate(50, 0, 0);
    k2md2->GetAnimation()->Play("idle");
    
    DemiDemo::GetApp()->GetInputManager()->registerKeyReleaseEvent("changeClip",
        [&](const OIS::KeyEvent& e){
            switch (e.key)
            {
                case OIS::KC_1:
                    k2md->GetAnimation()->Play("idle");
                    k2md2->GetAnimation()->Play("idle");
                    break;
                case OIS::KC_2:
                    k2md->GetAnimation()->Play("walk_1");
                    k2md2->GetAnimation()->Play("walk_1");
                    break;
                case OIS::KC_3:
                    k2md->GetAnimation()->Play("death_1");
                    k2md2->GetAnimation()->Play("death_1");
                    break;
                case OIS::KC_4:
                    k2md->GetAnimation()->Play("attack_1");
                    k2md2->GetAnimation()->Play("portrait");
                    break;
                case OIS::KC_5:
                    k2md->GetAnimation()->Play("attack_2");
                    k2md2->GetAnimation()->Play("bored_1");
                    break;
                case OIS::KC_6:
                    k2md->GetAnimation()->Play("attack_3");
                    break;
                case OIS::KC_7:
                    k2md->GetAnimation()->Play("ability_1");
                    break;
                case OIS::KC_8:
                    k2md->GetAnimation()->Play("ability_2");
                    break;
                case OIS::KC_9:
                    k2md->GetAnimation()->Play("ability_3");
                    break;
                case OIS::KC_0:
                    k2md->GetAnimation()->Play("ability_4");
                    break;
                default:
                    break;
            }
        });
}

void UpdateScene()
{
    k2md->Update(DiBase::Driver->GetDeltaSecond());
    k2md2->Update(DiBase::Driver->GetDeltaSecond());
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open();

	return 0;
}