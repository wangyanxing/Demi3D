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

namespace Demi
{
    class DEMI_DEMO_API DiInputManager : public OIS::MouseListener, public OIS::KeyListener, public DiBase
    {
    public:
        DiInputManager();
        virtual ~DiInputManager();

        typedef std::function<void(const OIS::MouseEvent&)> MouseMoveCallback;
        typedef std::function<void(const OIS::MouseEvent&, OIS::MouseButtonID)> MousePressCallback;
        typedef std::function<void(const OIS::MouseEvent&, OIS::MouseButtonID)> MouseReleaseCallback;
        typedef std::function<void(const OIS::KeyEvent&)> KeyPressCallback;
        typedef std::function<void(const OIS::KeyEvent&)> KeyReleaseCallback;

        void createInput(size_t _handle);
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

#if 0
        void injectMouseMove(int _absx, int _absy, int _absz);
        void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
        void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
        void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
        void injectKeyRelease(MyGUI::KeyCode _key);
#endif

        void setMousePosition(int _x, int _y);
        void updateCursorPosition();

        OIS::Keyboard* getKeyboard() { return mKeyboard; }
        OIS::Mouse* getMouse() { return mMouse; }

    private:
        bool mouseMoved(const OIS::MouseEvent& _arg);
        bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool keyPressed(const OIS::KeyEvent& _arg);
        bool keyReleased(const OIS::KeyEvent& _arg);

        void checkPosition();

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
