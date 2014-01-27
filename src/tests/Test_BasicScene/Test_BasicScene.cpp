
#include "DiPch.h"
#include "Application.h"
#include "Skybox.h"
#include "SimpleShape.h"
#include "AnimModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShaderManager.h"
#include "RenderWindow.h"
#include "ClipController.h"
#include "SimpleShape.h"
#include "RenderThread.h"
#include "GfxDriver.h"

void CreateModels()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
#if 0
    const int size = 10;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                DiMaterialPtr mat = DiMaterial::QuickCreate("lambert_v", "lambert_p", CSF_SKINNED);
                mat->SetDiffuse(DiColor(1, 1, 1));

                DiString name;
                name.Format("md_%d_%d", i, j);
                DiAnimModelPtr model = make_shared<DiAnimModel>(name, "robot.model", "robot.motion");
                model->SetMaterial(mat);
                model->SetAutoUpdateAnims(true);
                model->GetClipSet()->GetClipController("Walk")->SetEnabled(true);

                DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
                cullnode->AttachObject(model);
                cullnode->SetPosition(i * 120.0f, j * 120.0f, k * 120.0f);
            }
        }
    }
#endif

    DiMaterialPtr mat = DiMaterial::QuickCreate("basic_v", "basic_p");
    DiSimpleShapePtr model = make_shared<DiSimpleShape>();
    model->CreateSphere(10, 25, 25);
    //model->CreateTorus(8, 2, 32, 32);

    model->SetMaterial(mat);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->AttachObject(model);
}

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f, 0.1f));

    DiDirLight* dirlight = sm->CreateDirLight();
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(1, 1, 2).normalisedCopy());

    DiRenderThread* rt = new DiRenderThread();
    rt->Start();
   
    CreateModels();
}

void UpdateScene()
{
}

int main()
{
	DiApp app;
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open(800, 600, "basic test");

	while (app.IsOpen())
	{
		app.Update();
	}

	return 0;
}