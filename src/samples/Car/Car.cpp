
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

void AddMeshes()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
	
	DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
	DiModelPtr model = make_shared<DiModel>("test","F50N_H.model");

	//DiSimpleShape* model = DI_NEW DiSimpleShape();
	//model->CreateBox();
	//model->SetMaterial("F50N_H_F50ROJO.mtl");

	cullnode->AttachObject(model);
	cullnode->SetScale(0.02f,0.02f,0.02f);
}

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	sm->SetAmbientColor(DiColor(0.02f,0.02f,0.02f));

	DiDirLight* dirlight0 = sm->CreateDirLight();
	dirlight0->SetColor(DiColor(1,1,1,2));
	dirlight0->SetDirection(DiVec3(2,1.2f,10).normalisedCopy());

	DiDirLight* dirlight1 = sm->CreateDirLight();
	dirlight1->SetColor(DiColor(1,1,1,1));
	dirlight1->SetDirection(DiVec3(-2,-1.2f,-10).normalisedCopy());

	sm->GetSkybox()->Enable(10000,"bridge.dds");

	AddMeshes();
}

void UpdateScene()
{

}

int main(){

	DiApp app;
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open(1700, 1000, "Demi3D Sample - Car");

	while (app.IsOpen())
	{
		app.Update();
	}

	return 0;
}