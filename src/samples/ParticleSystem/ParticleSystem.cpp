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
#include "BillboardSet.h"
#include "EffectManager.h"

#include "K2Model.h"
#include "K2Clip.h"
#include "K2Configs.h"
#include "K2Plugin.h"

#include "ParticleSystem.h"
#include "ParticleElement.h"
#include "ParticleEmitter.h"
#include "BoxEmitter.h"
#include "ColorController.h"
#include "TextureRotatorController.h"
#include "ScaleController.h"
#include "TokensParser.h"

#include "Property.h"

DiVector<DiParticleSystem*> g_fxs;
DiK2ModelPtr k2md = nullptr;

void InitFx_Torch()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    // model
    DiCullNode* nd1 = sm->GetRootNode()->CreateChild();
    auto k2md = make_shared<DiK2Model>("buildings/hellbourne/gate_torch/model.mdf");
    nd1->AttachObject(k2md);

    // effect
    auto _ps = DiEffectManager::GetInstance().CreateParticleSystemTemplate("Fx_gate_torch");
    g_fxs.push_back(_ps);
    std::shared_ptr<DiTransformUnit> ps(_ps);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->AttachObject(ps);
    _ps->Start();
    _ps->SetScale(DiVec3(0.3f));
    {
        DiParticleElement* element = _ps->CreateElement();
        element->SetRenderer("Billboard");
        auto emitter = element->CreateEmitter("Box");

        auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR | SHADER_FLAG_USE_MAP);
        auto texture = mat->GetShaderParameter()->WriteTexture2D("map", "dust_smoke_10.dds");
        mat->SetBlendMode(BLEND_ALPHA);
        texture->SetAddressing(AM_CLAMP);
        mat->SetDepthWrite(false);
        element->SetMaterialName(mat->GetName());

        ((DiBoxEmitter*)emitter)->SetWidth(40);
        ((DiBoxEmitter*)emitter)->SetHeight(40);
        ((DiBoxEmitter*)emitter)->SetDepth(40);
        emitter->position = DiVec3(0, 40, 0);

        auto angle = DI_NEW DiAttributeFixed();
        angle->SetValue(DiMath::DegreesToRadians(10));
        emitter->SetDynAngle(angle);

        auto ttl = DI_NEW DiAttributeRandom();
        ttl->SetMinMax(1.5f, 3.5f);
        emitter->SetDynTotalTimeToLive(ttl);
    }
}

void InitFx_Repeater02()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    // model
    DiCullNode* nd1 = sm->GetRootNode()->CreateChild();
    auto k2md = make_shared<DiK2Model>("buildings/legion/repeater2/model.mdf");
    //k2md = make_shared<DiK2Model>("heroes/aluna/model.mdf");
    nd1->AttachObject(k2md);
    //k2md->GetAnimation()->Play(K2PrefabClip::ANIM_IDLE);

#if 0
    // effect
    auto _ps = DiEffectManager::GetInstance().CreateParticleSystemTemplate("Fx_repeater2");
    g_fxs.push_back(_ps);
    std::shared_ptr<DiTransformUnit> ps(_ps);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->AttachObject(ps);
    _ps->Start();
    {
        DiParticleElement* element = _ps->CreateElement();
        element->SetRenderer("Billboard");
        auto emitter = element->CreateEmitter("Box");

        auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR | SHADER_FLAG_USE_MAP);
        auto texture = mat->GetShaderParameter()->WriteTexture2D("map", "smoke.dds");
        mat->SetBlendMode(BLEND_ALPHA);
        texture->SetAddressing(AM_CLAMP);
        mat->SetDepthWrite(false);
        element->SetMaterialName(mat->GetName());

        ((DiBoxEmitter*)emitter)->SetWidth(12);
        ((DiBoxEmitter*)emitter)->SetHeight(12);
        ((DiBoxEmitter*)emitter)->SetDepth(12);
        emitter->position = DiVec3(100, 240, -60);

        auto rt = DI_NEW DiAttributeFixed();
        rt->SetValue(15);
        emitter->SetDynEmissionRate(rt);

        auto angle = DI_NEW DiAttributeFixed();
        angle->SetValue(0);
        emitter->SetDynAngle(angle);

        auto spd = DI_NEW DiAttributeFixed();
        spd->SetValue(60);
        emitter->SetDynVelocity(spd);

        auto ttl = DI_NEW DiAttributeRandom();
        ttl->SetMinMax(2.5f, 3.5f);
        emitter->SetDynTotalTimeToLive(ttl);

        DiColorController* colorCtrl = (DiColorController*)element->CreateController("Color");
        colorCtrl->AddColour(0, DiColor(0.6f, 0.5f, 0.4f, 0.0f));
        colorCtrl->AddColour(0.5f, DiColor(0.3f, 0.25f, 0.2f, 0.4f));
        colorCtrl->AddColour(1, DiColor(0, 0, 0, 0));

        DiTextureRotatorController* texrotCtrl = (DiTextureRotatorController*)element->CreateController("TextureRotator");
        texrotCtrl->SetUseOwnRotationSpeed(true);
        auto rotspeed = DI_NEW DiAttributeRandom();
        rotspeed->SetMinMax(-100, 100);
        texrotCtrl->SetRotationSpeed(rotspeed);
        auto rot = DI_NEW DiAttributeRandom();
        rot->SetMinMax(0, 300);
        texrotCtrl->SetRotation(rot);

        DiScaleController* scaleCtrl = (DiScaleController*)element->CreateController("Scale");
        scaleCtrl->SetScale(DiVec2(1, 1.5), DiVec2(4, 8));
        scaleCtrl->SetSinceStartSystem(true);
    }

    DiFxTokensParser parser;
    parser.WriteSystem(_ps, "D:/Demi3D_release/ps2.xml");
#else
    DiFxTokensParser parser;
    auto pss = parser.LoadEffects("ps2.effect");
    auto _ps = pss[0];
    g_fxs.push_back(_ps);
    std::shared_ptr<DiTransformUnit> ps(_ps);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->AttachObject(ps);
    _ps->Start();
#endif
}

void InitFx_Repeater01()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    // model
    DiCullNode* nd1 = sm->GetRootNode()->CreateChild();
    auto k2md = make_shared<DiK2Model>("buildings/legion/repeater1/model.mdf");
    nd1->AttachObject(k2md);

#if 0
    // effect
    auto _ps = DiEffectManager::GetInstance().CreateParticleSystemTemplate("Fx_repeater1");
    g_fxs.push_back(_ps);
    std::shared_ptr<DiTransformUnit> ps(_ps);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->AttachObject(ps);
    _ps->Start();
    {
        DiParticleElement* element = _ps->CreateElement();
        element->SetRenderer("Billboard");
        auto emitter = element->CreateEmitter("Box");

        auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR | SHADER_FLAG_USE_MAP);
        mat->GetShaderParameter()->WriteTexture2D("map", "glow_01.dds");
        mat->SetBlendMode(BLEND_ADD);
        mat->SetDepthWrite(false);
        element->SetMaterialName(mat->GetName());

        ((DiBoxEmitter*)emitter)->SetWidth(50);
        ((DiBoxEmitter*)emitter)->SetHeight(70);
        ((DiBoxEmitter*)emitter)->SetDepth(50);
        emitter->position = DiVec3(0, 165, 5);

        auto rt = DI_NEW DiAttributeFixed();
        rt->SetValue(30);
        emitter->SetDynEmissionRate(rt);

        auto spd = DI_NEW DiAttributeFixed();
        spd->SetValue(20);
        emitter->SetDynVelocity(spd);

        auto ttl = DI_NEW DiAttributeRandom();
        ttl->SetMinMax(1, 2);
        emitter->SetDynTotalTimeToLive(ttl);

        auto sz = DI_NEW DiAttributeRandom();
        sz->SetMinMax(20, 40);
        emitter->SetDynParticleAllDimensions(sz);

        DiColorController* colorCtrl = (DiColorController*)element->CreateController("Color");
        colorCtrl->AddColour(0, DiColor::Black);
        colorCtrl->AddColour(0.5f, DiColor(0.25f, 1, 0.5f));
        colorCtrl->AddColour(1, DiColor::Black);
    }
    {
        DiParticleElement* element = _ps->CreateElement();
        element->SetRenderer("Billboard");
        auto emitter = element->CreateEmitter("Box");

        auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR | SHADER_FLAG_USE_MAP);
        auto texture = mat->GetShaderParameter()->WriteTexture2D("map", "mysticenergy2.dds");
        mat->SetBlendMode(BLEND_ADD);
        mat->SetDepthWrite(false);
        element->SetMaterialName(mat->GetName());
        texture->SetAddressing(AM_CLAMP);

        ((DiBoxEmitter*)emitter)->SetWidth(50);
        ((DiBoxEmitter*)emitter)->SetHeight(70);
        ((DiBoxEmitter*)emitter)->SetDepth(50);
        emitter->position = DiVec3(0, 165, 5);

        auto rt = DI_NEW DiAttributeFixed();
        rt->SetValue(6);
        emitter->SetDynEmissionRate(rt);

        auto spd = DI_NEW DiAttributeFixed();
        spd->SetValue(20);
        emitter->SetDynVelocity(spd);

        auto ttl = DI_NEW DiAttributeRandom();
        ttl->SetMinMax(1, 2);
        emitter->SetDynTotalTimeToLive(ttl);

        auto sz = DI_NEW DiAttributeRandom();
        sz->SetMinMax(20, 40);
        emitter->SetDynParticleAllDimensions(sz);

        DiColorController* colorCtrl = (DiColorController*)element->CreateController("Color");
        colorCtrl->AddColour(0, DiColor::Black);
        colorCtrl->AddColour(0.5f, DiColor(0.25f, 1, 0.5f));
        colorCtrl->AddColour(1, DiColor::Black);

        DiTextureRotatorController* texrotCtrl = (DiTextureRotatorController*)element->CreateController("TextureRotator");
        texrotCtrl->SetUseOwnRotationSpeed(true);
        auto rotspeed = DI_NEW DiAttributeRandom();
        rotspeed->SetMinMax(-100, 100);
        texrotCtrl->SetRotationSpeed(rotspeed);
        auto rot = DI_NEW DiAttributeRandom();
        rot->SetMinMax(0, 300);
        texrotCtrl->SetRotation(rot);
    }

    DiFxTokensParser parser;
    parser.WriteSystem(_ps, "D:/Demi3D_release/ps.xml");
#else
    DiFxTokensParser parser;
    auto pss = parser.LoadEffects("ps1.effect");
    auto _ps = pss[0];
    g_fxs.push_back(_ps);
    std::shared_ptr<DiTransformUnit> ps(_ps);
    DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    cullnode->AttachObject(ps);
    _ps->Start();
#endif
}

void InitScene()
{
    DI_INSTALL_PLUGIN(DiK2);    
    DI_INSTALL_PLUGIN(DiFx);

    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

    sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));
    DiDirLightPtr dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());

    DiBase::Driver->GetMainRenderWindow()->GetRenderBuffer()->SetClearColor(DiColor(0.5f, 0.5f, 0.5f));
	
    InitFx_Repeater01();
    //InitFx_Repeater02();
    //InitFx_Torch();
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi3D Sample - Particle System"));
	app.SetInitCallback(InitScene);
    app.SetUpdateCallback([&](){
        for (auto ps : g_fxs)
            ps->Update(DiBase::Driver->GetDeltaSecond());
        //k2md->UpdateAnimation(DiBase::Driver->GetDeltaSecond());
    });
    app.SetShutdownCallback([&](){
        DI_UNINSTALL_PLUGIN(DiFx);
        DI_UNINSTALL_PLUGIN(DiK2);
    });
	app.Open(argc, argv);

	return 0;
}