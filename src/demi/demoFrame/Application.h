
/********************************************************************
    File:       Application.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DiApplication_h__
#define DiApplication_h__

#include "Callback.h"
#include "OIS.h"

// namespace MyGUI
// {
//     class DemiWrapper;
// }

namespace Demi
{
    struct DemoConfig
    {
        DemoConfig(const DiString& windowTitle, uint32 width = 1024, uint32 height = 768)
        : windowWidth(width)
        , windowHeight(height)
        , fullScreen(false)
        , windowTitle(windowTitle)
        , mediaPath("../../Media")
        , logFile("Demi.log")
        {
        }

        uint32      windowWidth;
        uint32      windowHeight;
        DiString    windowTitle;

        bool        fullScreen;

        DiString    mediaPath;
        DiString    logFile;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_DEMO_API DemiDemo : public DiBase
    {
    public:
        DemiDemo(DemoConfig config);

        ~DemiDemo(void);

        static DemiDemo*            GetApp(){return sTheApp;}

    public:

        typedef Functor0            UpdateCallback;
        typedef Functor0            InitCallback;
        typedef Functor0            ShutdownCallback;

    public:

        void                        Update();

        void                        Close();

        bool                        IsOpen();

        void                        Open();

        void                        CloseEngine();

        DiCameraHelper*             GetCameraHelper() { return mCameraHelper; }
                                    
        void                        SetUpdateCallback(UpdateCallback val) { mUpdateCallback = val; }

        void                        SetShutdownCallback(ShutdownCallback val) { mShutdownCallback = val; }
        
        void                        SetInitCallback(InitCallback val) { mInitCallback = val; }

        void                        keyPressed(const OIS::KeyEvent &arg);

        void                        keyReleased(const OIS::KeyEvent &arg);

        void                        mouseMoved(const OIS::MouseEvent& evt);

        void                        mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        void                        mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        void                        Run();

        void                        OpenImpl();

    protected:

        DiAssetManager*             mAssetManager;
                                    
        DiCameraHelper*             mCameraHelper;
                                    
        static DemiDemo*            sTheApp;

        UpdateCallback              mUpdateCallback;

        InitCallback                mInitCallback;

        ShutdownCallback            mShutdownCallback;

        //MyGUI::DemiWrapper*       mGUIWrapper;

        DiInputManager*             mInputMgr;

        DiInfo*                     mInfo;

        DemoConfig                  mConfig;
    };

}
#endif // Application_h__