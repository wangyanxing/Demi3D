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

#ifndef DiWin32EGLWindow_h__
#define DiWin32EGLWindow_h__

#include "EGLWindow.h"
#include "Win32EGLUtil.h"

namespace Demi
{
    class DI_GLES2_API DiWin32EGLWindow : public DiEGLWindow
    {
    public:

        DiWin32EGLWindow(DiWin32EGLUtil* util);

        virtual ~DiWin32EGLWindow();

    public:

        virtual bool        Create(uint32& width, uint32& height,
            const DiString& title, bool fullscreen);

        virtual void        Update();

        virtual bool        Close();

        virtual bool        IsOpen();

        virtual void        SetWindowSize(uint32 width, uint32 height);

        virtual void        GetWindowSize(uint32& width, uint32& height);

        virtual void        GetTitle(char *title, uint32 maxLength) const;

        virtual void        SetTitle(const char *title);

    protected:

        virtual DiEGLContext* CreateEGLContext() const;

        virtual bool        _Create(uint32& width, uint32& height,
            const DiString& title, bool fullscreen);

    protected:

        bool                mHandlingmsg;

        HDC                 mHDC;
    };
}
#endif 