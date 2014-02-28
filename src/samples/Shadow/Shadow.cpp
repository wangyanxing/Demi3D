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



void InitScene()
{
    float scale = 0.5f;
    DiDirLightPtr dirlight;
    DiDebugHelperPtr dbghelper;
    auto mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");

    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.3f,0.3f,0.3f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor(0.5f,0.5f,0.5f));
    dirlight->SetDirection(DiVec3(1,-1,0.4).normalisedCopy());
    dirlight->SetShadowCastEnable(true);

    DiSimpleShapePtr plane = make_shared<DiSimpleShape>();
    plane->SetShadowCastEnable(false);
    plane->CreatePlane(6000, 6000);
    plane->SetMaterial(mat);
    plane->GetMaterial()->SetDiffuse(DiColor::White);
    DiCullNode* planeNode = sm->GetRootNode()->CreateChild();
    planeNode->AttachObject(plane);
    planeNode->Translate(0, -107 * scale, 0);

//     for (int i = -10; i <= 10; ++i)
//     {
//         DiCullNode* boxNode = sm->GetRootNode()->CreateChild();
//         DiSimpleShapePtr box = make_shared<DiSimpleShape>();
//         box->CreateBox(30);
//         box->SetShadowCastEnable(true);
//         auto mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");
//         mat->SetAmbient(DiColor(0.2f, 0.2f, 0.2f));
//         box->SetMaterial(mat);
//         boxNode->AttachObject(box);
//         boxNode->Scale(1, 8, 1);
//         boxNode->SetPosition(0, 90, i * 100);
//     }


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

//     DiModelPtr model = make_shared<DiModel>("knot", "knot.model");
//     auto mat = DiMaterial::QuickCreate("lambert_v", "lambert_p");
//     mat->SetAmbient(DiColor(0.2f, 0.2f, 0.2f));
//     model->SetMaterial(mat);
// 
//     DiCullNode* boxNode = sm->GetRootNode()->CreateChild();
//     boxNode->AttachObject(model);
//     boxNode->Scale(5, 5, 5);
//     boxNode->Translate(0, 50, 0);

    DiCamera* camera = sm->GetCamera();
    camera->SetNearClipDistance(5);
    camera->SetFarClipDistance(5000);
    camera->SetPosition(0, 0, 500);

    //DiShadowParams& params = dirlight->GetShadowParams();
    //params.splits[0] = 200;
    //params.splits[1] = 400;
    //params.splits[2] = 600;
    //params.splits[3] = 800;
    //params.nonUniform = true;
    //params.quantize = 0.5f;
    //params.minView = 3.0f;
    //dirlight->SetupShadowCamera(sm);

    //dbghelper = make_shared<DiDebugHelper>();
    //sm->AttachObject(dbghelper);
}

void UpdateScene()
{
//     DiSceneManager* sm = DiBase::Driver->GetSceneManager();
//     
//     DiShadowParams& params = dirlight->GetShadowParams();
//     dbghelper->Clear();
//     if (params.shadowCameras[0])
//         dbghelper->AddFrustum(params.shadowCameras[0], DiColor::White);
//     dbghelper->AddFrustum(params.shadowCameras[1], DiColor::Red);
//     dbghelper->AddFrustum(params.shadowCameras[2], DiColor::Green);
//     dbghelper->AddFrustum(params.shadowCameras[3], DiColor::Blue);
}

int main(int argc, char *argv[])
{
    DemiDemo app(DemoConfig("Demi3D Sample - Shadow"));
    app.SetInitCallback(functor(InitScene));
    //app.SetUpdateCallback(functor(UpdateScene));
    app.Open();

    return 0;
}