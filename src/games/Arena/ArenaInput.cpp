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

#include "ArenaPch.h"

#include "ArenaInput.h"
#include "ArenaGameApp.h"
#include "ArenaGame.h"
#include "RenderWindow.h"
#include "GfxDriver.h"

#include <sstream>

namespace Demi
{
    ArInput::ArInput() :
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

    ArInput::~ArInput()
    {
        DestroyInput();
    }

    void ArInput::CreateInput(DiWndHandle _handle, DiWndViewHandle viewhandle)
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

    void ArInput::DestroyInput()
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

    bool ArInput::mouseMoved(const OIS::MouseEvent& _arg)
    {
        mCursorX += _arg.state.X.rel;
        mCursorY += _arg.state.Y.rel;

        CheckPosition();

        K2MouseEvent event;
        event.state = _arg.state;
        event.type = K2MouseEvent::MOUSE_MOVE;
        ArGameApp::Get()->GetGame()->OnMouseInput(event);
        
        for (auto it = mMouseMoves.begin(); it != mMouseMoves.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    bool ArInput::mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
    {
        K2MouseEvent event;
        event.state = _arg.state;
        event.button = _id;
        event.type = K2MouseEvent::MOUSE_PRESS;
        ArGameApp::Get()->GetGame()->OnMouseInput(event);

        for (auto it = mMousePresses.begin(); it != mMousePresses.end(); ++it)
            if (it->second)
                it->second(_arg, _id);

        return true;
    }

    bool ArInput::mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
    {
        K2MouseEvent event;
        event.state = _arg.state;
        event.button = _id;
        event.type = K2MouseEvent::MOUSE_RELEASE;
        ArGameApp::Get()->GetGame()->OnMouseInput(event);

        for (auto it = mMouseReleases.begin(); it != mMouseReleases.end(); ++it)
            if (it->second)
                it->second(_arg, _id);

        return true;
    }
    
    bool ArInput::keyPressed(const OIS::KeyEvent& _arg)
    {
        K2KeyEvent event;
        event.type = K2KeyEvent::KEY_PRESS;
        event.key = _arg.key;
        event.text = _arg.text;
        ArGameApp::Get()->GetGame()->OnKeyInput(event);
        
        for (auto it = mKeyPresses.begin(); it != mKeyPresses.end(); ++it)
            if (it->second)
                it->second(_arg);
        
        return true;
    }
    
    bool ArInput::keyReleased(const OIS::KeyEvent& _arg)
    {
        K2KeyEvent event;
        event.type = K2KeyEvent::KEY_RELEASE;
        event.key = _arg.key;
        event.text = _arg.text;
        ArGameApp::Get()->GetGame()->OnKeyInput(event);
        
        for (auto it = mKeyReleases.begin(); it != mKeyReleases.end(); ++it)
            if (it->second)
                it->second(_arg);
        
        return true;
    }

#endif

#ifdef DEMI_TOUCH
    bool ArInput::touchMoved(const OIS::MultiTouchEvent &arg)
    {
        mCursorX += arg.state.X.rel;
        mCursorY += arg.state.Y.rel;
        
        K2MouseEvent event;
        event.state = arg.state;
        event.type = K2MouseEvent::MOUSE_MOVE;
        ArGameApp::Get()->GetGame()->OnMouseInput(event);
        
        for (auto it = mMouseMoves.begin(); it != mMouseMoves.end(); ++it)
        if (it->second)
            it->second(arg);
        return true;
    }
    bool ArInput::touchPressed(const OIS::MultiTouchEvent &arg)
    {
        K2MouseEvent event;
        event.state = arg.state;
        event.type = K2MouseEvent::MOUSE_PRESS;
        ArGameApp::Get()->GetGame()->OnMouseInput(event);
        
        for (auto it = mMousePresses.begin(); it != mMousePresses.end(); ++it)
        if (it->second)
            it->second(arg);
        return true;
    }
    bool ArInput::touchReleased(const OIS::MultiTouchEvent &arg)
    {
        K2MouseEvent event;
        event.state = arg.state;
        event.type = K2MouseEvent::MOUSE_RELEASE;
        ArGameApp::Get()->GetGame()->OnMouseInput(event);
        
        for (auto it = mMouseReleases.begin(); it != mMouseReleases.end(); ++it)
        if (it->second)
            it->second(arg);
        return true;
    }
    bool ArInput::touchCancelled(const OIS::MultiTouchEvent &arg)
    {
        return true;
    }
#endif
    
    void ArInput::Update()
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

    void ArInput::SetInputViewSize(int _width, int _height)
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

    void ArInput::SetMousePosition(int _x, int _y)
    {
        mCursorX = _x;
        mCursorY = _y;

        CheckPosition();
    }

    void ArInput::CheckPosition()
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

    void ArInput::UpdateCursorPosition()
    {
    }

    bool ArInput::RegisterMouseMoveEvent(const DiString& name, MouseMoveCallback cb)
    {
        DI_ASSERT(cb);
        if (mMouseMoves.contains(name))
            return false;
        mMouseMoves[name] = cb;
        return true;
    }

    bool ArInput::RegisterMousePressEvent(const DiString& name, MousePressCallback cb)
    {
        DI_ASSERT(cb);
        if (mMousePresses.contains(name))
            return false;
        mMousePresses[name] = cb;
        return true;
    }

    bool ArInput::RegisterMouseReleaseEvent(const DiString& name, MouseReleaseCallback cb)
    {
        DI_ASSERT(cb);
        if (mMouseReleases.contains(name))
            return false;
        mMouseReleases[name] = cb;
        return true;
    }

    bool ArInput::RegisterKeyPressEvent(const DiString& name, KeyPressCallback cb)
    {
        DI_ASSERT(cb);
        if (mKeyPresses.contains(name))
            return false;
        mKeyPresses[name] = cb;
        return true;
    }

    bool ArInput::RegisterKeyReleaseEvent(const DiString& name, KeyReleaseCallback cb)
    {
        DI_ASSERT(cb);
        if (mKeyReleases.contains(name))
            return false;
        mKeyReleases[name] = cb;
        return true;
    }

    bool ArInput::UnregisterMouseMoveEvent(const DiString& name)
    {
        return mMouseMoves.removeKey(name);
    }

    bool ArInput::UnregisterMousePressEvent(const DiString& name)
    {
        return mMousePresses.removeKey(name);
    }

    bool ArInput::UnregisterMouseReleaseEvent(const DiString& name)
    {
        return mMouseReleases.removeKey(name);
    }

    bool ArInput::UnregisterKeyPressEvent(const DiString& name)
    {
        return mKeyPresses.removeKey(name);
    }
    
    bool ArInput::UnregisterKeyReleaseEvent(const DiString& name)
    {
        return mKeyReleases.removeKey(name);
    }
}
