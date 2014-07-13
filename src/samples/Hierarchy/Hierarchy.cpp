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

DiVector<DiCullNode*> nodes;

DiMaterialPtr mat;
void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");
    mat->SetDiffuse(DiColor::White);

    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f, 0.1f));

    DiDirLightPtr dirlight = make_shared<DiDirLight>();
    sm->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(-1, -1, -2).normalisedCopy());

    int amount = 2;

    DiCullNode* parent = sm->GetRootNode();

    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(10, 0, 0);
        nodes.push_back(node);
        parent = node;
    }

    parent = sm->GetRootNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(-10, 0, 0);
        nodes.push_back(node);
        parent = node;
    }

    parent = sm->GetRootNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, 10, 0);
        nodes.push_back(node);
        parent = node;
    }

    parent = sm->GetRootNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, -10, 0);
        nodes.push_back(node);
        parent = node;
    }

    parent = sm->GetRootNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, 0, 10);
        nodes.push_back(node);
        parent = node;
    }

    parent = sm->GetRootNode();
    for (int i = 0; i < amount; i++)
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, 0, -10);
        nodes.push_back(node);
        parent = node;
    }
}

void UpdateScene()
{
    float time = DiBase::Driver->GetElapsedSecond();
    float rx = DiMath::Sin(time * 0.7f) * 0.2f;
    float ry = DiMath::Sin(time * 0.3f) * 0.1f;
    float rz = DiMath::Sin(time * 0.2f) * 0.1f;

    DiMat3 m;
    DiQuat rot;
    m.FromEulerAnglesXYZ(DiRadian(rx), DiRadian(ry), DiRadian(rz));
    rot.FromRotationMatrix(m);

    for (size_t i = 0; i < nodes.size(); i++)
        nodes[i]->SetOrientation(rot);
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi3D Sample - Hierarchy"));
	app.SetInitCallback(InitScene);
    app.SetUpdateCallback(UpdateScene);
	app.Open(argc, argv);

	return 0;
}