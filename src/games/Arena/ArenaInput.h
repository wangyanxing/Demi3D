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

#ifndef DiK2Input_h__
#define DiK2Input_h__

#include "K2Prerequisites.h"

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
    struct K2MouseEvent
    {
        enum Type
        {
            MOUSE_MOVE,
            MOUSE_PRESS,
            MOUSE_RELEASE
        };
#ifdef DEMI_KEYMOUSE
        OIS::MouseState state;
        OIS::MouseButtonID button;
#elif defined(DEMI_TOUCH)
        OIS::MultiTouchState state;
#endif
        Type type;
    };

    struct K2KeyEvent
    {
        enum Type
        {
            KEY_PRESS,
            KEY_RELEASE
        };
        OIS::KeyCode key;
        uint32 text;
        Type type;
    };

    class ArInput : public DiBase
#ifdef DEMI_KEYMOUSE
        , public OIS::MouseListener, public OIS::KeyListener
#endif
#ifdef DEMI_TOUCH
        , public OIS::MultiTouchListener
#endif
    {
    public:
        ArInput();

        ~ArInput();

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

        void CreateInput(DiWndHandle _handle, DiWndViewHandle viewhandle);
        
        void DestroyInput();
        
        void Update();

        void SetInputViewSize(int _width, int _height);

        bool RegisterMouseMoveEvent(const DiString& name, MouseMoveCallback cb);

        bool RegisterMousePressEvent(const DiString& name, MousePressCallback cb);
        
        bool RegisterMouseReleaseEvent(const DiString& name, MouseReleaseCallback cb);
        
        bool RegisterKeyPressEvent(const DiString& name, KeyPressCallback cb);
        
        bool RegisterKeyReleaseEvent(const DiString& name, KeyReleaseCallback cb);

        bool UnregisterMouseMoveEvent(const DiString& name);
        
        bool UnregisterMousePressEvent(const DiString& name);
        
        bool UnregisterMouseReleaseEvent(const DiString& name);
        
        bool UnregisterKeyPressEvent(const DiString& name);
        
        bool UnregisterKeyReleaseEvent(const DiString& name);

        void SetMousePosition(int _x, int _y);

        void UpdateCursorPosition();

        OIS::Keyboard* GetKeyboard() { return mKeyboard; }

        OIS::Mouse* GetMouse() { return mMouse; }

        static DiRay GetPickupRay(const K2MouseEvent& event);

    private:

#ifdef DEMI_KEYMOUSE
        bool mouseMoved(const OIS::MouseEvent& _arg);
        bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool keyPressed(const OIS::KeyEvent& _arg);
        bool keyReleased(const OIS::KeyEvent& _arg);
#endif

#ifdef DEMI_TOUCH
        bool touchMoved(const OIS::MultiTouchEvent &arg);
        bool touchPressed(const OIS::MultiTouchEvent &arg);
        bool touchReleased(const OIS::MultiTouchEvent &arg);
        bool touchCancelled(const OIS::MultiTouchEvent &arg);
#endif

        void CheckPosition();

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
