


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

    private:

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        
        LARGE_INTEGER           mStartTime;
        int64                   mTicksPerSec;
        static LARGE_INTEGER    mFrequent;
        
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        
        struct timeval          mStart;
		clock_t                 mZeroClock;
        
#else
#   error "Unsupported platform, no implementation for DiTimer"
#endif
    };
}

#endif
