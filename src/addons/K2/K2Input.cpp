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

#include "K2Pch.h"

#include "K2Input.h"
#include "K2GameApp.h"
#include "K2Game.h"
#include "RenderWindow.h"
#include "GfxDriver.h"

#include <sstream>

namespace Demi
{
    DiK2Input::DiK2Input() :
        mInputManager(0),
        mKeyboard(0),
        mMouse(0),
        mCursorX(0),
        mCursorY(0)
#if OIS_WITH_MULTITOUCH
        , mMultiTouch(nullptr)
#endif
        , mAccelerometer(nullptr)
    {
    }

    DiK2Input::~DiK2Input()
    {
        DestroyInput();
    }

    void DiK2Input::CreateInput(DiWndHandle _handle, DiWndViewHandle viewhandle)
    {
        std::ostringstream windowHndStr;
        windowHndStr << (ulong)_handle;

        OIS::ParamList pl;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_FOREGROUND"));
        pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_NONEXCLUSIVE"));

#if (DEMI_PLATFORM == DEMI_PLATFORM_IOS)
        // Pass the view to OIS so the contentScalingFactor can be used
        std::ostringstream viewHandleStr;
        viewHandleStr << (ulong)viewhandle;
        pl.insert(std::make_pair("VIEW", viewHandleStr.str()));
#endif

        mInputManager = OIS::InputManager::createInputSystem(pl);

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        mMultiTouch = static_cast<OIS::MultiTouch*>(mInputManager->createInputObject(OIS::OISMultiTouch, true));
        mMultiTouch->setEventCallback(this);

        mAccelerometer = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, true));

#elif DEMI_PLATFORM == DEMI_PLATFORM_WIN32 || DEMI_PLATFORM == DEMI_PLATFORM_OSX
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setEventCallback(this);

        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
        mMouse->setEventCallback(this);
#endif
    }

    void DiK2Input::DestroyInput()
    {
        if (mInputManager)
        {
            if (mMouse)
            {
                mInputManager->destroyInputObject(mMouse);
                mMouse = nullptr;
            }
            if (mKeyboard)
            {
                mInputManager->destroyInputObject(mKeyboard);
                mKeyboard = nullptr;
            }
#if OIS_WITH_MULTITOUCH
            if (mMultiTouch)
            {
                mInputManager->destroyInputObject(mMultiTouch);
                mMultiTouch = nullptr;
            }
#endif
            if (mAccelerometer)
            {
                mInputManager->destroyInputObject(mAccelerometer);
                mAccelerometer = nullptr;
            }
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = nullptr;
        }
    }

#ifdef DEMI_KEYMOUSE

    bool DiK2Input::mouseMoved(const OIS::MouseEvent& _arg)
    {
        mCursorX += _arg.state.X.rel;
        mCursorY += _arg.state.Y.rel;

        CheckPosition();

        K2MouseEvent event;
        event.state = _arg.state;
        event.type = K2MouseEvent::MOUSE_MOVE;
        DiK2GameApp::Get()->GetGame()->OnMouseInput(event);
        
        for (auto it = mMouseMoves.begin(); it != mMouseMoves.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    bool DiK2Input::mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
    {
        K2MouseEvent event;
        event.state = _arg.state;
        event.button = _id;
        event.type = K2MouseEvent::MOUSE_PRESS;
        DiK2GameApp::Get()->GetGame()->OnMouseInput(event);

        for (auto it = mMousePresses.begin(); it != mMousePresses.end(); ++it)
            if (it->second)
                it->second(_arg, _id);

        return true;
    }

    bool DiK2Input::mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
    {
        K2MouseEvent event;
        event.state = _arg.state;
        event.button = _id;
        event.type = K2MouseEvent::MOUSE_RELEASE;
        DiK2GameApp::Get()->GetGame()->OnMouseInput(event);

        for (auto it = mMouseReleases.begin(); it != mMouseReleases.end(); ++it)
            if (it->second)
                it->second(_arg, _id);

        return true;
    }

#endif

#ifdef DEMI_TOUCH
    bool DiK2Input::touchMoved(const OIS::MultiTouchEvent &arg)
    {
        for (auto it = mMouseMoves.begin(); it != mMouseMoves.end(); ++it)
        if (it->second)
            it->second(arg);
        return true;
    }
    bool DiK2Input::touchPressed(const OIS::MultiTouchEvent &arg)
    {
        for (auto it = mMousePresses.begin(); it != mMousePresses.end(); ++it)
        if (it->second)
            it->second(arg);
        return true;
    }
    bool DiK2Input::touchReleased(const OIS::MultiTouchEvent &arg)
    {
        for (auto it = mMouseReleases.begin(); it != mMouseReleases.end(); ++it)
        if (it->second)
            it->second(arg);
        return true;
    }
    bool DiK2Input::touchCancelled(const OIS::MultiTouchEvent &arg)
    {
        return true;
    }
#endif

    bool DiK2Input::keyPressed(const OIS::KeyEvent& _arg)
    {
        K2KeyEvent event;
        event.type = K2KeyEvent::KEY_PRESS;
        event.key = _arg.key;
        event.text = _arg.text;
        DiK2GameApp::Get()->GetGame()->OnKeyInput(event);

        for (auto it = mKeyPresses.begin(); it != mKeyPresses.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    bool DiK2Input::keyReleased(const OIS::KeyEvent& _arg)
    {
        K2KeyEvent event;
        event.type = K2KeyEvent::KEY_RELEASE;
        event.key = _arg.key;
        event.text = _arg.text;
        DiK2GameApp::Get()->GetGame()->OnKeyInput(event);

        for (auto it = mKeyReleases.begin(); it != mKeyReleases.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    void DiK2Input::Update()
    {
        if (mMouse)
        {
            mMouse->capture();
            DiRenderWindow* rw = Driver->GetMainRenderWindow();
            SetInputViewSize(rw->GetWidth(), rw->GetHeight());
        }
        if (mKeyboard)
            mKeyboard->capture();

#if OIS_WITH_MULTITOUCH
        if (mMultiTouch)
            mMultiTouch->capture();
#endif
        if (mAccelerometer)
            mAccelerometer->capture();
    }

    void DiK2Input::SetInputViewSize(int _width, int _height)
    {
        if (mMouse)
        {
            const OIS::MouseState& ms = mMouse->getMouseState();
            if (ms.width != _width || ms.height != _height)
            {
                ms.width = _width;
                ms.height = _height;
                CheckPosition();
            }
        }
    }

    void DiK2Input::SetMousePosition(int _x, int _y)
    {
        mCursorX = _x;
        mCursorY = _y;

        CheckPosition();
    }

    void DiK2Input::CheckPosition()
    {
        const OIS::MouseState& ms = mMouse->getMouseState();

        if (mCursorX < 0)
            mCursorX = 0;
        else if (mCursorX >= ms.width)
            mCursorX = ms.width - 1;

        if (mCursorY < 0)
            mCursorY = 0;
        else if (mCursorY >= ms.height)
            mCursorY = ms.height - 1;
    }

    void DiK2Input::UpdateCursorPosition()
    {
    }

    bool DiK2Input::RegisterMouseMoveEvent(const DiString& name, MouseMoveCallback cb)
    {
        DI_ASSERT(cb);
        if (mMouseMoves.contains(name))
            return false;
        mMouseMoves[name] = cb;
        return true;
    }

    bool DiK2Input::RegisterMousePressEvent(const DiString& name, MousePressCallback cb)
    {
        DI_ASSERT(cb);
        if (mMousePresses.contains(name))
            return false;
        mMousePresses[name] = cb;
        return true;
    }

    bool DiK2Input::RegisterMouseReleaseEvent(const DiString& name, MouseReleaseCallback cb)
    {
        DI_ASSERT(cb);
        if (mMouseReleases.contains(name))
            return false;
        mMouseReleases[name] = cb;
        return true;
    }

    bool DiK2Input::RegisterKeyPressEvent(const DiString& name, KeyPressCallback cb)
    {
        DI_ASSERT(cb);
        if (mKeyPresses.contains(name))
            return false;
        mKeyPresses[name] = cb;
        return true;
    }

    bool DiK2Input::RegisterKeyReleaseEvent(const DiString& name, KeyReleaseCallback cb)
    {
        DI_ASSERT(cb);
        if (mKeyReleases.contains(name))
            return false;
        mKeyReleases[name] = cb;
        return true;
    }

    bool DiK2Input::UnregisterMouseMoveEvent(const DiString& name)
    {
        return mMouseMoves.removeKey(name);
    }

    bool DiK2Input::UnregisterMousePressEvent(const DiString& name)
    {
        return mMousePresses.removeKey(name);
    }

    bool DiK2Input::UnregisterMouseReleaseEvent(const DiString& name)
    {
        return mMouseReleases.removeKey(name);
    }

    bool DiK2Input::UnregisterKeyPressEvent(const DiString& name)
    {
        return mKeyPresses.removeKey(name);
    }
    
    bool DiK2Input::UnregisterKeyReleaseEvent(const DiString& name)
    {
        return mKeyReleases.removeKey(name);
    }
}
