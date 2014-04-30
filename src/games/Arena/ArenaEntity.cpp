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
#include "ArenaEntity.h"
#include "ArenaProperty.h"

namespace Demi
{
    ArEntity::ArEntity()
        :mID(INVALID_OBJ_ID)
    {

    }

    ArEntity::~ArEntity()
    {
        auto it = mPropertyMap.begin();
        auto itEnd = mPropertyMap.end();
        while (it != itEnd)
        {
            it->second->Deactivate();
            DI_DELETE(it->second);
            it++;
        }
        mPropertyMap.clear();
    }

    void ArEntity::Update(float dt)
    {
        auto it = mPropertyMap.begin();
        auto itEnd = mPropertyMap.end();
        while (it != itEnd)
        {
            it->second->Update(dt);
            it++;
        }
    }
}