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
#include "K2World.h"
#include "K2GameApp.h"
#include "K2Game.h"
#include "K2RenderObjects.h"

namespace Demi
{

    DiK2GameEntity::DiK2GameEntity()
    {

    }

    DiK2GameEntity::~DiK2GameEntity()
    {

    }

    void DiK2GameEntity::SetModel(const DiString& model)
    {
        mModel = model;
        mRenderObj->LoadModel(mModel);
    }

    void DiK2GameEntity::Init(K2ObjTypes type)
    {
        mType = type;
        auto world = DiK2GameApp::Get()->GetWorld();
        mRenderObj = world->CreateRenderObject(type);
    }

    void DiK2GameEntity::Update(float dt)
    {
        DiK2Entity::Update(dt);

        mRenderObj->Update(dt);
    }

}