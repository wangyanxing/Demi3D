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
#include "ArenaEntityManager.h"
#include "ArenaEntityConfig.h"
#include "K2RenderObjects.h"
#include "EffectManager.h"
#include "ParticleSystem.h"

namespace Demi
{
    ArFxProperty::ArFxProperty()
    {

    }

    ArFxProperty::~ArFxProperty()
    {

    }

    void ArFxProperty::Update(float dt)
    {
        for (auto& pj : mProjectiles)
        {
            auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(pj->targetID);
            if(entityPtr)
            {
                if(pj->trailFx->GetState() == DiParticleSystem::PSS_STOPPED)
                {
                    continue;
                }
                auto targetPos = entityPtr->GetImpactPosition();
                
                //reach
                if(DiMath::Abs(pj->curPos.distance(targetPos)) < 2)
                {
                    pj->trailFx->Stop();
                    pj->node->DetachObject(pj->trailFx);
                    continue;
                }
                
                auto dir = (targetPos - pj->curPos).normalisedCopy();
                pj->curPos += dir * (dt * pj->speed);
                pj->node->SetPosition(pj->curPos);
            }
        }
    }
    
    DiParticleSystemPtr ArFxProperty::PlayParticleSystem(const DiString& templateName)
    {
        DiString name;
        name.Format("EtFx_%d_%d", mEntity->GetID(), mFxCount++);
        
        DiParticleSystemPtr ret = DiEffectManager::GetInstance().CreateParticleSystem(name, templateName);
        DiCullNode* node = Driver->GetSceneManager()->GetRootNode()->CreateChild();
        node->AttachObject(ret);
        ret->AttachModel(mEntity->GetRenderObj()->GetModel());
        ret->AddParticleSystemListener(this);
        ret->Start();

        mEffects.insert(ret);
        return ret;
    }
    
    void ArFxProperty::PlayProjectile(uint32 entityID)
    {
        auto entity = GetEntity<ArDynEntity>();
        auto entityConfig = entity->GetAttribute()->GetEntityConfig();
        if(entityConfig->attackprojectile.empty())
        {
            return;
        }
        
        DiString pjName = entityConfig->attackprojectile[0];
        ArFxProjectileConfig* pjConfig = entityConfig->GetProjectile(pjName);
        if(!pjConfig)
        {
            DI_WARNING("Failed to get the projectile config");
            return;
        }
        
        auto pos = entity->GetRenderObj()->GetWorldPosition();
        auto rot = entity->GetRenderObj()->GetNode()->GetDerivedOrientation();
        
        DiString fxName = entityConfig->path + "projectile/" + pjConfig->traileffect;
        DiString impactFxName = entityConfig->path + "projectile/" + pjConfig->impacteffect;
        
        DiString name;
        name.Format("EtFx_%d_%d", mEntity->GetID(), mFxCount++);
        
        DiParticleSystemPtr trailFx = DiEffectManager::GetInstancePtr()->CreateParticleSystem(name, fxName);
        DiCullNode* node = Driver->GetSceneManager()->GetRootNode()->CreateChild();
        node->AttachObject(trailFx);
        node->SetPosition(pos);
        trailFx->AttachModel(mEntity->GetRenderObj()->GetModel());
        trailFx->AddParticleSystemListener(this);
        trailFx->Start();
        
        Projectile* pj = DI_NEW Projectile();
        pj->speed = pjConfig->speed;
        pj->curPos = pos + rot * (entityConfig->attackoffset * entityConfig->GetMaxPreGlobalScale());
        pj->trailFx = trailFx;
        pj->impactFx = nullptr;
        pj->node = node;
        pj->targetID = entityID;
        node->SetPosition(pj->curPos);
        
        mProjectiles.push_back(pj);
    }

    void ArFxProperty::Init()
    {
        auto entity = GetEntity<ArDynEntity>();
        auto entityConfig = entity->GetAttribute()->GetEntityConfig();
        if(entityConfig)
        {
            //mProjectiles = entityConfig->attackprojectile;
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
                DiParticleSystemPtr ps = *it;
                auto node = ps->GetParentCullNode();
                Driver->GetSceneManager()->DestroyNode(node);
                
                mEffects.erase(it);
            }
        }
    }
}