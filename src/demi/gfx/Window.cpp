
#include "Window.h"

#ifdef WIN32
#   include "Win32Window.h"
#endif

namespace Demi
{
    DiWindow* DiWindow::CreateWnd()
    {
#ifdef WIN32
        return DI_NEW DiWin32Window();
#endif
        DI_ERROR("Unsupported platform, cannot create the window.");
        return nullptr;
    }
}