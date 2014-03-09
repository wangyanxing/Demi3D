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
#include "Command.h"
#include "LogManager.h"

//#include "MyGUI_DemiWrapper.h"
//#include "MyGUI_MouseButton.h"
//#include "MyGUI_InputManager.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include "ApplicationOSX.h"
#endif

#include <functional>

namespace Demi
{
#if DEMI_PLATFORM == DEMI_PLATFORM_OSX
        const DiString gfxGLDrvLib = "DiDrvGL";
#       define USE_OPEN_GL 1
#else
#   define USE_OPEN_GL 0
#   if DEMI_DEBUG
        const DiString gfxD3D9DrvLib = "DiDrvD3D9_d";
        const DiString gfxGLDrvLib   = "DiDrvGL_d";
#   else
        const DiString gfxD3D9DrvLib = "DiDrvD3D9";
        const DiString gfxGLDrvLib   = "DiDrvGL";
#   endif
#endif

    DemiDemo::DemiDemo(DemoConfig config) :
        mAssetManager(nullptr)
        , mInputMgr(nullptr)
        //, mInfo(nullptr)
        //, mGUIWrapper(nullptr)
        , mConfig(config)
        , mQuit(false)
    {
        DI_INIT_PROFILER;

        srand((unsigned int)time(NULL));

        DI_ASSERT(!sTheApp);
        sTheApp = this;

        DiLogManager* logmgr = DI_NEW DiLogManager();
        logmgr->Init(mConfig.logFile);

#if USE_OPEN_GL
        DiPlugin::LoadPlugin(gfxGLDrvLib);
#else
        DiPlugin::LoadPlugin(gfxD3D9DrvLib);
#endif

        mAssetManager = new DiAssetManager;
        mAssetManager->SetBasePath(mConfig.mediaPath);

        CommandMgr->RegisterString("scene_type", config.sceneType, 0, "Scene manager type");
    }

    DemiDemo::~DemiDemo(void)
    {
        DI_CLOSE_PROFILER;
    }

    void DemiDemo::Update()
    {
        DI_PROFILE_BEGIN_FRAME

        if (mUpdateCallback)
            mUpdateCallback();

        mInputMgr->captureInput();
        mCameraHelper->Update(Driver->GetDeltaSecond());

        //mInfo->Update(mDriver->GetMainRenderWindow()->GetDeltaSecond());
        if (mMainHwnd)
            Driver->Render();
        
        if (mQuit)
            CloseEngine();

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

    void DemiDemo::Open()
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

    void DemiDemo::mouseMoved(const OIS::MouseEvent& evt)
    {
        mCameraHelper->OnMouseMove(evt);
    }

    void DemiDemo::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        mCameraHelper->OnMouseDown(evt, id);
    }

    void DemiDemo::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        mCameraHelper->OnMouseUp(evt, id);
    }

    void DemiDemo::CloseEngine()
    {
        if (mShutdownCallback)
            mShutdownCallback();

        mInputMgr->unregisterKeyPressEvent("App::KeyDown");
        mInputMgr->unregisterKeyReleaseEvent("App::KeyUp");
        mInputMgr->unregisterMouseMoveEvent("App::MsMove");
        mInputMgr->unregisterMousePressEvent("App::MsDown");
        mInputMgr->unregisterMouseReleaseEvent("App::MsUp");

        //SAFE_DELETE(mInfo);
        //SAFE_DELETE(mGUIWrapper);
        SAFE_DELETE(mInputMgr);
        SAFE_DELETE(mCameraHelper);
        
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

    void DemiDemo::Run()
    {
        while (IsOpen())
            Update();
    }

    void DemiDemo::OpenImpl()
    {
        bool ret = Driver->Init(mConfig.windowWidth, mConfig.windowHeight, mConfig.windowTitle, mConfig.fullScreen);
        DI_ASSERT(ret);

        mInputMgr = new DiInputManager();
        mInputMgr->createInput((size_t)(mMainHwnd));
        
        using namespace std::placeholders;

        mInputMgr->registerKeyPressEvent("App::KeyDown", std::bind(&DemiDemo::keyPressed, this, _1));
        mInputMgr->registerKeyReleaseEvent("App::KeyUp", std::bind(&DemiDemo::keyReleased, this, _1));
        mInputMgr->registerMouseMoveEvent("App::MsMove", std::bind(&DemiDemo::mouseMoved, this, _1));
        mInputMgr->registerMousePressEvent("App::MsDown", std::bind(&DemiDemo::mousePressed, this, _1, _2));
        mInputMgr->registerMouseReleaseEvent("App::MsUp", std::bind(&DemiDemo::mouseReleased, this, _1, _2));

        //mGUIWrapper = new MyGUI::DemiWrapper();
        //mGUIWrapper->init("MyGUI_Core.xml");

        //mInfo = new DiInfo();

        DiCamera* cam = Driver->GetSceneManager()->GetCamera();
        mCameraHelper = new DiCameraHelper(cam);

        if (mInitCallback)
            mInitCallback();
    }

    DemiDemo* DemiDemo::sTheApp = nullptr;
}