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
#include "ArenaStateProperty.h"
#include "ArenaMoveProperty.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "ArenaGameApp.h"
#include "ArenaGameEntity.h"
#include "K2RenderObjects.h"

namespace Demi
{
    ArStateProperty::ArStateProperty()
    {

    }

    ArStateProperty::~ArStateProperty()
    {

    }

    void ArStateProperty::Update(float dt)
    {

    }

    void ArStateProperty::ModalityChange(ArModalityType mod)
    {

    }

    void ArStateProperty::AddState(ArCharState stateType, ArStateID id, double life)
    {
        auto i = mStates.find(id);
        if (i == mStates.end())
        {
            ArStateInfo state;
            state.active = true;
            state.life = life;
            state.lifeCycle = life;
            state.stateID = id;
            state.stateType = stateType;
            state.timeStamp = DiBase::Driver->GetElapsedSecond();

            mStates[id] = state;
        }
    }

    void ArStateProperty::RemoveState(ArStateID id)
    {
        auto i = mStates.find(id);
        if (i != mStates.end())
        {
            mStates.erase(i);
        }
    }

}