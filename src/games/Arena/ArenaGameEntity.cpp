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
#include "ArenaGameEntity.h"
#include "K2World.h"
#include "ArenaGameApp.h"
#include "ArenaGame.h"
#include "K2RenderObjects.h"
#include "ArenaAttribute.h"
#include "ArenaEntityManager.h"


#include "DebugHelper.h"

namespace Demi
{
    ArGameEntity::ArGameEntity()
    {
#if 0
        mDebugger = make_shared<DiDebugHelper>();
        DiMaterialPtr helpermat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        helpermat->SetDepthCheck(false);
        mDebugger->SetMaterial(helpermat);
        DiBase::Driver->GetSceneManager()->GetRootNode()->AttachObject(mDebugger);
#endif
    }

    ArGameEntity::~ArGameEntity()
    {
        SAFE_DELETE(mAttribute);
    }

    void ArGameEntity::SetModel(const DiString& model)
    {
        mModel = model;
        mRenderObj->LoadModel(mModel);
    }

    void ArGameEntity::Init(K2ObjTypes type)
    {
        mType = type;
        auto world = ArGameApp::Get()->GetWorld();
        mRenderObj = world->CreateRenderObject(type);
    }

    void ArGameEntity::Update(float dt)
    {
        ArEntity::Update(dt);

#if 0
        mDebugger->Clear();
        for (int i = 0; i < DEFAULT_FOLLOWER_NUM; ++i)
        {
            DiRadian angle((DiMath::TWO_PI / DEFAULT_FOLLOWER_NUM) * i);

            float radius = 3;
            DiK2Pos offset(DiMath::Sin(angle) * radius, DiMath::Cos(angle) * radius);
            DiK2Pos ret = offset + mRenderObj->GetPosition();
            mDebugger->AddBoundingBox(ret.ToWorldPos(), 3, mFollowers[i] == INVALID_OBJ_ID ? DiColor::Yellow : DiColor::White);
        }
#endif
    }

    bool ArGameEntity::IsDead()
    {
        return false;
    }
    
    DiK2Pos ArGameEntity::GetFollowTarget(ArObjID follower, float radius)
    {
        DiK2Pos ret;
        auto it = std::find_if(mFollowers.begin(), mFollowers.end(),
                               [follower] (ArObjID id) { return id == follower; } );

        auto renderObj = GetRenderObj();
        DI_ASSERT(renderObj);
        
        if(it == mFollowers.end())
        {
            return renderObj->GetPosition();
        }
        
        int numSlots = (int)mFollowers.size();
        int id = int(it - mFollowers.begin());
        DiRadian angle((DiMath::TWO_PI / numSlots)*id);
        
        DiK2Pos offset(DiMath::Sin(angle) * radius, DiMath::Cos(angle) * radius);
        ret = offset + renderObj->GetPosition();
        
        return ret;
    }
    
    void ArGameEntity::AddFollower(ArObjID follower, const DiK2Pos& pos)
    {
        // check if the follower vector is full or not
        auto emptySlotIt = std::find_if(mFollowers.begin(), mFollowers.end(),
                                        [](const ArObjID& v) { return INVALID_OBJ_ID == v; });
        if (emptySlotIt == mFollowers.end())
        {
            mFollowers.emplace_back(INVALID_OBJ_ID);
        }
        
        float minDist = 0;
        int minID = -1;
        auto renderObj = GetRenderObj();
        DI_ASSERT(renderObj);
        
        int numSlots = (int)mFollowers.size();
        for (int i = 0; i < numSlots; ++i)
        {
            if (mFollowers[i] == follower)
            {
                DI_WARNING("The follower[%d] has already existed", follower);
                return;
            }
            
            if(mFollowers[i] != INVALID_OBJ_ID)
                continue;
            
            DiRadian angle((DiMath::TWO_PI / numSlots)*i);
            DiK2Pos offset(DiMath::Sin(angle) * 5, DiMath::Cos(angle) * 5);
            DiK2Pos ret = offset + renderObj->GetPosition();
            float dist = pos.SquareDistance(ret);
            if(minID < 0)
            {
                minDist = dist;
                minID = i;
            }
            else
            {
                if(dist < minDist)
                {
                    minDist = dist;
                    minID = i;
                }
            }
        }
        
        DI_ASSERT(minID >= 0);
        mFollowers[minID] = follower;
    }
    
    void ArGameEntity::RemoveFollower(ArObjID follower)
    {
        for (auto i = mFollowers.begin(); i != mFollowers.end(); ++i)
        {
            if(*i == follower)
            {
                *i = INVALID_OBJ_ID;
                return;
            }
        }
        DI_WARNING("Failed to remove: cannot loacate the follower[%d];", follower);
    }

    bool ArGameEntity::CheckDistance(ArGameEntity* entity, float distance)
    {
        if (!entity || !entity->GetRenderObj() | !GetRenderObj())
            return false;

        DiK2Pos source = GetRenderObj()->GetPosition();
        DiK2Pos target = entity->GetRenderObj()->GetPosition();
        float radiusTarget = entity->GetRenderObj()->GetRadius();
        float radiusSource = GetRenderObj()->GetRadius();

        if (source.Distance(target) <= distance + radiusSource + radiusTarget)
            return true;
        return false;
    }
    
    DiVec3 ArGameEntity::GetImpactPosition()
    {
        auto renderObj = GetRenderObj();
        float scale = GetAttribute()->GetEntityConfig()->GetMaxPreGlobalScale();
        auto offset = GetAttribute()->GetEntityConfig()->targetoffset;
        
        return renderObj->GetWorldPosition() + renderObj->GetRotQuat()*(scale * offset);
    }

    bool ArGameEntity::CheckDistance(ArObjID target, float distance)
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(target);
        return CheckDistance(entityPtr.get(), distance);
    }

    bool ArGameEntity::CheckAbsDistance(ArGameEntity* entity, float distance)
    {
        if (!entity || !entity->GetRenderObj() | !GetRenderObj())
            return false;

        DiK2Pos source = GetRenderObj()->GetPosition();
        DiK2Pos target = entity->GetRenderObj()->GetPosition();

        if (source.Distance(target) <= distance)
            return true;
        return false;
    }

    bool ArGameEntity::CheckAbsDistance(ArObjID target, float distance)
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(target);
        return CheckAbsDistance(entityPtr.get(), distance);
    }
}