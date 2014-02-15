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

void CreateModels()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    const int size = 3;
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
}

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f, 0.1f));

    DiDirLightPtr dirlight = make_shared<DiDirLight>();
    sm->AttachObject(dirlight);
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