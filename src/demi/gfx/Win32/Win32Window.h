


#ifndef DiWin32Window_h__
#define DiWin32Window_h__


#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32

#include "Window.h"

namespace Demi
{
    class DI_GFX_API DiWin32Window : public DiWindow
    {
    public:

        DiWin32Window();

        ~DiWin32Window();

    public:

        bool        Create(uint32& width, uint32& height,
                        const DiString& title, bool fullscreen);

        void        Update();

        bool        Close();

        bool        IsOpen();

        void        SetWindowSize(uint32 width, uint32 height);

        void        GetWindowSize(uint32& width, uint32& height);

        void        GetTitle(char *title, uint32 maxLength) const;

        void        SetTitle(const char *title);

        void        SwapBuffers();

    private:

        bool        mHandlingmsg;

        HDC         mHDC;
    };
}

#endif

#endif
