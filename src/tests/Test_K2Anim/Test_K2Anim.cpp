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
#include "Bone.h"
#include "K2Anim.h"

K2Anim* k2anim = nullptr;

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

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    DiString resBase = "L:/Games/HON_res/";
    DiString resTexBase = "L:/Games/HON_tex/";
    DiString output = "C:/Demi/media/models/hon/";
#else
    DiString resBase = "/Users/wangya/Projects/HONres/resources0/";
    DiString resTexBase = "/Users/wangya/Projects/HONres/textures/";
    DiString output = "/Users/wangya/Demi/media/models/hon/";
#endif
    
    k2anim = new K2Anim(resBase,sm);
    //k2anim->Load("items/couriers/dragon/high.model", "items/couriers/dragon/clips/default_1.clip");
    k2anim->Load("items/couriers/panda/high.model", "items/couriers/panda/clips/default_1.clip");
    //k2anim->Load("buildings/hellbourne/range_rax/high.model", "buildings/hellbourne/range_rax/clips/default_1.clip");
    //k2anim->Load("world/props/plants/goodplant/model.model", "world/props/plants/goodplant/clips/default_1.clip");
    //k2anim->Load("world/props/plants/badplant/model.model", "world/props/plants/badplant/clips/default_1.clip");
    //k2anim->Load("world/props/fish/model.model", "world/props/fish/clips/path_1.clip");
    //k2anim->Load("world/props/halloween_props/pumkinward.model", "world/props/halloween_props/pumkinward.clip");
    //k2anim->Load("world/props/halloween_props/ghost/model.model", "world/props/halloween_props/ghost/default_1.clip");
    //k2anim->Load("world/props/winter/snowman/model.model", "world/props/winter/snowman/default_1.clip");
    //k2anim->Load("heroes/aluna/high.model", "heroes/aluna/clips/default_1.clip");
    
    k2anim->_UpdateBonesHelper();

    DemiDemo::GetApp()->GetInputManager()->registerMousePressEvent("testeuler",
        [](const OIS::MouseEvent& evt, OIS::MouseButtonID id){
            if (id == OIS::MB_Right)
                k2anim->mBonesHelper->SetVisible(!k2anim->mBonesHelper->GetVisible());
            else if (id == OIS::MB_Left)
                k2anim->_UpdateClipsHelper();

        });
}

void UpdateScene()
{
    k2anim->_UpdateClipsHelper();
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: K2 Anim"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open();

	return 0;
}
