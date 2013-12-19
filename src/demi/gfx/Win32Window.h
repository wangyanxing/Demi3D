
/********************************************************************
    File:       Win32Window.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#ifdef WIN32

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
                        const char* title, bool fullscreen);

        void        Update();

        bool        Close();

        bool        IsOpen();

        void        SetWindowSize(uint32 width, uint32 height);

        void        GetWindowSize(uint32& width, uint32& height);

        void        GetTitle(char *title, uint32 maxLength) const;

        void        SetTitle(const char *title);

    private:

        bool        mHandlingmsg;
    };
}

#endif