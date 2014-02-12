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

#ifndef DiWindow_h__
#define DiWindow_h__


namespace Demi
{
    class DI_GFX_API DiWindow : public DiBase
    {
    public:

        DiWindow() : mWndHandle(nullptr), mDestroyWindow(false), 
                     mParentWnd(nullptr), mVSync(false){}

        virtual             ~DiWindow() {}

    public:

        virtual bool        Create(uint32& width, uint32& height,
                                const DiString& title, bool fullscreen) = 0;

        virtual void        Update() = 0;

        virtual bool        Close()  = 0;

        virtual bool        IsOpen() = 0;

        virtual void        SetVSync(bool vsync) {}

        bool                IsVSync() const { return mVSync; }

        bool                DestroyingWindow() const { return mDestroyWindow; }

        void                SetParentRenderWnd(DiRenderWindow* parent) { mParentWnd = parent; }

        DiRenderWindow*     GetParentRenderWnd() { return mParentWnd; }

        DiWndHandle         GetWndHandle() { return mWndHandle; }

        virtual void        SetWindowSize(uint32 width, uint32 height) = 0;

        virtual void        GetWindowSize(uint32& width, uint32& height) = 0;

        virtual void        GetTitle(char *title, uint32 maxLength) const = 0;

        virtual void        SetTitle(const char *title) = 0;
        
        virtual void        OnMoveOrResize() {}
        
        virtual void        SwapBuffers() = 0;

    protected:

        DiWndHandle         mWndHandle;

        DiRenderWindow*     mParentWnd;

        bool                mDestroyWindow;

        bool	            mVSync;
    };

    class DI_GFX_API DiWindowManager
    {
    public:

        DiWindowManager(){}

        ~DiWindowManager(){}

    public:

        void                RegisterWindow(DiWindow* wnd);

        void                UnregisterWindow(DiWindow* wnd);

        DiWindow*           GetWindow(DiWndHandle handle);

    private:

        DiMap<DiWndHandle, DiWindow*> mWnds;
    };
}

#endif
