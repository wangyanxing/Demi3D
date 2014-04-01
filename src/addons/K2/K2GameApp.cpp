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
#include "K2GameApp.h"
#include "GfxDriver.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "RenderWindow.h"
#include "K2Input.h"
#include "K2Configs.h"
#include "EnginePlugin.h"
#include "GfxDriver.h"
#include "Command.h"
#include "LogManager.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include "K2GameAppOSX.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "K2GameAppIOS.h"
#endif

namespace Demi
{
    const DiString gfxD3D9DrvLib = "DiDrvD3D9";
    const DiString gfxGLDrvLib = "DiDrvGL";

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   define USE_OPEN_GL 1
#else
#   define USE_OPEN_GL 0
#endif

    DiK2GameApp::DiK2GameApp()
        :mAssetManager(nullptr)
        , mInputMgr(nullptr)
        , mQuit(false)
        , mGame(nullptr)
    {
        DI_INIT_PROFILER;

        DI_ASSERT(!sApp);
        sApp = this;

        srand((unsigned int)time(NULL));

        DiLogManager* logmgr = DI_NEW DiLogManager();
        logmgr->Init("Game.log");

#if USE_OPEN_GL
        DiPlugin::LoadPlugin(gfxGLDrvLib);
#else
        DiPlugin::LoadPlugin(gfxD3D9DrvLib);
#endif

        mAssetManager = new DiAssetManager;
        mAssetManager->SetBasePath("../../Media");

        CommandMgr->RegisterString("scene_type", "Octree", 0, "Scene manager type");
    }

    DiK2GameApp::~DiK2GameApp()
    {
        DI_CLOSE_PROFILER;
    }

    void DiK2GameApp::Update()
    {
        DI_PROFILE_BEGIN_FRAME

        mInputMgr->Update();

        if (mMainHwnd)
            Driver->Render();

        if (mQuit)
            CloseEngine();

        DI_PROFILE_END_FRAME
    }

    void DiK2GameApp::Close()
    {
        CloseEngine();

        DI_DELETE mGame;
        mGame = nullptr;
    }


    bool DiK2GameApp::IsOpen()
    {
        return mMainHwnd ? true : false;
    }

    void DiK2GameApp::Open()
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

        mAppDelegate = [[AppDelegate alloc] init];
        [[NSApplication sharedApplication] setDelegate:mAppDelegate];

        [NSApp run];

        [pool drain];
        [mAppDelegate release];

#elif DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        OpenImpl();
        Run();

#else
        DI_WARNING("Unsupported platform, quiting");
#endif
    }

    void DiK2GameApp::OnKeyPressed(const OIS::KeyEvent& evt)
    {
        if (evt.key == OIS::KC_ESCAPE)
            mQuit = true;
    }

    void DiK2GameApp::OnKeyReleased(const OIS::KeyEvent& evt)
    {
    }

    void DiK2GameApp::CloseEngine()
    {
        mInputMgr->UnregisterKeyPressEvent("App::KeyDown");
        mInputMgr->UnregisterKeyReleaseEvent("App::KeyUp");

        SAFE_DELETE(mInputMgr);

        DiK2Configs::Shutdown();

        Driver->Shutdown();

#if USE_OPEN_GL
        DiPlugin::UnloadPlugin(gfxGLDrvLib);
#else
        DiPlugin::UnloadPlugin(gfxD3D9DrvLib);
#endif

        SAFE_DELETE(mAssetManager);

        if (DiLogManager::GetInstancePtr())
            DI_DELETE DiLogManager::GetInstancePtr();
    }

    void DiK2GameApp::Run()
    {
        while (IsOpen())
            Update();
    }

    void DiK2GameApp::OpenImpl()
    {
        bool ret = Driver->Init(1024,768, "Game", false);
        DI_ASSERT(ret);

        mInputMgr = new DiK2Input();
        mInputMgr->CreateInput((size_t)(mMainHwnd));

        using namespace std::placeholders;

        mInputMgr->RegisterKeyPressEvent("App::KeyDown", std::bind(&DiK2GameApp::OnKeyPressed, this, _1));
        mInputMgr->RegisterKeyReleaseEvent("App::KeyUp", std::bind(&DiK2GameApp::OnKeyReleased, this, _1));
    
        DiK2Configs::Init();

        mGame = DI_NEW DiK2Game();
        mGame->OpenWorld("maps/grimmscrossing");
        mGame->SetHero("heroes/aluna/model.mdf");

        Driver->GetMainRenderWindow()->SetUpdateCallback([this](){
            mGame->Update();
        });
    }

    DiK2GameApp* DiK2GameApp::sApp = nullptr;

}