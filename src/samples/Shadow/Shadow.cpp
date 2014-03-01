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
#include "DebugHelper.h"

void SetupScene()
{
    DiVector<DiCullNode*> nodes;
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default003", "default003.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(4.92384f, 0, 9.12999f);
        node->SetOrientation(0, 0, 1, 0);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default002", "default002.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(-9.47882f, 0, 11.8899f);
        node->SetOrientation(0, 0, 1, 0);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default001", "default001.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(6.91004f, 0, -8.2187);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default", "default.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(-12.6195, 0, -11.4676);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default004", "default004.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(25.4397f, 0, -0.0661511f);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default005", "default005.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(12.4845f, 0, 9.83395f);
        node->SetOrientation(0, 0, 1, 0);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default006", "default006.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(-20.3926f, 0, 10.2288f);
        node->SetScale(0.0478292f, 0.0478292f, 0.0478292f);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default007", "default007.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(-3.18984f, 0, -12.4651f);
        node->SetScale(0.0198347f, 0.0198347f, 0.0198347f);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default008", "default008.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(-0.0778171f, 0, 11.5481f);
        node->SetScale(0.0943205f, 0.0943205f, 0.0943205f);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default009", "default009.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(21.9934f, 0, 4.21011f);
        node->SetScale(0.0315056f, 0.0315056f, 0.0315056f);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("default010", "default010.model");
        model->SetShadowCastEnable(true);
        node->AttachObject(model);
        node->SetPosition(19.0808f, 0, 7.58754f);
        node->SetScale(0.0315056f, 0.0315056f, 0.0315056f);
        nodes.push_back(node);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("Plane001", "plane001.model");
        model->SetShadowCastEnable(false);
        node->AttachObject(model);
        node->SetPosition(-1.10306f, 0, -0.0453091f);
        node->SetScale(5, 1, 5);
        nodes.push_back(node);
    }
    for (auto i : nodes)
    {
        auto s = i->GetScale();
        auto p = i->GetPosition();
        i->SetScale(s * 50);
        i->SetPosition(p * 50);
    }
}

void InitScene()
{
    float scale = 0.5f;
    DiDirLightPtr dirlight;
    DiDebugHelperPtr dbghelper;
    auto mat = DiMaterial::QuickCreate("lambert_v", "lambert_p", SHADER_FLAG_SHADOW_HIGH);
    mat->SetAmbient(DiColor(0.1f, 0.1f, 0.1f));

    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.1f, 0.1f, 0.1f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor(0.9f,0.9f,0.9f));
    dirlight->SetDirection(DiVec3(0.3f,-0.7f,0.4).normalisedCopy());
    dirlight->SetShadowCastEnable(true);

#if 0
    DiSimpleShapePtr plane = make_shared<DiSimpleShape>();
    plane->SetShadowCastEnable(false);
    plane->CreatePlane(6000, 6000);
    plane->SetMaterial(mat);
    plane->GetMaterial()->SetDiffuse(DiColor::White);
    DiCullNode* planeNode = sm->GetRootNode()->CreateChild();
    planeNode->AttachObject(plane);
    planeNode->Translate(0, -107 * scale, 0);

    DiVec3 scaleVec(scale, scale, scale);
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("athene", "athene.model");
        model->SetShadowCastEnable(true);
        model->SetMaterial(mat);
        node->AttachObject(model);
        node->Translate(0, -27 * scale, 0);
        node->Yaw(DiDegree(90));
        node->SetScale(scaleVec);
    }
    {
        DiCullNode* node = sm->GetRootNode()->CreateChild();
        DiModelPtr model = make_shared<DiModel>("athene", "athene.model");
        model->SetShadowCastEnable(true);
        model->SetMaterial(mat);
        node->AttachObject(model);
        node->Translate(30, -27 * scale, 22);
        node->Yaw(DiDegree(-45));
        node->SetScale(scaleVec);
    }

    // Columns
    for (int x = -4; x <= 4; ++x)
    {
        for (int z = -4; z <= 4; ++z)
        {
            if (x != 0 || z != 0)
            {
                DiModelPtr model = make_shared<DiModel>("column", "column.model");
                DiCullNode* node = sm->GetRootNode()->CreateChild();
                model->SetShadowCastEnable(true);
                model->SetMaterial(mat);
                node->AttachObject(model);
                node->Translate(x * 500, 0, z * 500);
                node->SetScale(scale * 2, scale, scale * 2);
            }
        }

    }
#endif

    SetupScene();

    DiCamera* camera = sm->GetCamera();
    camera->SetNearClipDistance(5);
    camera->SetFarClipDistance(5000);
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app(DemoConfig("Demi3D Sample - Shadow", 800, 600));
    app.SetInitCallback(functor(InitScene));
    app.Open();

    return 0;
}