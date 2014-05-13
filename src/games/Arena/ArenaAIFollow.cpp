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
#include "ArenaAIFollow.h"
#include "ArenaNPCEntity.h"
#include "K2RenderObjects.h"
#include "ArenaEntityManager.h"
#include "ArenaGameApp.h"

#define AI_MOVETOTARGET_MIN_TIME         0.5 
#define AI_MOVETOTARGET_MIN_DISTANCE     1.0f

namespace Demi
{
    ArAIFollowCommand::ArAIFollowCommand(ArEntity* entity, ArObjID target, float range)
        : ArAICommand(entity)
        , mTargetID(target)
        , mRange(range)
    {
        mType = ENUM_AI_TYPE_FOLLOWTOTARGET;

    }

    ArAIFollowCommand::~ArAIFollowCommand()
    {

    }

    void ArAIFollowCommand::Enter()
    {
        if (CheckDistance())
            Move();
    }

    void ArAIFollowCommand::Leave()
    {
    }

    void ArAIFollowCommand::Update(float dt)
    {
        mElapse += dt;

        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        auto targetEntity = std::dynamic_pointer_cast<ArNPCEntity>(entityPtr);
        if (!targetEntity)
        {
            DI_WARNING("Cannot lotate the NPC target[id=%d]", mTargetID);
        }

        DiK2Pos posTarget = targetEntity->GetRenderObj()->GetPosition();
        if (mLastPos.x == INVALID_FLOAT_VALUE)
            mLastPos = posTarget;

        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        auto walkMode = mEntity->GetMoveProperty()->GetWalkMode();

        float radiusTarget = targetEntity->GetRenderObj()->GetRadius();
        float radiusSource = mEntity->GetRenderObj()->GetRadius();

        float distance = source.Distance(posTarget);
        if (distance <= radiusTarget + radiusSource + mRange)
        {
            Stop();
            return;
        }

        if (walkMode > ENUM_WALK_MODE_STOP)
        {
            Move();
            mElapse = 0;
            return;
        }

        if (mElapse < AI_MOVETOTARGET_MIN_TIME)
        {
            return;
        }
        
        // the target's moving
        if (mLastPos.Distance(posTarget) > AI_MOVETOTARGET_MIN_DISTANCE)
        {
            Move();
            mLastPos = posTarget;
            mElapse = 0;
            return;
        }
    }

    void ArAIFollowCommand::Redo()
    {
        Move();
    }

    void ArAIFollowCommand::Stop()
    {
        mEntity->GetMoveProperty()->Stop();
    }

    void ArAIFollowCommand::Move()
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        auto targetEntity = std::dynamic_pointer_cast<ArNPCEntity>(entityPtr);
        if (!targetEntity)
        {
            Stop();
            EndCommand();
            return;
        }
        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        DiK2Pos target = targetEntity->GetRenderObj()->GetPosition();
        mEntity->GetMoveProperty()->MoveTo(source, target, mRange);
    }

    bool ArAIFollowCommand::CheckDistance()
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        auto targetEntity = std::dynamic_pointer_cast<ArNPCEntity>(entityPtr);
        if (!targetEntity)
        {
            Stop();
            EndCommand();
            return false;
        }

        float radiusTarget = targetEntity->GetRenderObj()->GetRadius();
        float radiusSource = mEntity->GetRenderObj()->GetRadius();

        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        DiK2Pos target = targetEntity->GetRenderObj()->GetPosition();
        if (source.Distance(target) <= mRange + radiusTarget + radiusSource)
            return false;

        return true;
    }
}