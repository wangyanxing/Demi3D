
#include "GfxPch.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32

#include "Win32Window.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "Command.h"
#include "GfxDriver.h"

namespace Demi
{
    static const char *gWindowClass = "_DEMI3D_WND";
    static const DWORD gWindowStyle = WS_OVERLAPPEDWINDOW;
    static const DWORD gFullscreenStyle = WS_POPUP;

    static INT_PTR CALLBACK windowProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
    {
        DiWin32Window* window = (DiWin32Window*)LongToPtr(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        DiRenderWindow* renderWnd = DiBase::Driver->FindRenderWindow(DiWndHandle(hwnd));

        if (window && window->DestroyingWindow())
            return 0;

        switch (msg)
        {
        case WM_CREATE:
            ::UpdateWindow(hwnd);
            break;

        case WM_CLOSE:
            DiBase::Driver->MarkClosing();

            window->Close();

            if (window->GetParentRenderWnd())
                window->GetParentRenderWnd()->Closing();

            break;
        case WM_ACTIVATE:
	    {
            bool active = (LOWORD(wParam) != WA_INACTIVE);
            if( active )
            {
                if (renderWnd)
                    renderWnd->GetRenderBuffer()->SetActive(true);
            }
            else
            {
                if (renderWnd)
                    renderWnd->GetRenderBuffer()->SetActive(false);
            }
		    break;
	    }
        //case WM_MOUSEMOVE:
        //    break;

        //case WM_LBUTTONDOWN:
        //    break;

        //case WM_LBUTTONUP:
        //    break;

        //case WM_RBUTTONDOWN:
        //    break;

        //case WM_RBUTTONUP:
        //    break;

        //case WM_KEYDOWN:
        //    break;

        //case WM_KEYUP:
        //    break;

        // for command manager
        case WM_COPYDATA:
        {
            const char* str = (const char*)(((COPYDATASTRUCT*)lParam)->lpData);
            DiString s(str, ((COPYDATASTRUCT*)lParam)->cbData);
            DiBase::CommandMgr->ExecuteCommand(s.c_str());
            break;
        }

        default:
            return ::DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }

    static ATOM registerWindowClass(HINSTANCE hInstance)
    {
        static ATOM atom = 0;
        if (!atom)
        {
            WNDCLASSEX wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = (WNDPROC)windowProc;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(void*);
            wcex.hInstance = hInstance;
            wcex.hIcon = ::LoadIcon(hInstance, (LPCTSTR)0);
            wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName = 0;
            wcex.lpszClassName = gWindowClass;
            wcex.hIconSm = ::LoadIcon(wcex.hInstance, (LPCTSTR)0);
            atom = ::RegisterClassEx(&wcex);
        }
        return atom;
    }

    DiWin32Window::DiWin32Window()
    {
        mHandlingmsg = false;
        mDestroyWindow = false;
    }

    DiWin32Window::~DiWin32Window()
    {
    }

    bool DiWin32Window::Create(uint32& width, uint32& height,
        const char* title, bool fullscreen)
    {
        bool ok = false;
        DI_ASSERT(!mWndHandle);

        registerWindowClass((HINSTANCE)::GetModuleHandle(0));
        RECT winRect;
        winRect.left = 0;
        winRect.top = 0;
        winRect.right = width;
        winRect.bottom = height;
        DWORD dwstyle = (fullscreen ? gFullscreenStyle : gWindowStyle);
        uint32  offset = fullscreen ? 0 : 50;
        ::AdjustWindowRect(&winRect, dwstyle, 0);
        mWndHandle = ::CreateWindowA(gWindowClass, title, dwstyle,
            offset, offset,
            winRect.right - winRect.left, winRect.bottom - winRect.top,
            0, 0, 0, 0);
        DI_ASSERT(mWndHandle);

        if (mWndHandle)
        {
            ok = true;
            ShowWindow((HWND)mWndHandle, SW_SHOWNORMAL);
            ::SetFocus((HWND)mWndHandle);
            SetWindowLongPtr((HWND)mWndHandle, GWLP_USERDATA, PtrToLong(this));
        }

        RAWINPUTDEVICE rawInputDevice;
        rawInputDevice.usUsagePage = 1;
        rawInputDevice.usUsage = 6;
        rawInputDevice.dwFlags = 0;
        rawInputDevice.hwndTarget = NULL;

        BOOL status = RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice));
        if (status != TRUE)
        {
            DI_ERROR("RegisterRawInputDevices failed: %d", GetLastError());
        }

        return ok;
    }

    void DiWin32Window::Update()
    {
        DI_ASSERT(mWndHandle);
        if (IsOpen())
        {
            mHandlingmsg = true;
            MSG msg;

            while (IsOpen() && ::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }

            mHandlingmsg = false;
            if (mWndHandle && mDestroyWindow)
            {
                ::DestroyWindow((HWND)mWndHandle);
                mWndHandle = nullptr;
                mDestroyWindow = false;
            }
        }
    }

    bool DiWin32Window::Close()
    {
        if (mWndHandle)
        {
            if (mHandlingmsg)
                mDestroyWindow = true;
            else
            {
                ::DestroyWindow((HWND)mWndHandle);
                mWndHandle = nullptr;
            }
        }

        return true;
    }

    bool DiWin32Window::IsOpen()
    {
        return mWndHandle ? true : false;
    }

    void DiWin32Window::SetWindowSize(uint32 width, uint32 height)
    {
        bool fullscreen = false;
        DI_ASSERT(mWndHandle);
        if (mWndHandle)
        {
            RECT rect;
            ::GetWindowRect((HWND)mWndHandle, &rect);
            rect.right = (LONG)(rect.left + width);
            rect.bottom = (LONG)(rect.top + height);
            RECT oldrect = rect;
            DWORD dwstyle = (fullscreen ? gFullscreenStyle : gWindowStyle);
            ::AdjustWindowRect(&rect, dwstyle, 0);
            ::MoveWindow((HWND)mWndHandle, (int)oldrect.left, (int)oldrect.top,
                (int)(rect.right - rect.left), (int)(rect.bottom - rect.top), 1);
        }
    }

    void DiWin32Window::GetWindowSize(uint32& width, uint32& height)
    {
        if (mWndHandle)
        {
            RECT rect;
            GetClientRect((HWND)mWndHandle, &rect);
            width = (uint32)(rect.right - rect.left);
            height = (uint32)(rect.bottom - rect.top);
        }
    }

    void DiWin32Window::GetTitle(char *title, uint32 maxLength) const
    {
        DI_ASSERT(mWndHandle);
        if (mWndHandle)
            GetWindowTextA((HWND)mWndHandle, title, maxLength);
    }

    void DiWin32Window::SetTitle(const char *title)
    {
        DI_ASSERT(mWndHandle);
        if (mWndHandle)
            ::SetWindowTextA((HWND)mWndHandle, title);
    }
}

#endif