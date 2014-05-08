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

namespace Demi
{
    ArAIProperty::ArAIProperty()
        : mLastMoveTime(0)
    {

    }

    ArAIProperty::~ArAIProperty()
    {

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

}