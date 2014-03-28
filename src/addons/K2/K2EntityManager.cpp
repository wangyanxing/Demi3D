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
#include "K2GameEntity.h"
#include "K2EntityManager.h"
#include "K2Hero.h"

namespace Demi
{
    DiK2EntityManager::DiK2EntityManager()
        :mHeroId(INVALID_OBJ_ID)
    {
    }

    DiK2EntityManager::~DiK2EntityManager()
    {
    }

    DiK2HeroEntityPtr DiK2EntityManager::CreateHero(K2ObjID id)
    {
        if (mHeroId == id && mHeroEntity)
            return mHeroEntity;

        mHeroEntity = make_shared<DiK2HeroEntity>();
        mHeroEntity->InitComponents();
        mHeroEntity->InitAttribute();
        mHeroEntity->SetID(id);
        mHeroId = id;
        mEntities[id] = mHeroEntity;
        mHeroEntity->Init(GAME_HERO);
        return mHeroEntity;
    }

    void DiK2EntityManager::Update(float dt)
    {
        auto itEnd = mEntities.end();
        for (auto it = mEntities.begin(); it != itEnd; it++)
        {
            auto entity = it->second;
            entity->Update(dt);
        }
    }

    Demi::K2ObjID K2AutoID::sID = 0;

}