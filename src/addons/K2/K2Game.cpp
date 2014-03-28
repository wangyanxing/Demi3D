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
#include "K2GameApp.h"
#include "K2Input.h"
#include "K2TerrainMap.h"
#include "K2EntityManager.h"
#include "K2RenderObjects.h"

#include "DirectionalLight.h"
#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"

namespace Demi
{
    DiK2Game::DiK2Game()
        : mWorld(nullptr)
        , mEntityMgr(nullptr)
        , mCamera(nullptr)
    {
        Init();
    }

    DiK2Game::~DiK2Game()
    {
        DI_DELETE mCamera;
        DI_DELETE mEntityMgr;
        DI_DELETE mWorld;
    }

    void DiK2Game::Update()
    {
        float dt = Driver->GetDeltaSecond();
        if (mCamera)
            mCamera->Update(dt);
        mWorld->Update(dt);
        mEntityMgr->Update(dt);
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
        mHero = GetEntityManager()->CreateHero(1);
        mHero->SetModel(model);
        DiVec3 spawnPoint = mWorld->GetSpawnPoint(0);
        mHero->GetRenderObj()->SetWorldPosition(spawnPoint);

        if (mCamera)
            mCamera->SetTarget(mHero->GetRenderObj());
    }

    void DiK2Game::Init()
    {
        mEntityMgr = DI_NEW DiK2EntityManager();

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));

        DiDirLightPtr dirlight = make_shared<DiDirLight>();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(DiVec3(-0.5f, -0.866f, 0).normalisedCopy());

        SetCamera(DiK2GameCamera::STYLE_CHARACTER);
    }

    void DiK2Game::OnKeyInput(const K2KeyEvent& event)
    {
        if (mHero)
            mHero->OnKeyInput(event);
        if (mWorld)
            mWorld->OnKeyInput(event);
        if (mCamera)
            mCamera->OnKeyInput(event);
    }

    void DiK2Game::OnMouseInput(const K2MouseEvent& event)
    {
        if (mHero)
            mHero->OnMouseInput(event);
        if (mWorld)
            mWorld->OnMouseInput(event);
        if (mCamera)
            mCamera->OnMouseInput(event);
    }

    void DiK2Game::SetCamera(DiK2GameCamera::Style style)
    {
        auto i = mCameraPool.find(style);
        if (i != mCameraPool.end())
        {
            mCamera = i->second;
            return;
        }

        DiCamera* cam = Driver->GetSceneManager()->GetCamera();

        switch (style)
        {
        case Demi::DiK2GameCamera::STYLE_FIXED:
            mCamera = DI_NEW DiK2GameCamera(cam);
            break;
        case Demi::DiK2GameCamera::STYLE_FREE:
            mCamera = DI_NEW DiK2FreeCamera(cam);
            break;
        case Demi::DiK2GameCamera::STYLE_CHARACTER:
            mCamera = DI_NEW DiK2CharCamera(cam);
            break;
        default:
            break;
        }
    }  
}