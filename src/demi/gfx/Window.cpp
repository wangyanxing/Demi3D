
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
}