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
#include "K2RenderObjects.h"

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

    }

    void ArFxProperty::Init()
    {
        auto entity = GetEntity<ArDynEntity>();
        auto entityConfig = entity->GetAttribute()->GetEntityConfig();
        if(entityConfig)
        {
        }
        else
        {
            DI_WARNING("No valid entity config!");
        }
    }
}