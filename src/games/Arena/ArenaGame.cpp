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
#include "ArenaGame.h"
#include "ArenaHero.h"
#include "ArenaGameCamera.h"
#include "ArenaGameApp.h"
#include "ArenaInput.h"
#include "ArenaLevel.h"
#include "ArenaEntityManager.h"

#include "K2World.h"
#include "K2Terrain.h"
#include "K2RenderObjects.h"

#include "RenderWindow.h"
#include "PostEffectManager.h"
#include "PostEffect.h"
#include "DirectionalLight.h"
#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"

namespace Demi
{
    ArGame::ArGame()
        : mEntityMgr(nullptr)
        , mCamera(nullptr)
        , mLevel(nullptr)
    {
        Init();
    }

    ArGame::~ArGame()
    {
        DI_DELETE mCamera;
        DI_DELETE mEntityMgr;
        DI_DELETE mLevel;
    }

    void ArGame::Update()
    {
        float dt = Driver->GetDeltaSecond();
        if (mCamera)
            mCamera->Update(dt);
        mLevel->Update(dt);
        mEntityMgr->Update(dt);
    }

    void ArGame::LoadLevel(const DiString& path)
    {
        mLevel->LoadMap(path);
    }

    void ArGame::SetHero(const DiString& configFile)
    {
        if (!mHero)
            mHero = GetEntityManager()->CreateHero(1);

        mHero->LoadHero(configFile);

        if (mCamera)
            mCamera->SetTarget(mHero->GetRenderObj());
    }

    void ArGame::Init()
    {
        mEntityMgr = DI_NEW ArEntityManager();

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));

        DiDirLightPtr dirlight = make_shared<DiDirLight>();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(DiVec3(-0.5f, -0.866f, 0).normalisedCopy());

        DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
        DiPostEffect* bloom = peMgr->GetEffect("Bloom");
        if (bloom)
            bloom->SetEnable(true);

        SetCamera(ArGameCamera::STYLE_CHARACTER);

        mLevel = DI_NEW ArLevel();
    }

    void ArGame::OnKeyInput(const K2KeyEvent& event)
    {
        if (mHero)
            mHero->OnKeyInput(event);
        if (mCamera)
            mCamera->OnKeyInput(event);
    }

    void ArGame::OnMouseInput(const K2MouseEvent& event)
    {
        if (mHero)
            mHero->OnMouseInput(event);
        if (mCamera)
            mCamera->OnMouseInput(event);
    }

    void ArGame::SetCamera(ArGameCamera::Style style)
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
        case Demi::ArGameCamera::STYLE_FIXED:
            mCamera = DI_NEW ArGameCamera(cam);
            break;
        case Demi::ArGameCamera::STYLE_FREE:
            mCamera = DI_NEW DiK2FreeCamera(cam);
            break;
        case Demi::ArGameCamera::STYLE_CHARACTER:
            mCamera = DI_NEW DiK2CharCamera(cam);
            break;
        default:
            break;
        }
    }  

    DiK2World* ArGame::GetWorld()
    {
        return mLevel->GetWorld();
    }
}