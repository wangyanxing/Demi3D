
#include "Demi.h"
#include "DemoFrame.h"

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

    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f, 0.1f));

    DiDirLight* dirlight = sm->CreateDirLight();
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(1, 1, 2).normalisedCopy());

    DiMaterialPtr mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");
    DiSimpleShapePtr model = make_shared<DiSimpleShape>();
    //model->CreateSphere(10, 16, 16);
    model->CreateTorus(8, 2, 32, 32);

    mat->SetDiffuse(DiColor::Red);
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

    CreateModels();
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
	DemiDemo app(DemoConfig("Demi test: basic scene"));
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
    app.Open();

	return 0;
}