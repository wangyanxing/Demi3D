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
#include "GfxPch.h"
#include "Window.h"

namespace Demi
{
    void DiWindowManager::RegisterWindow(DiWindow* wnd)
    {
        DI_ASSERT(wnd);
        mWnds[wnd->GetWndHandle()] = wnd;
    }

    void DiWindowManager::UnregisterWindow(DiWindow* wnd)
    {
        DI_ASSERT(wnd);
        auto handle = wnd->GetWndHandle();
        auto i = mWnds.find(handle);
        if (i == mWnds.end())
        {
            DI_WARNING("Cannot find the window, failed to unregister");
            return;
        }
        mWnds.erase(i);
    }

    DiWindow* DiWindowManager::GetWindow(DiWndHandle handle)
    {
        auto i = mWnds.find(handle);
        return i == mWnds.end() ? nullptr : i->second;
    }

    DiWindow::~DiWindow()
    {
    }

}