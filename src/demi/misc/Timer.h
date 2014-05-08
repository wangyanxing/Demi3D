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

#ifndef DiTimer_h__
#define DiTimer_h__

#include <time.h>

namespace Demi 
{
    class DI_MISC_API DiTimer
    {
    public:

        DiTimer();

        void                    Start();

        double                  GetElapse();

        static const DiString&  GetCurrentTime();

        static const DiString&  GetCurrentDateTime();

    private:

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        
        LARGE_INTEGER           mStartTime;
        int64                   mTicksPerSec;
        static LARGE_INTEGER    mFrequent;
        
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX || DEMI_PLATFORM == DEMI_PLATFORM_IOS
        
        struct timeval          mStart;
		clock_t                 mZeroClock;
        
#else
#   error "Unsupported platform, no implementation for DiTimer"
#endif
    };
}

#endif
