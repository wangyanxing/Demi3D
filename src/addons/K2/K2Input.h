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

#if 0
#   include "MyGUI_KeyCode.h"
#   include "MyGUI_MouseButton.h"
#   include "MyGUI_Types.h"
#endif

#include <functional>
#include "OIS.h"

namespace Demi
{
    struct DEMI_K2_API K2MouseEvent
    {
        enum Type
        {
            MOUSE_MOVE,
            MOUSE_PRESS,
            MOUSE_RELEASE
        };
        OIS::MouseState state;
        OIS::MouseButtonID button;
        Type type;
    };

    struct DEMI_K2_API K2KeyEvent
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

    class DEMI_K2_API DiK2Input : public OIS::MouseListener, public OIS::KeyListener, public DiBase
    {
    public:
        DiK2Input();

        ~DiK2Input();

        typedef std::function<void(const OIS::MouseEvent&)> MouseMoveCallback;
        typedef std::function<void(const OIS::MouseEvent&, OIS::MouseButtonID)> MousePressCallback;
        typedef std::function<void(const OIS::MouseEvent&, OIS::MouseButtonID)> MouseReleaseCallback;
        typedef std::function<void(const OIS::KeyEvent&)> KeyPressCallback;
        typedef std::function<void(const OIS::KeyEvent&)> KeyReleaseCallback;

        void CreateInput(size_t _handle);
        
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

    private:

        bool mouseMoved(const OIS::MouseEvent& _arg);
        
        bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        
        bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        
        bool keyPressed(const OIS::KeyEvent& _arg);

        bool keyReleased(const OIS::KeyEvent& _arg);

        void CheckPosition();

    private:
        OIS::InputManager* mInputManager;
        OIS::Keyboard* mKeyboard;
        OIS::Mouse* mMouse;

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
