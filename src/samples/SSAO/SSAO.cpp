
#include "DiPch.h"
#include "Application.h"
#include "Skybox.h"
#include "SimpleShape.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShaderManager.h"
#include "RenderWindow.h"
#include "CameraHelper.h"
#include "PostEffectManager.h"
#include "PostEffect.h"
#include "PostEffectPass.h"
#include "RenderPipeline.h"

DiVec4 ssaoParams(0.38f,0.027f,1.65f,0.559f);
DiPostEffectPass* passSSAO = NULL;

void AddMeshes()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
	DiModelPtr model = make_shared<DiModel>("sponza_model","sponza.model");

	cullnode->AttachObject(model);
	cullnode->SetScale(6,6,6);

	//sm->GetCamera()->SetFarClipDistance(200);
}

void SetupPostEffects()
{
    DiBase::Driver->GetPipeline()->SetEnableGBuffer(true);
	DiPostEffectManager* peMgr = DiApp::GetApp()->GetRenderWindow()->GetPostEffectManager();

	DiPostEffect* ssaoCrytek = peMgr->CreatePostEffect("SSAO_Crytek");
	ssaoCrytek->SetEnable(true);

    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

  	passSSAO = ssaoCrytek->CreatePass("output");
 	passSSAO->SetQuadScale(1);
  	passSSAO->BuildMaterial("screen_quad.vsh","ssao_crytek_orig.psh");
	passSSAO->SetInputGBuffer("sSceneDepthSampler",0);
	passSSAO->SetInputTex2D("sRotSampler4x4","PointsOnSphere4x4.dds");

	passSSAO->SetInput("SSAO_params", ssaoParams);
	
	passSSAO->SetInput("cFov",0.6f);
	//passSSAO->SetInput("cFov",sm->GetCamera()->GetFOVy().valueRadians());
	passSSAO->SetInput("cSampleLengthWorldSpace",0.38f);
	passSSAO->SetInput("cOffsetScale",1.63f);
	passSSAO->SetInput("cDefaultAccessibility",-0.32f);
	passSSAO->SetInput("cEdgeHighlight",2.03f);

	DiPostEffect* effectBox = peMgr->CreatePostEffect("BoxFilter");
	effectBox->SetEnable(true);
	DiPostEffectPass* passFilter = effectBox->CreatePass("output");
 	passFilter->SetQuadScale(1);
  	passFilter->BuildMaterial("screen_quad.vsh","post_box_filter.psh");
	passFilter->SetInput("image",INPUT_PREV_EFFECT);
}

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	sm->SetAmbientColor(DiColor(0.1f,0.1f,0.1f,0.1f));

	DiDirLight* dirlight = sm->CreateDirLight();
	dirlight->SetColor(DiColor());
	dirlight->SetDirection(DiVec3(1,1,2).normalisedCopy());

	AddMeshes();

	SetupPostEffects();
}

void UpdateScene()
{
	//DiKeyBoard* key = DiApp::GetApp()->GetInputSystem()->GetKeyboard();

	//if(key->IsKeyPressing(KC_Q)) {
	//	ssaoParams.x += 0.02f;
	//	ssaoParams.x = DiMath::Clamp(ssaoParams.x, 0.0f, 2.0f);
	//} else if(key->IsKeyPressing(KC_A)) {
	//	ssaoParams.x -= 0.02f;
	//	ssaoParams.x = DiMath::Clamp(ssaoParams.x, 0.0f, 2.0f);
	//} else if(key->IsKeyPressing(KC_W)) {
	//	ssaoParams.y += 0.002f;
	//	ssaoParams.y = DiMath::Clamp(ssaoParams.y, 0.0f, 2.0f);
	//} else if(key->IsKeyPressing(KC_S)) {
	//	ssaoParams.y -= 0.002f;
	//	ssaoParams.y = DiMath::Clamp(ssaoParams.y, 0.0f, 2.0f);
	//} else if(key->IsKeyPressing(KC_E)) {
	//	ssaoParams.z += 0.01f;
	//	ssaoParams.z = DiMath::Clamp(ssaoParams.z, -10.0f, 10.0f);
	//} else if(key->IsKeyPressing(KC_D)) {
	//	ssaoParams.z -= 0.01f;
	//	ssaoParams.z = DiMath::Clamp(ssaoParams.z, -10.0f, 10.0f);
	//} else if(key->IsKeyPressing(KC_R)) {
	//	ssaoParams.w += 0.01f;
	//	ssaoParams.w = DiMath::Clamp(ssaoParams.w, -10.0f, 10.0f);
	//} else if(key->IsKeyPressing(KC_F)) {
	//	ssaoParams.w -= 0.01f;
	//	ssaoParams.w = DiMath::Clamp(ssaoParams.w, -10.0f, 10.0f);
	//}

	//passSSAO->SetInput("SSAO_params", ssaoParams);
}

int main(){

	DiApp app;
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open(1024, 768, "Sample - SSAO");

	while (app.IsOpen())
	{
		app.Update();
	}

	return 0;
}