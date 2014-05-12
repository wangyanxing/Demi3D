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
        mType = ENUM_AI_TYPE_MOVETOTARGET;

        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        mTargetEntity = std::dynamic_pointer_cast<ArNPCEntity>(entityPtr);

        if (!mTargetEntity)
        {
            DI_WARNING("Cannot lotate the NPC target[id=%d]", mTargetID);
        }
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

        DiK2Pos posTarget = mTargetEntity->GetRenderObj()->GetPosition();
        if (mLastPos.x == INVALID_FLOAT_VALUE)
            mLastPos = posTarget;

        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        auto walkMode = mEntity->GetMoveProperty()->GetWalkMode();

        float radiusTarget = mTargetEntity->GetRenderObj()->GetRadius();
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
        if (!mTargetEntity)
        {
            Stop();
            EndCommand();
            return;
        }
        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        DiK2Pos target = mTargetEntity->GetRenderObj()->GetPosition();
        mEntity->GetMoveProperty()->MoveTo(source, target, mRange);
    }

    bool ArAIFollowCommand::CheckDistance()
    {
        if (!mTargetEntity)
        {
            Stop();
            EndCommand();
            return false;
        }

        float radiusTarget = mTargetEntity->GetRenderObj()->GetRadius();
        float radiusSource = mEntity->GetRenderObj()->GetRadius();

        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        DiK2Pos target = mTargetEntity->GetRenderObj()->GetPosition();
        if (source.Distance(target) <= mRange + radiusTarget + radiusSource)
            return false;

        return true;
    }
}