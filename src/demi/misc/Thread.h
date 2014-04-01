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

#ifndef DiThread_h__
#define DiThread_h__


#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   include <Windows.h>
#   include "Win32/Win32Thread.h"
#   include "Win32/Win32Event.h"
#   include "Win32/Win32Criticalsection.h"
#   include "Win32/Win32Interlocked.h"

namespace Demi
{
    class DI_MISC_API DiThread : public DiTheradWin32{};
}

#elif (DEMI_PLATFORM == DEMI_PLATFORM_OSX)

namespace Demi
{
    class DI_MISC_API DiThread {};
}

#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS)

namespace Demi
{
    class DI_MISC_API DiThread {};
}

#else

#   error "The thread lib is not supported on this platform yet."

#endif


#endif
