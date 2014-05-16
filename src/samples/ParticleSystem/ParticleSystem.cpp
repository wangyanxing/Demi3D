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
#include "ParticleSystem.h"
#include "ParticleElement.h"

DiParticleSystem* _ps = nullptr;

void AddMeshes()
{
    DiEffectManager* effectMgr = DI_NEW DiEffectManager();
    
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();
	   
	DiCullNode* cullnode = sm->GetRootNode()->CreateChild();
    DiBillboardSetPtr bs = make_shared<DiBillboardSet>();
    
    auto mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
    bs->SetMaterial(mat);
    
    bs->CreateBillboard(0,0,0);
    bs->CreateBillboard(0,10,0);
    
    _ps = effectMgr->CreateParticleSystemTemplate("test");
    std::shared_ptr<DiTransformUnit> ps(_ps);
    cullnode->AttachObject(ps);
    _ps->Start();
    
    DiParticleElement* element = _ps->CreateElement();
    element->SetRenderer("Billboard");
    element->CreateEmitter("Point");
    element->SetMaterialName(mat->GetName());

	//cullnode->AttachObject(bs);
}

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	sm->SetAmbientColor(DiColor(0.3f,0.3f,0.3f));
    
	AddMeshes();
}

void Update()
{
    _ps->Update(DiBase::Driver->GetDeltaSecond());
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi3D Sample - Particle System"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(Update);
	app.Open(argc, argv);

	return 0;
}