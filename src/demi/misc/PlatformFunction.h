
/********************************************************************
    File:       PlatformFunction.h
    Creator:    demiwangya
*********************************************************************/

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   include "win32/Win32Function.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX || DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "OSX/OSXFunction.h"
#endif