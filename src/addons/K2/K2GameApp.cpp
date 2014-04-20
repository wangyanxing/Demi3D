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
#include "Window.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include "K2GameAppOSX.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "K2GameAppIOS.h"
#endif

#define DRV_DX9     0
#define DRV_GL      1
#define DRV_GL_ES2  2

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   define USE_DRV     DRV_GL
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   define USE_DRV     DRV_GL_ES2
#else
#   define USE_DRV     DRV_DX9
#endif

#ifdef DEMI_STATIC_API
#   if USE_DRV == DRV_DX9
#       include "DrvD3D9Plugin.h"
#   elif USE_DRV == DRV_GL
#       include "DrvGLPlugin.h"
#   elif USE_DRV == DRV_GL_ES2
#       include "DrvGLES2Plugin.h"
#   endif
#endif

namespace Demi
{
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

#if USE_DRV == DRV_DX9
        DI_INSTALL_PLUGIN(DiDrvD3D9);
#elif USE_DRV == DRV_GL
        DI_INSTALL_PLUGIN(DiDrvGL);
#elif USE_DRV == DRV_GL_ES2
        DI_INSTALL_PLUGIN(DiDrvGLES2);
#endif

        mAssetManager = new DiAssetManager;
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        mAssetManager->SetBasePath("media");
#else
        mAssetManager->SetBasePath("../../media");
#endif

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

    void DiK2GameApp::Open(int argc, char *argv[])
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
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
        UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
        [pool release];
#else
        DI_WARNING("Unsupported platform, quiting");
#endif
    }

    void DiK2GameApp::CloseEngine()
    {
        SAFE_DELETE(mInputMgr);

        DiK2Configs::Shutdown();

        Driver->Shutdown();

#if USE_DRV == DRV_DX9
        DI_UNINSTALL_PLUGIN(DiDrvD3D9);
#elif USE_DRV == DRV_GL
        DI_UNINSTALL_PLUGIN(DiDrvGL);
#elif USE_DRV == DRV_GL_ES2
        DI_UNINSTALL_PLUGIN(DiDrvGLES2);
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

        DiWindow* wnd = Driver->GetMainRenderWindow()->GetWindow();
        mInputMgr->CreateInput(wnd->GetWndHandle(), wnd->GetWndViewHandle());

        using namespace std::placeholders;

        DiK2Configs::Init();

        mGame = DI_NEW DiK2Game();
        mGame->OpenWorld("maps/test_entity");
        mGame->SetHero("heroes/aluna/model.mdf");

        Driver->GetMainRenderWindow()->SetUpdateCallback([this](){
            mGame->Update();
        });
    }

    DiK2GameApp* DiK2GameApp::sApp = nullptr;

}