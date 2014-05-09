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
#include "ArenaAIMove.h"
#include "ArenaNPCEntity.h"
#include "K2RenderObjects.h"

#define AI_MOVE_THRESHOLD 0.1f

namespace Demi
{
    ArAIMoveCommand::ArAIMoveCommand(ArEntity* entity, const DiK2Pos& pos, float range)
        :ArAICommand(entity)
        , mPos(pos)
        , mRange(range)
    {
        mType = ENUM_AI_TYPE_MOVETOPOINT;
    }

    ArAIMoveCommand::~ArAIMoveCommand()
    {

    }

    void ArAIMoveCommand::Enter()
    {
        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        if (mRange > AI_MOVE_THRESHOLD && source.Distance(mPos) <= mRange + AI_MOVE_THRESHOLD)
        {
            EndCommand();
        }
        else
        {
            Move();
        }
    }

    void ArAIMoveCommand::Leave()
    {

    }

    void ArAIMoveCommand::Update(float dt)
    {
        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        if (source.Distance(mPos) <= mRange + AI_MOVE_THRESHOLD)
        {
            EndCommand();
        }
        else
        {
            auto walkMode = mEntity->GetMoveProperty()->GetWalkMode();
            if (walkMode == ENUM_WALK_MODE_STOP)
            {
                Move();
            }
        }
    }

    void ArAIMoveCommand::Redo()
    {
        Move();
    }

    void ArAIMoveCommand::Stop()
    {
        mEntity->GetMoveProperty()->Stop();
    }

    void ArAIMoveCommand::Move()
    {
        DiK2Pos source = mEntity->GetRenderObj()->GetPosition();
        mEntity->GetMoveProperty()->MoveTo(source, mPos, mRange);
    }

}