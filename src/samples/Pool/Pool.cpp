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

/************************************************************************/
/* This sample is adapted from Ogre's fresnel sample                    */
/************************************************************************/

#include "Demi.h"
#include "DemoFrame.h"

void SetupWater(DiSceneManager* sm)
{
    // Render targets
    auto reflectMap = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>("reflect_rt");
    reflectMap->SetDimensions(512, 512);
    reflectMap->SetFormat(PF_A8R8G8B8);
    reflectMap->SetUsage(TU_RENDER_TARGET);
    reflectMap->SetAutoMipmap(false);
    reflectMap->SetAddressing(AM_CLAMP);
    reflectMap->CreateTexture();
    auto reflectRT = reflectMap->GetRenderTarget();

    auto refractMap = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>("refract_rt");
    refractMap->SetDimensions(512, 512);
    refractMap->SetFormat(PF_A8R8G8B8);
    refractMap->SetUsage(TU_RENDER_TARGET);
    refractMap->SetAutoMipmap(false);
    refractMap->SetAddressing(AM_CLAMP);
    refractMap->CreateTexture();
    auto refractRT = refractMap->GetRenderTarget();

    // Water plane
    DiCullNode* nodePlane = sm->GetRootNode()->CreateChild();
    DiSimpleShapePtr model = make_shared<DiSimpleShape>();
    model->CreatePlane(100);
    auto waterMat = DiMaterial::QuickCreate("pool_fresnel_v", "pool_fresnel_p");
    auto shaderParam = waterMat->GetShaderParameter();
    shaderParam->WriteFloat("scale", 1.0f);
    shaderParam->WriteFloat("scroll", 0.05f);
    shaderParam->WriteFloat("fresnelBias", -0.1f);
    shaderParam->WriteFloat("fresnelScale", 1.8f);
    shaderParam->WriteFloat("fresnelPower", 8.0f);
    shaderParam->WriteFloat4("tintColour", DiVec4(0, 0.05f, 0.05f, 1));
    shaderParam->WriteFloat("noiseScale", 0.05f);
    shaderParam->WriteTexture2D("noiseMap", "waves2.dds");
    shaderParam->WriteTexture2D("reflectMap", "reflect_rt");
    shaderParam->WriteTexture2D("refractMap", "refract_rt");

    model->SetMaterial(waterMat);
    nodePlane->AttachObject(model);
    nodePlane->SetScale(7, 1, 13);

    // Add to scene manager with the callbacks
    DiCamera* cam = sm->GetCamera();
    sm->AddExtraRenderTarget(reflectRT, cam,
        [nodePlane, cam](DiRenderTarget* rt) {
            nodePlane->SetVisible(false);
            cam->EnableReflection(DiPlane(DiVec3::UNIT_Y, 0));
        },
        [nodePlane, cam](DiRenderTarget* rt) {
            nodePlane->SetVisible(true);
            cam->DisableReflection();
        });
    
    sm->AddExtraRenderTarget(refractRT, cam,
        [nodePlane](DiRenderTarget* rt) {
            nodePlane->SetVisible(false);
        },
        [nodePlane](DiRenderTarget* rt) {
            nodePlane->SetVisible(true);
        });
}

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    sm->AttachObject(make_shared<DiModel>("columns", "columns.model"));
    sm->AttachObject(make_shared<DiModel>("bathlower", "romanbathlower.model"));
    sm->AttachObject(make_shared<DiModel>("bathupper", "romanbathupper.model"));

    sm->GetSkybox()->Enable(1000, "park.dds");
    
    SetupWater(sm);
}

int main(int argc, char *argv[])
{
    DemiDemo app(DemoConfig("Demi3D Sample - Pool"));
    app.SetInitCallback(functor(InitScene));
    app.Open();

    return 0;
}