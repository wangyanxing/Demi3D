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
#include "K2World.h"
#include "ArenaGameApp.h"
#include "ArenaGame.h"
#include "K2RenderObjects.h"
#include "ArenaAttribute.h"
#include "ArenaEntityManager.h"

namespace Demi
{

    ArGameEntity::ArGameEntity()
    {
    }

    ArGameEntity::~ArGameEntity()
    {
        SAFE_DELETE(mAttribute);
    }

    void ArGameEntity::SetModel(const DiString& model)
    {
        mModel = model;
        mRenderObj->LoadModel(mModel);
    }

    void ArGameEntity::Init(K2ObjTypes type)
    {
        mType = type;
        auto world = ArGameApp::Get()->GetWorld();
        mRenderObj = world->CreateRenderObject(type);
    }

    void ArGameEntity::Update(float dt)
    {
        ArEntity::Update(dt);

        mRenderObj->Update(dt);
    }

    bool ArGameEntity::IsDead()
    {
        return false;
    }

    bool ArGameEntity::CheckDistance(ArGameEntity* entity, float distance)
    {
        if (!entity || !entity->GetRenderObj() | !GetRenderObj())
            return false;

        DiK2Pos source = GetRenderObj()->GetPosition();
        DiK2Pos target = entity->GetRenderObj()->GetPosition();
        float radiusTarget = entity->GetRenderObj()->GetRadius();
        float radiusSource = GetRenderObj()->GetRadius();

        if (source.Distance(target) <= distance + radiusSource + radiusTarget)
            return true;
        return false;
    }

    bool ArGameEntity::CheckDistance(ArObjID target, float distance)
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(target);
        return CheckDistance(entityPtr.get(), distance);
    }

    bool ArGameEntity::CheckAbsDistance(ArGameEntity* entity, float distance)
    {
        if (!entity || !entity->GetRenderObj() | !GetRenderObj())
            return false;

        DiK2Pos source = GetRenderObj()->GetPosition();
        DiK2Pos target = entity->GetRenderObj()->GetPosition();

        if (source.Distance(target) <= distance)
            return true;
        return false;
    }

    bool ArGameEntity::CheckAbsDistance(ArObjID target, float distance)
    {
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(target);
        return CheckAbsDistance(entityPtr.get(), distance);
    }
}