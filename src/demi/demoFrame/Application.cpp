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

#include "DemoPch.h"
#include "Application.h"
#include "GfxDriver.h"
#include "AssetManager.h"
#include "CameraHelper.h"
#include "SceneManager.h"
#include "RenderWindow.h"
#include "InputManager.h"
#include "Info.h"
#include "EnginePlugin.h"
#include "GfxDriver.h"
#include "Window.h"
#include "Command.h"
#include "LogManager.h"
#include "ScriptPlugin.h"
#include "PathLib.h"

#include "MyGUI.h"
#include "MyGUI_DemiWrapper.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include "ApplicationOSX.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "ApplicationIOS.h"
#endif

#include <functional>

//////////////////////////////////////////////////////////////////////////

#define DRV_DX9     0
#define DRV_GL      1
#define DRV_GL_ES2  2

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   define USE_DRV     DRV_GL
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   define USE_DRV     DRV_GL_ES2
#else
#   define USE_DRV     DRV_GL
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

//////////////////////////////////////////////////////////////////////////

namespace Demi
{
    static bool Quit(DiCmdArgs*)
    {
        DemiDemo::GetApp()->QuitApp();
        return true;
    }

    DemiDemo::DemiDemo(DemoConfig config) :
        mAssetManager(nullptr)
        , mInputMgr(nullptr)
        , mConfig(config)
        , mQuit(false)
        , mGUIWrapper(nullptr)
    {
        DI_INIT_PROFILER;

        srand((unsigned int)time(NULL));

        DI_ASSERT(!sTheApp);
        sTheApp = this;

        DiLogManager* logmgr = DI_NEW DiLogManager();
        logmgr->Init(mConfig.logFile);

#if USE_DRV == DRV_DX9
        DI_INSTALL_PLUGIN(DiDrvD3D9);
#elif USE_DRV == DRV_GL
        DI_INSTALL_PLUGIN(DiDrvGL);
#elif USE_DRV == DRV_GL_ES2
        DI_INSTALL_PLUGIN(DiDrvGLES2);
#endif

#if USE_SCRIPT
        DI_INSTALL_PLUGIN(DiScript);
#endif

        mAssetManager = new DiAssetManager;
        mAssetManager->SetBasePath(mConfig.mediaPath);

        CommandMgr->RegisterString("scene_type", config.sceneType, 0, "Scene manager type");
        CommandMgr->AddCommand("quit", Quit, "Close Application");

        DiPathLib::ResetCurrentDir();
    }

    DemiDemo::~DemiDemo(void)
    {
        DI_CLOSE_PROFILER;
    }

    void DemiDemo::Update()
    {
        DI_PROFILE_BEGIN_FRAME
        
        float delta = Driver->GetDeltaSecond();
#if 0
        static DiTimer fpstimer;
        if (fpstimer.GetElapse() > 1.0)
        {
            fpstimer.Start();
            printf("FPS:%g\n",1.0f/delta);
        }
#endif
        
        if (mUpdateCallback)
            mUpdateCallback();

        mInputMgr->captureInput();
        mCameraHelper->Update(delta);

        if (mMainHwnd)
            Driver->Render();
        
        if (mQuit)
            Close();

        DI_PROFILE_END_FRAME
    }

    void DemiDemo::Close()
    {
        CloseEngine();
    }

    bool DemiDemo::IsOpen()
    {
        return mMainHwnd ? true : false;
    }

    void DemiDemo::Open(int argc, char *argv[])
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

    void DemiDemo::keyPressed(const OIS::KeyEvent& evt)
    {
        if (evt.key == OIS::KC_ESCAPE)
        {
            mQuit = true;
        }

        mCameraHelper->OnKeyDown(evt);
    }

    void DemiDemo::keyReleased(const OIS::KeyEvent& evt)
    {
        mCameraHelper->OnKeyUp(evt);
    }
    
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
    
    void DemiDemo::mouseMoved(const OIS::MultiTouchEvent& evt)
    {
        mCameraHelper->OnMouseMove(evt);
    }
    
    void DemiDemo::mousePressed(const OIS::MultiTouchEvent& evt)
    {
        mCameraHelper->OnMouseDown(evt);
    }
    
    void DemiDemo::mouseReleased(const OIS::MultiTouchEvent& evt)
    {
        mCameraHelper->OnMouseUp(evt);
    }
    
#else
    
    void DemiDemo::mouseMoved(const OIS::MouseEvent& evt)
    {
        mCameraHelper->OnMouseMove(evt.state);
    }
    
    void DemiDemo::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        mCameraHelper->OnMouseDown(evt.state, id);
    }
    
    void DemiDemo::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        mCameraHelper->OnMouseUp(evt.state, id);
    }
    
#endif

    void DemiDemo::CloseEngine()
    {
        if (mShutdownCallback)
            mShutdownCallback();

        mInputMgr->unregisterKeyPressEvent("App::KeyDown");
        mInputMgr->unregisterKeyReleaseEvent("App::KeyUp");
        mInputMgr->unregisterMouseMoveEvent("App::MsMove");
        mInputMgr->unregisterMousePressEvent("App::MsDown");
        mInputMgr->unregisterMouseReleaseEvent("App::MsUp");

        SAFE_DELETE(mInputMgr);
        SAFE_DELETE(mCameraHelper);
        SAFE_DELETE(mGUIWrapper);

        Driver->Shutdown();

#if USE_SCRIPT
        DI_UNINSTALL_PLUGIN(DiScript);
#endif

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

    void DemiDemo::Run()
    {
        while (IsOpen())
            Update();
    }

    void DemiDemo::OpenImpl()
    {
        bool ret = Driver->Init(mConfig.windowWidth, mConfig.windowHeight, mConfig.windowTitle, mConfig.fullScreen);
        DI_ASSERT(ret);

        mInputMgr = DI_NEW DiInputManager();
        
        DiWindow* wnd = Driver->GetMainRenderWindow()->GetWindow();
        mInputMgr->createInput(wnd->GetWndHandle(), wnd->GetWndViewHandle());
        
        using namespace std::placeholders;

        mInputMgr->registerKeyPressEvent("App::KeyDown", std::bind(&DemiDemo::keyPressed, this, _1));
        mInputMgr->registerKeyReleaseEvent("App::KeyUp", std::bind(&DemiDemo::keyReleased, this, _1));
        mInputMgr->registerMouseMoveEvent("App::MsMove", std::bind(&DemiDemo::mouseMoved, this, _1));
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        mInputMgr->registerMousePressEvent("App::MsDown", std::bind(&DemiDemo::mousePressed, this, _1));
        mInputMgr->registerMouseReleaseEvent("App::MsUp", std::bind(&DemiDemo::mouseReleased, this, _1));
#else
        mInputMgr->registerMousePressEvent("App::MsDown", std::bind(&DemiDemo::mousePressed, this, _1,_2));
        mInputMgr->registerMouseReleaseEvent("App::MsUp", std::bind(&DemiDemo::mouseReleased, this, _1,_2));
#endif

        DiCamera* cam = Driver->GetSceneManager()->GetCamera();
        mCameraHelper = DI_NEW DiCameraHelper(cam);
        
        mGUIWrapper = new MyGUI::DemiWrapper();
        mGUIWrapper->init("Editor.xml");

        if (mInitCallback)
            mInitCallback();
    }

    DemiDemo* DemiDemo::sTheApp = nullptr;
}