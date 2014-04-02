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
/********************************************************************
File:       Mouse.h
Creator:    InputManager.h
*********************************************************************/

#ifndef DiInputManager_h__
#define DiInputManager_h__

//#include "MyGUI_KeyCode.h"
//#include "MyGUI_MouseButton.h"
//#include "MyGUI_Types.h"

#include <functional>
#include "OIS.h"

#if OIS_VERSION >= 0x010300		//  OIS_VERSION >= 1.3.0
#define OIS_WITH_MULTITOUCH		1
#else							//  OIS_VERSION == 1.2.0
#define OIS_WITH_MULTITOUCH		0
#endif

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32 || DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   define DEMI_KEYMOUSE
#endif

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   define DEMI_TOUCH
#endif


namespace Demi
{
    class DEMI_DEMO_API DiInputManager : public DiBase

#ifdef DEMI_KEYMOUSE
        , public OIS::MouseListener, public OIS::KeyListener
#endif
#ifdef DEMI_TOUCH
        , public OIS::MultiTouchListener
#endif
    {
    public:
        DiInputManager();
        virtual ~DiInputManager();

#ifdef DEMI_KEYMOUSE
        typedef std::function<void(const OIS::MouseEvent&)> MouseMoveCallback;
        typedef std::function<void(const OIS::MouseEvent&, OIS::MouseButtonID)> MousePressCallback;
        typedef std::function<void(const OIS::MouseEvent&, OIS::MouseButtonID)> MouseReleaseCallback;
#elif defined(DEMI_TOUCH)
        typedef std::function<void(const OIS::MultiTouchEvent&)> MouseMoveCallback;
        typedef std::function<void(const OIS::MultiTouchEvent&)> MousePressCallback;
        typedef std::function<void(const OIS::MultiTouchEvent&)> MouseReleaseCallback;
#endif
        
        typedef std::function<void(const OIS::KeyEvent&)> KeyPressCallback;
        typedef std::function<void(const OIS::KeyEvent&)> KeyReleaseCallback;

        void createInput(DiWndHandle wndhandle, DiWndViewHandle viewhandle);
        void destroyInput();
        void captureInput();
        void setInputViewSize(int _width, int _height);

        bool registerMouseMoveEvent(const DiString& name, MouseMoveCallback cb);
        bool registerMousePressEvent(const DiString& name, MousePressCallback cb);
        bool registerMouseReleaseEvent(const DiString& name, MouseReleaseCallback cb);
        bool registerKeyPressEvent(const DiString& name, KeyPressCallback cb);
        bool registerKeyReleaseEvent(const DiString& name, KeyReleaseCallback cb);

        bool unregisterMouseMoveEvent(const DiString& name);
        bool unregisterMousePressEvent(const DiString& name);
        bool unregisterMouseReleaseEvent(const DiString& name);
        bool unregisterKeyPressEvent(const DiString& name);
        bool unregisterKeyReleaseEvent(const DiString& name);

        void setMousePosition(int _x, int _y);
        void updateCursorPosition();

        OIS::Keyboard* getKeyboard() { return mKeyboard; }
        OIS::Mouse* getMouse() { return mMouse; }

    private:
        
#ifdef DEMI_KEYMOUSE
        bool mouseMoved(const OIS::MouseEvent& _arg);
        bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool keyPressed(const OIS::KeyEvent& _arg);
        bool keyReleased(const OIS::KeyEvent& _arg);
#endif
        
#ifdef DEMI_TOUCH
		bool touchMoved( const OIS::MultiTouchEvent &arg );
		bool touchPressed( const OIS::MultiTouchEvent &arg );
		bool touchReleased( const OIS::MultiTouchEvent &arg );
		bool touchCancelled( const OIS::MultiTouchEvent &arg );
#endif

        void checkPosition();

    private:
        OIS::InputManager* mInputManager;
        
        OIS::Keyboard* mKeyboard;
        
        OIS::Mouse* mMouse;
        
#if OIS_WITH_MULTITOUCH
		OIS::MultiTouch* mMultiTouch;
#endif
		OIS::JoyStick* mAccelerometer;

        DiMap<DiString, MouseMoveCallback> mMouseMoves;
        DiMap<DiString, MousePressCallback> mMousePresses;
        DiMap<DiString, MouseReleaseCallback> mMouseReleases;
        DiMap<DiString, KeyPressCallback> mKeyPresses;
        DiMap<DiString, KeyReleaseCallback> mKeyReleases;

        int mCursorX;
        int mCursorY;
    };

} 

#endif
