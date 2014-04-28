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

#ifndef DiApplication_h__
#define DiApplication_h__

#include "DemoPrerequisites.h"
#include "OIS.h"

namespace Demi
{
    struct DemoConfig
    {
        DemoConfig(const DiString& title, uint32 width = 1024, uint32 height = 768)
        : windowWidth(width)
        , windowHeight(height)
        , fullScreen(false)
        , windowTitle(title)
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        , mediaPath("media")
#else
        , mediaPath("../../media")
#endif
        , logFile("Demi.log")
        , sceneType("Octree")
        {
        }

        uint32      windowWidth;
        uint32      windowHeight;
        DiString    windowTitle;
        
        // scene manager type
        DiString    sceneType;

        bool        fullScreen;

        DiString    mediaPath;
        DiString    logFile;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_DEMO_API DemiDemo : public DiBase
    {
    public:
        DemiDemo(DemoConfig config);

        virtual ~DemiDemo(void);

        static DemiDemo* GetApp(){return sTheApp;}

    public:

        typedef std::function<void(void)> UpdateCallback;
        typedef std::function<void(void)> InitCallback;
        typedef std::function<void(void)> ShutdownCallback;

    public:

        virtual void      Update();

        virtual void      Close();

        bool              IsOpen();

        void              Open(int argc, char *argv[]);

        void              CloseEngine();

        DiInputManager*   GetInputManager() { return mInputMgr; }

        DiCameraHelper*   GetCameraHelper() { return mCameraHelper; }
                          
        void              SetUpdateCallback(UpdateCallback val) { mUpdateCallback = val; }

        void              SetShutdownCallback(ShutdownCallback val) { mShutdownCallback = val; }
        
        void              SetInitCallback(InitCallback val) { mInitCallback = val; }

        virtual void      keyPressed(const OIS::KeyEvent &arg);

        virtual void      keyReleased(const OIS::KeyEvent &arg);
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        virtual void      mouseMoved(const OIS::MultiTouchEvent& evt);

        virtual void      mousePressed(const OIS::MultiTouchEvent& evt);

        virtual void      mouseReleased(const OIS::MultiTouchEvent& evt);
#else
        virtual void      mouseMoved(const OIS::MouseEvent& evt);
        
        virtual void      mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
        
        virtual void      mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
#endif

        void              Run();

        virtual void      OpenImpl();

        void              QuitApp() { mQuit = true; }

    protected:

        DiAssetManager*         mAssetManager;
                                
        DiCameraHelper*         mCameraHelper;
                                
        static DemiDemo*        sTheApp;

        UpdateCallback          mUpdateCallback;

        InitCallback            mInitCallback;

        ShutdownCallback        mShutdownCallback;

        MyGUI::DemiWrapper*     mGUIWrapper;

        DiInputManager*         mInputMgr;

        DiInfo*                 mInfo;

        DemoConfig              mConfig;
        
        bool                    mQuit;
    };

}
#endif // Application_h__