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
#include "SpotLight.h"

DiDebugHelper* hp;
DiLight* lt;
void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    sm->SetAmbientColor(DiColor(0.6f, 0.6f, 0.6f));
    
    float scale = 0.5f;
    DiDirLightPtr dirlight;
    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor(0.8f,0.8f,0.8f));
    dirlight->SetDirection(DiVec3(0.3f,-0.7f,0.4).normalisedCopy());
    //dirlight->InitForShadowCasting(sm, ShadowTextureConfig(1024,1024,PF_A32B32G32R32F));
    
    auto pos = DiVec3(150,275,130)*2;
    dirNode->SetPosition(pos);
    
    DiSpotLightPtr sptLt = make_shared<DiSpotLight>();
    dirNode->AttachObject(sptLt);
    sptLt->SetDirection((-pos).normalisedCopy());
    sptLt->SetRange( DiDegree(80), DiDegree(90) );
    sptLt->InitForShadowCasting(sm, ShadowTextureConfig(1024,1024,PF_A16B16G16R16F));
    
    DiDebugHelperPtr dbghelper;
    auto mat = DiMaterial::QuickCreate("lambert_v", "lambert_p", SHADER_FLAG_SHADOW_RECEIVER);
    mat->SetAmbient(DiColor(0.8f, 0.8f, 0.8f));
    mat->SetDiffuse(DiColor(0.8f, 0.8f, 0.8f));

    dbghelper = make_shared<DiDebugHelper>();
    sm->GetRootNode()->AttachObject(dbghelper);
    DiMaterialPtr helpermat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
    helpermat->SetDepthCheck(false);
    dbghelper->SetMaterial(helpermat);
    //dbghelper->AddFrustum(dirlight->GetShadowCamera(0), DiColor::Red);

    hp = dbghelper.get();
    lt = sptLt.get();

#if 0
    DiSimpleShapePtr plane = make_shared<DiSimpleShape>();
    plane->SetShadowCastEnable(false);
    plane->CreatePlane(600, 600);
    plane->SetMaterial(mat);
    plane->GetMaterial()->SetDiffuse(DiColor::White);
    DiCullNode* planeNode = sm->GetRootNode()->CreateChild();
    planeNode->AttachObject(plane);
    planeNode->Translate(0, 0, 0);

    const int size = 1;
    for (int i = -size; i <= size; i++)
    {
        for (int j = -size; j <= size; j++)
        {
            DiMaterialPtr mat = DiMaterial::QuickCreate("lambert_v", "lambert_p", SHADER_FLAG_SHADOW_RECEIVER);
            mat->SetDiffuse(DiColor(1, 1, 1));
            mat->SetAmbient(DiColor(0.7f, 0.7f, 0.7f));
            
            DiString name;
            name.Format("md_%d_%d", i, j);
            DiAnimModelPtr model = make_shared<DiAnimModel>(name, "robot.model", "robot.motion");
            //DiModelPtr model = make_shared<DiModel>(name, "robot.model");
            model->SetMaterial(mat);
            model->SetShadowCastEnable(true);
            
            model->SetAutoUpdateAnims(true);
            model->GetClipSet()->GetClipController("Walk")->SetEnabled(true);
            
            DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
            cullnode->AttachObject(model);
            cullnode->SetPosition(i * 140.0f, 0, j * 140.0f);
        }
    }
    
#else
    
    DiSimpleShapePtr plane = make_shared<DiSimpleShape>();
    plane->SetShadowCastEnable(false);
    plane->CreatePlane(300, 300);
    plane->SetMaterial(mat);
    plane->GetMaterial()->SetDiffuse(DiColor::White);
    DiCullNode* planeNode = sm->GetRootNode()->CreateChild();
    planeNode->AttachObject(plane);

    DiMaterialPtr m = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_SHADOW_RECEIVER);
    m->SetDiffuse(DiColor(0.9f, 0.9f, 0.9f));
    
    DiSimpleShapePtr box = make_shared<DiSimpleShape>();
    box->SetShadowCastEnable(true);
    box->CreateBox(10);
    box->SetMaterial(m);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->SetPosition(0,5,0);
    cullnode->AttachObject(box);

#endif

    //SetupScene();

    DiCamera* camera = sm->GetCamera();
    camera->SetNearClipDistance(5);
    camera->SetFarClipDistance(5000);
}

void UpdateScene()
{
    hp->Clear();
    hp->AddFrustum(lt->GetShadowCamera(0), DiColor::Red);
}

int main(int argc, char *argv[])
{
    DemiDemo app(DemoConfig("Demi3D Sample - Shadow", 800, 600));
    app.SetInitCallback(InitScene);
    app.SetUpdateCallback(UpdateScene);
    app.Open(argc, argv);

    return 0;
}