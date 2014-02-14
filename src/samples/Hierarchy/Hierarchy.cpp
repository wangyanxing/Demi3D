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
DiVector<DiSceneNodePtr> scenenodes;

#define USE_OLD_SCENE_MANAGER

DiMaterialPtr mat;
void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");
    mat->SetDiffuse(DiColor::White);

    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f, 0.1f));

    DiDirLightPtr dirlight = sm->CreateDirLight();
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(1, 1, 2).normalisedCopy());

    int amount = 80;

    DiCullNode* parent = sm->GetRootNode();
    DiSceneNodePtr root = sm->GetRootSceneNode();

    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

#ifdef USE_OLD_SCENE_MANAGER
        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(10, 0, 0);
        nodes.push_back(node);
        parent = node;
#else
        DiSceneNodePtr nd = root->CreateChild();
        nd->AddObject(model);
        nd->Translate(DiVec3(10, 0, 0));
        scenenodes.push_back(nd);
        root = nd;
#endif
    }

    parent = sm->GetRootNode();
    root = sm->GetRootSceneNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

#ifdef USE_OLD_SCENE_MANAGER
        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(-10, 0, 0);
        nodes.push_back(node);
        parent = node;
#else
        DiSceneNodePtr nd = root->CreateChild();
        nd->AddObject(model);
        nd->Translate(DiVec3(-10, 0, 0));
        scenenodes.push_back(nd);
        root = nd;
#endif
    }

    parent = sm->GetRootNode();
    root = sm->GetRootSceneNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

#ifdef USE_OLD_SCENE_MANAGER
        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, 10, 0);
        nodes.push_back(node);
        parent = node;
#else
        DiSceneNodePtr nd = root->CreateChild();
        nd->AddObject(model);
        nd->Translate(DiVec3(0, 10, 0));
        scenenodes.push_back(nd);
        root = nd;
#endif
    }

    parent = sm->GetRootNode();
    root = sm->GetRootSceneNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

#ifdef USE_OLD_SCENE_MANAGER
        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, -10, 0);
        nodes.push_back(node);
        parent = node;
#else
        DiSceneNodePtr nd = root->CreateChild();
        nd->AddObject(model);
        nd->Translate(DiVec3(0, -10, 0));
        scenenodes.push_back(nd);
        root = nd;
#endif
    }

    parent = sm->GetRootNode();
    root = sm->GetRootSceneNode();
    for (int i = 0; i < amount; i++) 
    {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

#ifdef USE_OLD_SCENE_MANAGER
        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, 0, 10);
        nodes.push_back(node);
        parent = node;
#else
        DiSceneNodePtr nd = root->CreateChild();
        nd->AddObject(model);
        nd->Translate(DiVec3(0, 0, 10));
        scenenodes.push_back(nd);
        root = nd;
#endif
    }

    parent = sm->GetRootNode();
    root = sm->GetRootSceneNode();
    for (int i = 0; i < amount; i++) {
        DiSimpleShapePtr model = make_shared<DiSimpleShape>();
        model->CreateBox(10);
        model->SetMaterial(mat);

#ifdef USE_OLD_SCENE_MANAGER
        DiCullNode* node = parent->CreateChild();
        node->AttachObject(model);
        node->Translate(0, 0, -10);
        nodes.push_back(node);
        parent = node;
#else
        DiSceneNodePtr nd = root->CreateChild();
        nd->AddObject(model);
        nd->Translate(DiVec3(0, 0, -10));
        scenenodes.push_back(nd);
        root = nd;
#endif
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

#ifdef USE_OLD_SCENE_MANAGER
    for (size_t i = 0; i < nodes.size(); i++)
        nodes[i]->SetOrientation(rot);
#else
    for (size_t i = 0; i < scenenodes.size(); i++)
        scenenodes[i]->SetRotation(rot);
#endif
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi3D Sample - Hierarchy"));
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open();

	return 0;
}