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
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        
        if (entityPtr && CheckDistance())
        {
            entityPtr->AddFollower(mEntity->GetID(), mEntity->GetRenderObj()->GetPosition());
            Move(entityPtr);
        }
    }

    void ArAIFollowCommand::Leave()
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        if(entityPtr)
        {
            entityPtr->RemoveFollower(mEntity->GetID());
        }
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

        DiK2Pos posTarget = targetEntity->GetFollowTarget(mEntity->GetID(), 0);
        
        if (mLastPos.x == INVALID_FLOAT_VALUE)
            mLastPos = posTarget;

        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        auto walkMode = mEntity->GetMoveProperty()->GetWalkMode();

        float distance = source.Distance(posTarget);
        if (distance <= mRange)
        {
            Stop();
            return;
        }

        if (walkMode > ENUM_WALK_MODE_STOP)
        {
            Move(entityPtr);
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
            Move(entityPtr);
            mLastPos = posTarget;
            mElapse = 0;
            return;
        }
    }

    void ArAIFollowCommand::Redo()
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTargetID);
        Move(entityPtr);
    }

    void ArAIFollowCommand::Stop()
    {
        mEntity->GetMoveProperty()->Stop();
    }

    void ArAIFollowCommand::Move(ArGameEntityPtr targetPtr)
    {
        auto targetEntity = std::dynamic_pointer_cast<ArNPCEntity>(targetPtr);
        if (!targetEntity)
        {
            Stop();
            EndCommand();
            return;
        }
        
        DiK2Pos posTarget = targetEntity->GetFollowTarget(mEntity->GetID(), mRange);
        
        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        mEntity->GetMoveProperty()->MoveTo(source, posTarget, 0);
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

        DiK2Pos posTarget = targetEntity->GetFollowTarget(mEntity->GetID(), mRange);

        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        if (source.Distance(posTarget) <= 0)
            return false;

        return true;
    }
}