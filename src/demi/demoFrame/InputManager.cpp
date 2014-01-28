
#include "DemoPch.h"
#include "InputManager.h"
//#include "MyGUI.h"
#include "Application.h"
#include "RenderWindow.h"
#include "GfxDriver.h"

#include <sstream>

namespace Demi
{
    /*
    MyGUI::Char translateWin32Text(MyGUI::KeyCode kc)
    {
        static WCHAR deadKey = 0;

        BYTE keyState[256];
        HKL  layout = GetKeyboardLayout(0);
        if (GetKeyboardState(keyState) == 0)
            return 0;

        int code = *((int*)&kc);
        unsigned int vk = MapVirtualKeyEx((UINT)code, 3, layout);
        if (vk == 0)
            return 0;

        WCHAR buff[3] = { 0, 0, 0 };
        int ascii = ToUnicodeEx(vk, (UINT)code, keyState, buff, 3, 0, layout);
        if (ascii == 1 && deadKey != '\0')
        {
            // A dead key is stored and we have just converted a character key
            // Combine the two into a single character
            WCHAR wcBuff[3] = { buff[0], deadKey, '\0' };
            WCHAR out[3];

            deadKey = '\0';
            if (FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
                return out[0];
        }
        else if (ascii == 1)
        {
            // We have a single character
            deadKey = '\0';
            return buff[0];
        }
        else if (ascii == 2)
        {
            // Convert a non-combining diacritical mark into a combining diacritical mark
            // Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
            // http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
            switch (buff[0])
            {
            case 0x5E: // Circumflex accent: ?
                deadKey = 0x302;
                break;
            case 0x60: // Grave accent: ?
                deadKey = 0x300;
                break;
            case 0xA8: // Diaeresis: ?
                deadKey = 0x308;
                break;
            case 0xB4: // Acute accent: ?
                deadKey = 0x301;
                break;
            case 0xB8: // Cedilla: ?
                deadKey = 0x327;
                break;
            default:
                deadKey = buff[0];
                break;
            }
        }

        return 0;
    }
    */

    DiInputManager::DiInputManager() :
        mInputManager(0),
        mKeyboard(0),
        mMouse(0),
        mCursorX(0),
        mCursorY(0)
    {
    }

    DiInputManager::~DiInputManager()
    {
        destroyInput();
    }

    void DiInputManager::createInput(size_t _handle)
    {
        std::ostringstream windowHndStr;
        windowHndStr << _handle;

        OIS::ParamList pl;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

        mInputManager = OIS::InputManager::createInputSystem(pl);

        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setEventCallback(this);

        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
        mMouse->setEventCallback(this);
    }

    void DiInputManager::destroyInput()
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
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = nullptr;
        }
    }

    bool DiInputManager::mouseMoved(const OIS::MouseEvent& _arg)
    {
        mCursorX += _arg.state.X.rel;
        mCursorY += _arg.state.Y.rel;

        checkPosition();

        //injectMouseMove(_arg.state.X.abs, _arg.state.Y.abs, _arg.state.Z.abs);
        
        for (auto it = mMouseMoves.begin(); it != mMouseMoves.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    bool DiInputManager::mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
    {
        //injectMousePress(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));

        for (auto it = mMousePresses.begin(); it != mMousePresses.end(); ++it)
            if (it->second)
                it->second(_arg, _id);

        return true;
    }

    bool DiInputManager::mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
    {
        //injectMouseRelease(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
        
        for (auto it = mMouseReleases.begin(); it != mMouseReleases.end(); ++it)
            if (it->second)
                it->second(_arg, _id);

        return true;
    }

    bool DiInputManager::keyPressed(const OIS::KeyEvent& _arg)
    {
        /* MyGUI::Char text = (MyGUI::Char)_arg.text;
         MyGUI::KeyCode key = MyGUI::KeyCode::Enum(_arg.key);
         int scan_code = key.toValue();

         if (scan_code > 70 && scan_code < 84)
         {
         static MyGUI::Char nums[13] = { 55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46 };
         text = nums[scan_code - 71];
         }
         else if (key == MyGUI::KeyCode::Divide)
         {
         text = '/';
         }
         else
         {
         text = translateWin32Text(key);
         }*/

        //injectKeyPress(key, text);

        for (auto it = mKeyPresses.begin(); it != mKeyPresses.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    bool DiInputManager::keyReleased(const OIS::KeyEvent& _arg)
    {
        //injectKeyRelease(MyGUI::KeyCode::Enum(_arg.key));

        for (auto it = mKeyReleases.begin(); it != mKeyReleases.end(); ++it)
            if (it->second)
                it->second(_arg);

        return true;
    }

    void DiInputManager::captureInput()
    {
        if (mMouse)
        {
            mMouse->capture();

            DiRenderWindow* rw = Driver->GetMainRenderWindow();
            setInputViewSize(rw->GetWidth(), rw->GetHeight());
        }
        mKeyboard->capture();
    }

    void DiInputManager::setInputViewSize(int _width, int _height)
    {
        if (mMouse)
        {
            const OIS::MouseState& ms = mMouse->getMouseState();
            if (ms.width != _width || ms.height != _height)
            {
                ms.width = _width;
                ms.height = _height;
                checkPosition();
            }
        }
    }

    void DiInputManager::setMousePosition(int _x, int _y)
    {
        //const OIS::MouseState &ms = mMouse->getMouseState();
        mCursorX = _x;
        mCursorY = _y;

        checkPosition();
    }

    void DiInputManager::checkPosition()
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

    void DiInputManager::updateCursorPosition()
    {
        const OIS::MouseState& ms = mMouse->getMouseState();
        //injectMouseMove(mCursorX, mCursorY, ms.Z.abs);
    }

#if 0
    void DiInputManager::injectMouseMove(int _absx, int _absy, int _absz)
    {
        //MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
    }

    void DiInputManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
    {
        //MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
    }

    void DiInputManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
    {
        //MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
    }

    void DiInputManager::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
    {
        //MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
    }

    void DiInputManager::injectKeyRelease(MyGUI::KeyCode _key)
    {
        //MyGUI::InputManager::getInstance().injectKeyRelease(_key);
    }
#endif

    bool DiInputManager::registerMouseMoveEvent(const DiString& name, MouseMoveCallback cb)
    {
        DI_ASSERT(cb);
        if (mMouseMoves.contains(name))
            return false;
        mMouseMoves[name] = cb;
        return true;
    }

    bool DiInputManager::registerMousePressEvent(const DiString& name, MousePressCallback cb)
    {
        DI_ASSERT(cb);
        if (mMousePresses.contains(name))
            return false;
        mMousePresses[name] = cb;
        return true;
    }

    bool DiInputManager::registerMouseReleaseEvent(const DiString& name, MouseReleaseCallback cb)
    {
        DI_ASSERT(cb);
        if (mMouseReleases.contains(name))
            return false;
        mMouseReleases[name] = cb;
        return true;
    }

    bool DiInputManager::registerKeyPressEvent(const DiString& name, KeyPressCallback cb)
    {
        DI_ASSERT(cb);
        if (mKeyPresses.contains(name))
            return false;
        mKeyPresses[name] = cb;
        return true;
    }

    bool DiInputManager::registerKeyReleaseEvent(const DiString& name, KeyReleaseCallback cb)
    {
        DI_ASSERT(cb);
        if (mKeyReleases.contains(name))
            return false;
        mKeyReleases[name] = cb;
        return true;
    }

    bool DiInputManager::unregisterMouseMoveEvent(const DiString& name)
    {
        return mMouseMoves.removeKey(name);
    }

    bool DiInputManager::unregisterMousePressEvent(const DiString& name)
    {
        return mMousePresses.removeKey(name);
    }

    bool DiInputManager::unregisterMouseReleaseEvent(const DiString& name)
    {
        return mMouseReleases.removeKey(name);
    }

    bool DiInputManager::unregisterKeyPressEvent(const DiString& name)
    {
        return mKeyPresses.removeKey(name);
    }
    
    bool DiInputManager::unregisterKeyReleaseEvent(const DiString& name)
    {
        return mKeyReleases.removeKey(name);
    }
}
