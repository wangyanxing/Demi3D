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

#include "ArenaPch.h"
#include "ArenaFxProperty.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "ArenaGameApp.h"
#include "ArenaGameEntity.h"
#include "ArenaDynEntity.h"
#include "ArenaAttribute.h"
#include "K2RenderObjects.h"
#include "EffectManager.h"
#include "ParticleSystem.h"

namespace Demi
{
    void ArFxProjectileConfig::Load(const DiXMLElement& node)
    {
        if (!node.CheckName("projectile"))
        {
            DI_WARNING("Bad projectile config file!");
            return;
        }
        
        name = node.GetAttribute("name");
        speed = node.GetFloat("speed");
        gravity = node.GetFloat("gravity");
        modelscale = node.GetFloat("modelscale");
        model = node.GetAttribute("model");
        traileffect = node.GetAttribute("traileffect");
        impacteffect = node.GetAttribute("impacteffect");
    }
    
    ArFxProperty::ArFxProperty()
    {

    }

    ArFxProperty::~ArFxProperty()
    {

    }

    void ArFxProperty::Update(float dt)
    {
        
    }
    
    DiParticleSystemPtr ArFxProperty::PlayParticleSystem(const DiString& templateName)
    {
        DiString name;
        name.Format("EtFx_%d_%d", mEntity->GetID(), mFxCount++);
        
        DiParticleSystemPtr ret = DiEffectManager::GetInstance().CreateParticleSystem(name, templateName);
        mEntity->GetRenderObj()->GetNode()->AttachObject(ret);
        ret->AttachModel(mEntity->GetRenderObj()->GetModel());
        ret->AddParticleSystemListener(this);
        ret->Start();

        mEffects.insert(ret);
        return ret;
    }
    
    void ArFxProperty::PlayProjectile(uint32 entityID, const DiString& bone)
    {
    }

    void ArFxProperty::Init()
    {
        auto entity = GetEntity<ArDynEntity>();
        auto entityConfig = entity->GetAttribute()->GetEntityConfig();
        if(entityConfig)
        {
            mProjectiles = entityConfig->attackprojectile;
        }
        else
        {
            DI_WARNING("No valid entity config!");
        }
    }
    
    void ArFxProperty::HandleParticleSystemEvent(DiParticleSystemPtr particleSystem, DiFxEvent& event)
    {
        if (event.eventType == PU_EVT_SYSTEM_STOPPED)
        {
            auto it = mEffects.find(particleSystem);
            if (it != mEffects.end())
            {
                mEffects.erase(it);
            }
        }
    }
}