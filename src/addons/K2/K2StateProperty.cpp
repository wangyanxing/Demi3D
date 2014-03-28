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

#include "K2Pch.h"
#include "K2StateProperty.h"
#include "K2MoveProperty.h"
#include "K2TerrainMap.h"
#include "K2World.h"
#include "K2GameApp.h"
#include "K2GameEntity.h"
#include "K2RenderObjects.h"

namespace Demi
{
    DiK2StateProperty::DiK2StateProperty()
    {

    }

    DiK2StateProperty::~DiK2StateProperty()
    {

    }

    void DiK2StateProperty::Update(float dt)
    {

    }

    void DiK2StateProperty::ModalityChange(K2ModalityType mod)
    {

    }

    void DiK2StateProperty::AddState(K2CharState stateType, K2StateID id, double life)
    {
        auto i = mStates.find(id);
        if (i == mStates.end())
        {
            K2StateInfo state;
            state.active = true;
            state.life = life;
            state.lifeCycle = life;
            state.stateID = id;
            state.stateType = stateType;
            state.timeStamp = DiBase::Driver->GetElapsedSecond();

            mStates[id] = state;
        }
    }

    void DiK2StateProperty::RemoveState(K2StateID id)
    {
        auto i = mStates.find(id);
        if (i != mStates.end())
        {
            mStates.erase(i);
        }
    }

}