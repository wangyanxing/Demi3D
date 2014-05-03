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

float timeElap = 0;
DiShaderParameter* shaderParam = nullptr;
DiCullNode* node = nullptr;

void AddMeshes()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	DiMaterialPtr m0 = DiMaterial::QuickCreate("lava_v","lava_p");
	m0->SetDiffuse(DiColor(1,0,0));
	m0->SetAmbient(DiColor(0.6f,0.2f,0));
	
	shaderParam = m0->GetShaderParameter();
	shaderParam->WriteFloat("fogDensity",0.45f);
	shaderParam->WriteFloat2("uvScale",DiVec2(3,1));
	shaderParam->WriteFloat3("fogColor",DiVec3(0,0,0));
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
	shaderParam->WriteTexture2D("texture1","cloud.pvr");
	shaderParam->WriteTexture2D("texture2","lavatile.pvr");
#else
	shaderParam->WriteTexture2D("texture1","cloud.png");
	shaderParam->WriteTexture2D("texture2","lavatile.jpg");
#endif

	DiSimpleShapePtr model = make_shared<DiSimpleShape>();
	model->CreateTorus(16,8,32,32);
    //model->CreateSphere(15);
    //model->CreateBox(25);
	model->SetMaterial(m0);
	node = sm->GetRootNode()->CreateChild();
	node->AttachObject(model);
}

void SetupPostEffects()
{
	DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
	DiPostEffect* bloom = peMgr->GetEffect("Gray");
    if (bloom)
		bloom->SetEnable(true);
    
	//DiPostEffect* film = peMgr->GetEffect("Film");
    //if (film)
	//	film->SetEnable(true);
}

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	DemiDemo::GetApp()->GetCameraHelper()->Enable(false);
	sm->GetCamera()->SetPosition(0,0,80);
	sm->GetCamera()->LookAt(0,0,0);

	AddMeshes();

	SetupPostEffects();
}

void UpdateScene()
{
	// update the material
    float delta = DiBase::Driver->GetDeltaSecond();
	timeElap += delta;
	shaderParam->WriteFloat("time",timeElap);

	node->Yaw(DiRadian(0.0625f * delta));
	node->Pitch(DiRadian(0.25f * delta));
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi3D Sample - Lava", 1500, 800));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open(argc, argv);

	return 0;
}