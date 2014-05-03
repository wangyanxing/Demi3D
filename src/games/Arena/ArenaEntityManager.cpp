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
#include "ArenaGameEntity.h"
#include "ArenaEntityManager.h"
#include "ArenaHero.h"

namespace Demi
{
    ArEntityManager::ArEntityManager()
        :mHeroId(INVALID_OBJ_ID)
    {
    }

    ArEntityManager::~ArEntityManager()
    {
    }

    ArHeroEntityPtr ArEntityManager::CreateHero(ArObjID id)
    {
        if (mHeroId == id && mHeroEntity)
            return mHeroEntity;

        mHeroEntity = make_shared<ArHeroEntity>();
        mHeroEntity->InitComponents();
        mHeroEntity->InitAttribute();
        mHeroEntity->SetID(id);
        mHeroId = id;
        mEntities[id] = mHeroEntity;
        mHeroEntity->Init(GAME_HERO);
        return mHeroEntity;
    }

    void ArEntityManager::Update(float dt)
    {
        auto itEnd = mEntities.end();
        for (auto it = mEntities.begin(); it != itEnd; it++)
        {
            auto entity = it->second;
            entity->Update(dt);
        }
    }

    Demi::ArObjID ArAutoID::sID = 0;
}