
/********************************************************************
    File:       Thread.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   include <Windows.h>
#   include "Win32Thread.h"
#   include "Win32Event.h"
#   include "Win32Criticalsection.h"
#   include "Win32Interlocked.h"

namespace Demi
{
    class DI_MISC_API DiThread : public DiTheradWin32{};
}

#elif (DEMI_PLATFORM == DEMI_PLATFORM_OSX)

namespace Demi
{
    class DI_MISC_API DiThread {};
}

#else

#   error "The thread lib is not supported on this platform yet."

#endif
