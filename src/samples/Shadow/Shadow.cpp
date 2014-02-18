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

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.3f,0.3f,0.3f));

    DiDirLightPtr dirlight = make_shared<DiDirLight>();
    sm->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(1, 1, 2).normalisedCopy());

    DiSimpleShapePtr plane = make_shared<DiSimpleShape>();
    plane->CreatePlane(300);
    plane->SetMaterial(DiMaterial::QuickCreate("lambert_v", "lambert_p"));
    sm->AttachObject(plane);

    DiModelPtr model = make_shared<DiModel>("knot", "knot.model");
    auto mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");
    mat->SetAmbient(DiColor(0.2f, 0.2f, 0.2f));
    model->SetMaterial(mat);

    DiCullNode* boxNode = sm->GetRootNode()->CreateChild();
    boxNode->AttachObject(model);
    boxNode->Scale(5, 5, 5);
    boxNode->Translate(0, 50, 0);
}

int main(int argc, char *argv[])
{
    DemiDemo app(DemoConfig("Demi3D Sample - Shadow"));
    app.SetInitCallback(functor(InitScene));
    app.Open();

    return 0;
}