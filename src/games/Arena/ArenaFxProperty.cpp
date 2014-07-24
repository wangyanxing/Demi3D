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
#include "ParticleElement.h"
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
        for(auto iter = mProjectiles.begin(); iter != mProjectiles.end(); )
        {
            auto& pj = *iter;
            if(pj->trailFx->GetState() == DiParticleSystem::PSS_STOPPED)
            {
                iter++;
                continue;
            }
            
            // update the target position
            auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(pj->targetID);
            if(entityPtr)
            {
                pj->curTarget = entityPtr->GetImpactPosition();
            }
            
            // update the motion
            auto dir = (pj->curTarget - pj->curPos).normalisedCopy();
            pj->curPos += dir * (dt * pj->speed);
            pj->node->SetPosition(pj->curPos);
            size_t numElements = pj->trailFx->GetNumElements();
            for (size_t i = 0; i < numElements; ++i)
            {
                pj->trailFx->GetElement(i)->direction = -dir;
            }
            
            // update the effect
            pj->trailFx->Update(dt);
            
            //reach or not
            if(DiMath::Abs(pj->curPos.distance(pj->curTarget)) < 8)
            {
                pj->trailFx->Stop();
                pj->node->DetachObject(pj->trailFx);
                DiEffectManager::GetInstance().DestroyParticleSystem(pj->trailFx);

                if(entityPtr && pj->impactFx)
                {
                    entityPtr->GetRenderObj()->GetNode()->AttachObject(pj->impactFx);
                    pj->impactFx->AttachModel(entityPtr->GetRenderObj()->GetModel());
                    pj->impactFx->Start();
                }
                
                DI_DELETE pj;
                iter = mProjectiles.erase(iter);
            }
            else
            {
                ++iter;
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

        mEffects.insert(ret.get());
        return ret;
    }
    
    void ArFxProperty::PlayProjectile(uint32 entityID)
    {
        if(!mCurrentConfig)
        {
            Init();
        }
        
        auto entity = GetEntity<ArDynEntity>();
        auto entityConfig = entity->GetAttribute()->GetEntityConfig();
        
        auto pos = entity->GetRenderObj()->GetWorldPosition();
        auto rot = entity->GetRenderObj()->GetNode()->GetDerivedOrientation();
        
        DiString name;
        name.Format("EtPj_%d_%d", mEntity->GetID(), mFxCount++);
        DiString impactname;
        impactname.Format("EtImp_%d_%d", mEntity->GetID(), mFxCount++);
        
        DiParticleSystemPtr trailFx = DiEffectManager::GetInstancePtr()->CreateParticleSystem(name, mTrailFxName);
        DiCullNode* node = Driver->GetSceneManager()->GetRootNode()->CreateChild();
        node->AttachObject(trailFx);
        node->SetPosition(pos);
        trailFx->AttachModel(mEntity->GetRenderObj()->GetModel());
        trailFx->Start();
        
        DiParticleSystemPtr impactFx = DiEffectManager::GetInstancePtr()->CreateParticleSystem(impactname, mImpactFxName);
        impactFx->AddParticleSystemListener(this);
        
        Projectile* pj = DI_NEW Projectile();
        pj->speed      = mCurrentConfig->speed;
        pj->curPos     = pos + rot * (entityConfig->attackoffset * entityConfig->GetMaxPreGlobalScale());
        pj->trailFx    = trailFx;
        pj->impactFx   = impactFx;
        pj->node       = node;
        pj->targetID   = entityID;
        
        node->SetPosition(pj->curPos);
        
        mProjectiles.push_back(pj);
    }

    void ArFxProperty::Init()
    {
        auto entity = GetEntity<ArDynEntity>();
        auto entityConfig = entity->GetAttribute()->GetEntityConfig();
        if(!entityConfig)
        {
            DI_WARNING("No valid entity config!");
            return;
        }
        
        DiString pjName = entityConfig->attackprojectile[0];
        mCurrentConfig = entityConfig->GetProjectile(pjName);
        if(!mCurrentConfig)
        {
            DI_WARNING("Failed to get the projectile config");
            return;
        }
        
        mTrailFxName = entityConfig->path + "projectile/" + mCurrentConfig->traileffect;
        mImpactFxName = entityConfig->path + "projectile/" + mCurrentConfig->impacteffect;
    }
    
    void ArFxProperty::HandleParticleSystemEvent(DiParticleSystem* particleSystem, DiFxEvent& event)
    {
        if (event.eventType == PU_EVT_SYSTEM_STOPPED)
        {
            auto node = particleSystem->GetParentCullNode();
            node->DetachObject(particleSystem);
            
            DiEffectManager::GetInstance().DestroyParticleSystem(particleSystem->GetName());
        }
    }
}