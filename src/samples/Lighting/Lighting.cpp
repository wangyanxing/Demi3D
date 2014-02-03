
#include "Demi.h"
#include "DemoFrame.h"


DiCullNode* lightSphereNode = NULL;
DiPointLight* pointlight = NULL;

void AddMesh(float x, float y, const DiColor& col, const DiString& shader)
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
	DiCullNode* cullnode = sm->GetRootNode()->CreateChild();

	DiSimpleShapePtr model = make_shared<DiSimpleShape>();
	if(y > 10)
		model->CreateTorus(8,2,32,32);
	else
		model->CreateSphere(10,20,20);

	DiMaterialPtr mat = DiMaterial::QuickCreate(shader + "_v", shader + "_p");
	mat->SetDiffuse(col);

	if(shader == "phong")
    {
		mat->SetSpecular(DiColor(0.8f,0.8f,0.8f));
		mat->SetShininess(y > 10 ? 10.0f : 30.0f);
	}

	model->SetMaterial(mat);
	cullnode->AttachObject(model);
	cullnode->SetPosition(x,y,1);
}

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	sm->SetAmbientColor(DiColor(0.1f,0.1f,0.1f,0.1f));

	DiDirLight* dirlight = sm->CreateDirLight();
	dirlight->SetColor(DiColor());
	dirlight->SetDirection(DiVec3(1,1,2).normalisedCopy());

	pointlight = sm->CreatePointLight();
	pointlight->SetColor(DiColor());
	pointlight->SetAttenuation(100);

	// visbile mesh for the point light
	DiSimpleShapePtr lightSphere = make_shared<DiSimpleShape>();
	lightSphere->CreateSphere(1,16,8);
	DiMaterialPtr m = DiMaterial::QuickCreate("basic_v","basic_p");
	m->SetDiffuse(DiColor(1,0.8f,0));
	lightSphere->SetMaterial(m);

	lightSphereNode = sm->GetRootNode()->CreateChild();
	lightSphereNode->AttachObject(lightSphere);

	float y1 = 0, y2 = -20, y3 = 20;      
                             
	AddMesh( -60, y1, DiColor(0.1f,0.1f,0.1f),"phong");
	AddMesh( -60, y2, DiColor(0.1f,0.1f,0.1f),"lambert");
	AddMesh( -60, y3, DiColor(0.1f,0.1f,0.1f),"phong");
	                
	AddMesh( -40, y1, DiColor(1.0f,1.0f,1.0f),"phong");
	AddMesh( -40, y2, DiColor(1.0f,1.0f,1.0f),"lambert");
	AddMesh( -40, y3, DiColor(1.0f,1.0f,1.0f),"phong");
	                
	AddMesh( -20, y1, DiColor(1.0f,0.33f,0.0f),"phong");
	AddMesh( -20, y2, DiColor(1.0f,0.33f,0.0f),"lambert");
	AddMesh( -20, y3, DiColor(1.0f,0.33f,0.0f),"phong");
	                
	AddMesh(   0, y1, DiColor(1.0f,0.67f,0.0f),"phong");
	AddMesh(   0, y2, DiColor(1.0f,0.67f,0.0f),"lambert");
	AddMesh(   0, y3, DiColor(1.0f,0.67f,0.0f),"phong");
	                 
	AddMesh(  20, y1, DiColor(0.33f,1.0f,0.0f),"phong");
	AddMesh(  20, y2, DiColor(0.33f,1.0f,0.0f),"lambert");
	AddMesh(  20, y3, DiColor(0.33f,1.0f,0.0f),"phong");
	                
	AddMesh(  40, y1, DiColor(0.0f,0.33f,1.0f),"phong");
	AddMesh(  40, y2, DiColor(0.0f,0.33f,1.0f),"lambert");
	AddMesh(  40, y3, DiColor(0.0f,0.33f,1.0f),"phong");
	                
	AddMesh(  60, y1, DiColor(0.33f,0.0f,1.0f),"phong");
	AddMesh(  60, y2, DiColor(0.33f,0.0f,1.0f),"lambert"); 
	AddMesh(  60, y3, DiColor(0.33f,0.0f,1.0f),"phong");
}

void UpdateScene()
{
	float time = DiBase::Driver->GetElapsedSecond();
	lightSphereNode->SetPosition(70 * DiMath::Cos(time), 30 * DiMath::Sin(time), 70 * DiMath::Sin(time));
	pointlight->SetPosition(lightSphereNode->GetPosition());
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi3D Sample - Basic Lighting"));
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open();

	return 0;
}