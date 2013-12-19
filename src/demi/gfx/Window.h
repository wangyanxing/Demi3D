
/********************************************************************
    File:       Window.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiWindow : public DiBase
    {
    public:

        DiWindow() : mWndHandle(nullptr), mDestroyWindow(false), mParentWnd(nullptr){}

        virtual             ~DiWindow() {}

    public:

        static DiWindow*    CreateWnd();

    public:

        virtual bool        Create(uint32& width, uint32& height,
                                const char* title, bool fullscreen) = 0;

        virtual void        Update() = 0;

        virtual bool        Close()  = 0;

        virtual bool        IsOpen() = 0;

        bool                DestroyingWindow() const { return mDestroyWindow; }

        void                SetParentRenderWnd(DiRenderWindow* parent) { mParentWnd = parent; }

        DiRenderWindow*     GetParentRenderWnd() { return mParentWnd; }

        DiWndHandle         GetWndHandle() { return mWndHandle; }

        virtual void        SetWindowSize(uint32 width, uint32 height) = 0;

        virtual void        GetWindowSize(uint32& width, uint32& height) = 0;

        virtual void        GetTitle(char *title, uint32 maxLength) const = 0;

        virtual void        SetTitle(const char *title) = 0;

    protected:

        DiWndHandle         mWndHandle;

        DiRenderWindow*     mParentWnd;

        bool                mDestroyWindow;
    };
}