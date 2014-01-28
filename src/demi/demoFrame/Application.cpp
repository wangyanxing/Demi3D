#include "DemoPch.h"
#include "Application.h"
#include "GfxDriver.h"
#include "AssetManager.h"
#include "CameraHelper.h"
#include "SceneManager.h"
#include "RenderWindow.h"
//#include "MyGUI_DemiWrapper.h"
//#include "MyGUI_MouseButton.h"
//#include "MyGUI_InputManager.h"
#include "InputManager.h"
#include "Info.h"
#include "EnginePlugin.h"
#include "GfxDriver.h"
#include "Command.h"

#define USE_OPEN_GL 1

namespace Demi
{
#ifdef _DEBUG
    const DiString gfxD3D9DrvLib = "DiDrvD3D9_d";
    const DiString gfxGLDrvLib = "DiDrvGL_d";
#else
    const DiString gfxD3D9DrvLib = "DiDrvD3D9";
    const DiString gfxGLDrvLib = "DiDrvGL";
#endif

    DiApp::DiApp(const DiString& mediaPath,const DiString& logName) :
          mAssetManager(nullptr)
        //, mGUIWrapper(nullptr)
        , mInputMgr(nullptr)
        //, mInfo(nullptr)
        , mMediaPath(mediaPath)
        , mLogName(logName)
    {
        //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
        //_CrtSetBreakAlloc(225);

        DI_INIT_PROFILER;

        srand((unsigned int)time(NULL));

        DI_ASSERT(!sTheApp);
        sTheApp = this;
    }

    DiApp::~DiApp(void)
    {
        DI_CLOSE_PROFILER;

        //_CrtDumpMemoryLeaks();
    }

    void DiApp::Update()
    {
        DI_PROFILE_BEGIN_FRAME
        
        if (mUpdateCallback)
            mUpdateCallback();

        mInputMgr->captureInput();
        mCameraHelper->Update(Driver->GetDeltaSecond());

        //mInfo->Update(mDriver->GetMainRenderWindow()->GetDeltaSecond());
        if (mMainHwnd)
            Driver->Render();

        DI_PROFILE_END_FRAME
    }

    void DiApp::Close()
    {
        CloseEngine();
    }

    bool DiApp::IsOpen()
    {
        return mMainHwnd ? true : false;
    }

    void DiApp::Open(uint32 w, uint32 h, const char* title, bool fullscreen)
    {
        DiLogManager* logmgr = new DiLogManager;
        logmgr->Init(mLogName);

#if USE_OPEN_GL
        DiPlugin::LoadPlugin(gfxGLDrvLib);
#else
        DiPlugin::LoadPlugin(gfxD3D9DrvLib);
#endif

        mAssetManager = new DiAssetManager;
        mAssetManager->SetBasePath(mMediaPath);

        bool ret = Driver->Init(w,h,title,fullscreen);
        DI_ASSERT(ret);

        mInputMgr = new DiInputManager();
        mInputMgr->createInput((size_t)(mMainHwnd));

        mInputMgr->registerKeyPressEvent("App::KeyDown", Demi::functor(*this, &DiApp::keyPressed));
        mInputMgr->registerKeyReleaseEvent("App::KeyUp", Demi::functor(*this, &DiApp::keyReleased));
        mInputMgr->registerMouseMoveEvent("App::MsMove", Demi::functor(*this, &DiApp::mouseMoved));
        mInputMgr->registerMousePressEvent("App::MsDown", Demi::functor(*this, &DiApp::mousePressed));
        mInputMgr->registerMouseReleaseEvent("App::MsUp", Demi::functor(*this, &DiApp::mouseReleased));

        //mGUIWrapper = new MyGUI::DemiWrapper();
        //mGUIWrapper->init("MyGUI_Core.xml");

        //mInfo = new DiInfo();

        DiCamera* cam = Driver->GetSceneManager()->GetCamera();
        mCameraHelper = new DiCameraHelper(cam);

        if (mInitCallback)
            mInitCallback();
    }

    void DiApp::keyPressed(const OIS::KeyEvent& evt)
    {
        if (evt.key == OIS::KC_ESCAPE)
        {
            CommandMgr->ExecuteCommand("quit");
        }

        mCameraHelper->OnKeyDown(evt);
    }

    void DiApp::keyReleased(const OIS::KeyEvent& evt)
    {
        mCameraHelper->OnKeyUp(evt);
    }

    void DiApp::mouseMoved(const OIS::MouseEvent& evt)
    {
        mCameraHelper->OnMouseMove(evt);
    }

    void DiApp::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        mCameraHelper->OnMouseDown(evt, id);
    }

    void DiApp::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        mCameraHelper->OnMouseUp(evt, id);
    }

    void DiApp::CloseEngine()
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

    DiApp* DiApp::sTheApp = NULL;
}