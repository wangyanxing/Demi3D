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
#include "EnginePlugin.h"
#include "TransAxes.h"

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    DiDirLightPtr dirlight;
    sm->SetAmbientColor(DiColor(0.2f, 0.2f, 0.2f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(0.5f,-0.86f,0).normalisedCopy());
    //dirlight->SetShadowCastEnable(true);

#if 0
    DiSimpleShapePtr lightSphere = make_shared<DiSimpleShape>("lightSphere");
    lightSphere->CreatePlane(40,40);
    DiMaterialPtr m = DiMaterial::QuickCreate("k2_water_v", "k2_water_p", SHADER_FLAG_USE_ENV_MAP | SHADER_FLAG_ENV_MAP_MIX);
    //m->SetDiffuse(DiColor(1, 0.8f, 0));
    lightSphere->SetMaterial(m);

    m->SetSpecular(DiColor::White);
    m->SetShininess(1438);
    auto pm = m->GetShaderParameter();
    m->SetBlendMode(BLEND_ALPHA);
    m->SetOpacity(0.5f);
#if 0
    pm->WriteFloat2("v_Rotation", DiVec2(1,-1));
    pm->WriteFloat3("v_TextureParams", DiVec3(0.3f, 0.6f, 0.02f));
#endif
    pm->WriteFloat("fSpeed", 0.02f);
    pm->WriteTexture2D("map", "water_color.dds");
    pm->WriteTextureCUBE("envMap", "deadlock.dds");
    pm->WriteTexture2D("normalmap1", "water_normal_rxgb.dds");
    pm->WriteTexture2D("normalmap2", "water_normal_rxgb.dds");

    DiCullNode* nd = sm->GetRootNode()->CreateChild();
    nd->AttachObject(lightSphere);

    DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
    DiPostEffect* bloom = peMgr->GetEffect("Bloom");
    //bloom->SetEnable(true);
#endif
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
	app.Open(argc, argv);

	return 0;
}