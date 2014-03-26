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
#include "K2Game.h"
#include "K2World.h"
#include "K2Hero.h"
#include "K2GameCamera.h"

#include "DirectionalLight.h"
#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"

namespace Demi
{
    DiK2Game::DiK2Game()
        : mWorld(nullptr)
        , mHero(nullptr)
        , mCamera(nullptr)
    {
        mCamera = DI_NEW DiK2GameCamera(Driver->GetSceneManager()->GetCamera());

        Init();
    }

    DiK2Game::~DiK2Game()
    {
        DI_DELETE mWorld;
        DI_DELETE mHero;
        DI_DELETE mCamera;
    }

    void DiK2Game::Update()
    {
        mCamera->Update();
    }

    void DiK2Game::OpenWorld(const DiString& path)
    {
        if (mWorld)
            DI_DELETE mWorld;

        mWorld = DI_NEW DiK2World();
        mWorld->Load(path);
    }

    void DiK2Game::SetHero(const DiString& model)
    {
        if (mHero)
            DI_DELETE mHero;

        mHero = DI_NEW DiK2Hero(this);
        mHero->LoadModel(model);

        mCamera->SetTarget(mHero);
    }

    void DiK2Game::Init()
    {
        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));

        DiDirLightPtr dirlight = make_shared<DiDirLight>();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(DiVec3(-0.5f, -0.866f, 0).normalisedCopy());
    }
}