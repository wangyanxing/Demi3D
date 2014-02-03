
/********************************************************************
    File:       Application.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DiApplication_h__
#define DiApplication_h__

#include "Callback.h"
#include "OIS.h"

namespace MyGUI
{
    class DemiWrapper;
}

namespace Demi
{
    class DiWindowUtil;

    class DEMI_DEMO_API DiApp : public DiBase
    {
    public:
        DiApp(const DiString& mediaPath = "../../Media/",const DiString& logName = "demiEngine.log");

        ~DiApp(void);

        static DiApp*                GetApp(){return sTheApp;}

    public:

        typedef Functor0            UpdateCallback;
        typedef Functor0            InitCallback;
        typedef Functor0            ShutdownCallback;

    public:

        // Open a window
        void                        Update();

        void                        Close();

        bool                        IsOpen();

        void                        Open(uint32 w, uint32 h, const char* title, bool fullscreen = false);

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

    protected:

        DiAssetManager*             mAssetManager;
                                    
        DiCameraHelper*             mCameraHelper;
                                    
        static DiApp*               sTheApp;

        DiString                    mMediaPath;
        
        DiString                    mLogName;

        UpdateCallback              mUpdateCallback;

        InitCallback                mInitCallback;

        ShutdownCallback            mShutdownCallback;

        //MyGUI::DemiWrapper*       mGUIWrapper;

        DiInputManager*             mInputMgr;

        DiInfo*                     mInfo;
    };

}
#endif // Application_h__