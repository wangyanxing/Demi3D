
#include "Demi.h"
#include "DemoFrame.h"

const int numMeshes = 500;
DiVector<DiCullNode*> sphereMeshes;

void AddMeshes()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	sphereMeshes.reserve(numMeshes);

	for(int i = 0; i < numMeshes; i++) 
    {

		DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
		DiSimpleShapePtr model = make_shared<DiSimpleShape>();

		DiMaterialPtr mat = DiMaterial::QuickCreate("sample_fresnel_v","sample_fresnel_p");

		model->CreateSphere(10,20,20);
		model->SetMaterial(mat);
		mat->GetShaderParameter()->WriteFloat("mRefractionRatio",1.02f);
		mat->GetShaderParameter()->WriteFloat("mFresnelBias",0.1f);
		mat->GetShaderParameter()->WriteFloat("mFresnelScale",1.0f);
		mat->GetShaderParameter()->WriteFloat("mFresnelPower",2.0f);
		mat->GetShaderParameter()->WriteTextureCUBE("cubeMap","park.dds");

		cullnode->AttachObject(model);

		cullnode->SetPosition(DiMath::UnitRandom()*1000 - 500,
			DiMath::UnitRandom()*1000 - 500,DiMath::UnitRandom()*1000 - 500);
		cullnode->SetScale(DiMath::UnitRandom()*3 + 1);

		sphereMeshes.push_back(cullnode);
	}
}

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	sm->SetAmbientColor(DiColor(0.1f,0.1f,0.1f,0.1f));

	DiDirLight* dirlight = sm->CreateDirLight();
	dirlight->SetColor(DiColor());
	dirlight->SetDirection(DiVec3(1,1,2).normalisedCopy());

	sm->GetSkybox()->Enable(10000,"park.dds");

    DiApp::GetApp()->GetCameraHelper()->SetStyle(CS_SMOOTH);
    sm->GetCamera()->SetPosition(0, 0, 300);
	sm->GetCamera()->LookAt(0,0,0);

	AddMeshes();
}

void UpdateScene()
{
	float time = DiBase::Driver->GetElapsedSecond() * 0.1f;
	
	for (int i = 0; i < numMeshes; i++)
    {
		float z = sphereMeshes[i]->GetPosition().z;
		sphereMeshes[i]->SetPosition(500 * DiMath::Cos( time + i ),
			500 * DiMath::Sin( time + i * 1.1f), z);
	}
}

int main()
{

	DiApp app;
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open(1700, 1000, "Demi3D Sample - Fresnel Material");

	while (app.IsOpen())
	{
		app.Update();
	}

	return 0;
}