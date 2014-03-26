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
        auto input = DiK2GameApp::GetApp()->GetInputManager();
        input->unregisterKeyPressEvent("Game::KeyDown");
        input->unregisterKeyReleaseEvent("Game::KeyUp");
        input->unregisterMouseMoveEvent("Game::MsMove");
        input->unregisterMousePressEvent("Game::MsDown");
        input->unregisterMouseReleaseEvent("Game::MsUp");

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

        mHero = DI_NEW DiK2HeroEntity(this);
        mHero->LoadModel(model);

        mCamera->SetTarget(mHero);
    }

    void DiK2Game::Init()
    {
        using namespace std::placeholders;
        auto input = DiK2GameApp::GetApp()->GetInputManager();
        input->registerKeyPressEvent("Game::KeyDown", std::bind(&DiK2Game::KeyPressed, this, _1));
        input->registerKeyReleaseEvent("Game::KeyUp", std::bind(&DiK2Game::KeyReleased, this, _1));
        input->registerMouseMoveEvent("Game::MsMove", std::bind(&DiK2Game::MouseMoved, this, _1));
        input->registerMousePressEvent("Game::MsDown", std::bind(&DiK2Game::MousePressed, this, _1, _2));
        input->registerMouseReleaseEvent("Game::MsUp", std::bind(&DiK2Game::MouseReleased, this, _1, _2));

        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor(0.3f, 0.3f, 0.3f));

        DiDirLightPtr dirlight = make_shared<DiDirLight>();
        DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
        dirNode->AttachObject(dirlight);
        dirlight->SetColor(DiColor());
        dirlight->SetDirection(DiVec3(-0.5f, -0.866f, 0).normalisedCopy());
    }

    void DiK2Game::KeyPressed(const OIS::KeyEvent &arg)
    {

    }

    void DiK2Game::KeyReleased(const OIS::KeyEvent &arg)
    {

    }

    void DiK2Game::MouseMoved(const OIS::MouseEvent& evt)
    {

    }

    void DiK2Game::MousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        if (id == OIS::MB_Left)
        {
            // click on map
            auto terrain = mWorld->GetTerrain();

            float screenPosX = float(evt.state.X.abs) / float(evt.state.width);
            float screenPosY = float(evt.state.Y.abs) / float(evt.state.height);

            screenPosX = DiMath::Clamp<float>(screenPosX, 0, 1);
            screenPosY = DiMath::Clamp<float>(screenPosY, 0, 1);

            DiRay ray = mCamera->GetCamera()->GetCameraToViewportRay(screenPosX, screenPosY);
            DiVec3 clickout;
            if (terrain->RayIntersects(ray, clickout))
            {
                
            }
        }
    }

    void DiK2Game::MouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {

    }
}