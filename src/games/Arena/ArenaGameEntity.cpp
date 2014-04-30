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

namespace Demi
{

    ArGameEntity::ArGameEntity()
    {

    }

    ArGameEntity::~ArGameEntity()
    {

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

}