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
#include "ArenaMotionProperty.h"
#include "ArenaAIProperty.h"
#include "ArenaAICommand.h"
#include "ArenaDynEntity.h"
#include "ArenaNPCEntity.h"
#include "ArenaMoveProperty.h"
#include "ArenaGameApp.h"
#include "ArenaHero.h"
#include "ArenaEntityManager.h"
#include "ArenaAIMove.h"
#include "ArenaAIMoveTarget.h"
#include "ArenaAIFollow.h"
#include "ArenaAIAttack.h"
#include "ArenaEntityConfig.h"
#include "ArenaAttribute.h"
#include "ArenaMoveProperty.h"

// in second
#define DEF_AI_MOVE_REQUEST_MIN_TIME 0.1

namespace Demi
{
    ArAIProperty::ArAIProperty()
        : mLastMoveTime(0)
    {
        mAIRoot = DI_NEW SequentialNode();
    }

    ArAIProperty::~ArAIProperty()
    {
        SAFE_DELETE(mAIRoot);
    }

    void ArAIProperty::ClearAIList()
    {
        if (!mAIList.empty())
        {
            mAIList.front()->Leave();
        }
        for (auto it = mAIList.begin(); it != mAIList.end(); it++)
        {
            DI_DELETE *it;
        }
        mAIList.clear();
    }

    void ArAIProperty::PushCommand(ArAICommand* cmd)
    {
        if (mAIList.empty())
        {
            cmd->Enter();
        }
        mAIList.push_back(cmd);
    }

    void ArAIProperty::PopCommand()
    {
        DI_ASSERT(!mAIList.empty());
        mAIList.front()->Leave();
        DI_DELETE mAIList.front();
        mAIList.erase(mAIList.begin());

        if (!mAIList.empty())
        {
            mAIList.front()->Enter();
        }
    }

    void ArAIProperty::Update(float dt)
    {
        ArProperty::Update(dt);

        // update the behavior tree
        mAIRoot->execute((void*) this);

        auto gameEntity = GetEntity<ArGameEntity>();
        if (gameEntity->IsDead())
        {
            ClearAIList();
        }

        if (mAIList.empty())
        {
            UpdateDummyAI(dt);
            return;
        }

        auto pAI = mAIList.front();
        while (pAI->IsEnd())
        {
            PopCommand();
            if (mAIList.empty())
            {
                return;
            }
            pAI = mAIList.front();
        }
        pAI->Update(dt);
    }

    void ArAIProperty::StopAI(const DiK2Pos& pos)
    {
        GetEntity<ArDynEntity>()->GetMoveProperty()->StopAt(pos);
    }

    void ArAIProperty::ClearAI()
    {
        ClearAIList();
    }

    void ArAIProperty::UpdateDummyAI(float dt)
    {
        auto pNpc = GetEntity<ArNPCEntity>();
        if (!pNpc->IsDead())
        {
//             ObjID_t targetID = pNpc->GetLockTargetID();
//             if (targetID != INVALID_OBJ_ID && targetID != pNpc->getID())
//             {
//                 Ptr<GameEntity> entityTarget = GameEntityManager::getSingleton().findEntity(targetID);
//                 if (entityTarget.isvalid() && entityTarget->GetRenderObject().isvalid())
//                 {
//                     pNpc->GetMoveProperty()->TurnTo(entityTarget->GetPosition());
//                 }
//             }
        }
        else
        {
            //pNpc->GetMoveProperty()->synDirection(INVALID_DIRECTION_VALUE);
        }
    }

    void ArAIProperty::CommandMoveTo(const DiK2Pos& pos, float range /*= 0.0f*/)
    {
        if (ArGameApp::Get()->GetEntityManager()->IsHero(GetEntity<ArGameEntity>()->GetID()))
        {
            double time = Driver->GetElapsedSecond();
            if (time - mLastMoveTime < DEF_AI_MOVE_REQUEST_MIN_TIME)
            {
                return;
            }
            mLastMoveTime = time;
        }
        if (GetEntity<ArGameEntity>()->IsDead())
        {
            return;
        }
        ClearAIList();
        
        auto pCmd = DI_NEW ArAIMoveCommand(mEntity, pos, range);
        PushCommand(pCmd);
    }

    void ArAIProperty::CommandMoveTo(ArObjID targetID, float range /*= 0.0f*/)
    {
        if (!mAIList.empty())
        {
            if (mAIList.front()->GetType() == ENUM_AI_TYPE_MOVETOTARGET)
                return;
        }

        if (ArGameApp::Get()->GetEntityManager()->IsHero(GetEntity<ArGameEntity>()->GetID()))
        {
            double time = Driver->GetElapsedSecond();
            if (time - mLastMoveTime < DEF_AI_MOVE_REQUEST_MIN_TIME)
            {
                return;
            }
            mLastMoveTime = time;
        }
        if (GetEntity<ArGameEntity>()->IsDead())
        {
            return;
        }
        ClearAIList();

        auto pCmd = DI_NEW ArAIMoveToTargetCommand(mEntity, targetID, range);
        PushCommand(pCmd);
    }

    void ArAIProperty::CommandFollowTo(ArObjID targetID, float range /*= 0.0f*/)
    {
        if (!mAIList.empty())
        {
            if (mAIList.front()->GetType() == ENUM_AI_TYPE_FOLLOWTOTARGET)
                return;
        }

        ClearAIList();
        auto pCmd = DI_NEW ArAIFollowCommand(mEntity, targetID, range);
        PushCommand(pCmd);
    }

    void ArAIProperty::CommandAttack(ArObjID targetID)
    {
        if (!mAIList.empty())
        {
            if (mAIList.front()->GetType() == ENUM_AI_TYPE_ATTACK)
                return;
        }

        ClearAIList();
        auto pCmd = DI_NEW ArAIAttackCommand(mEntity, targetID);
        PushCommand(pCmd);
    }

    void ArAIProperty::InitNPCBehaviorTree()
    {
        mAIRoot->addChild(DI_NEW BoolCondition([this](void*){
            
            float sight = DiK2Pos::FromWorldScale(mEntity->GetAttribute()->GetEntityConfig()->sightrangenight);
            if (!mEntity->CheckDistance(ArEntityManager::GetHeroID(), sight))
            {
                // change to idle
                mEntity->GetEntity<ArDynEntity>()->GetMotionProperty()->ModalityChange(MODALITY_STAND);
                return false;
            }
            else
            {
                return true;
            }
        }, true));

        mAIRoot->addChild(DI_NEW BoolCondition([this](void*){
            float range = DiK2Pos::FromWorldScale(mEntity->GetAttribute()->GetEntityConfig()->attackrange*1.3f);
            auto heroEntity = ArGameApp::Get()->GetEntityManager()->GetHero();
            auto thisEntity = dynamic_cast<ArDynEntity*>(mEntity);
            if (!thisEntity)
            {
                return false;
            }

            if (!mEntity->CheckAbsDistance(ArEntityManager::GetHeroID(), range))
            {
                // change to follow
                CommandFollowTo(ArEntityManager::GetHeroID(), range-0.1f);
                return false;
            }
            else
            {
                if (!(heroEntity->GetMoveProperty()->GetWalkMode() == ENUM_WALK_MODE_WALK))
                {
                    // attack
                    CommandAttack(ArEntityManager::GetHeroID());
                }
                return true;
            }
        }, true));
    }
}