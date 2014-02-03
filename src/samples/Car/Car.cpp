
#include "Demi.h"
#include "DemoFrame.h"

void AddMeshes()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
	
	DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
	DiModelPtr model = make_shared<DiModel>("car","F50N_H.model");

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

int main(int argc, char *argv[])
{
    DemiDemo app(argc, argv, DemoConfig("Demi3D Sample - Car", 1500, 800));
	app.SetInitCallback(functor(InitScene));
	app.Open();

	return 0;
}