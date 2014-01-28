
#include "MiscPch.h"
#include "Timer.h"

namespace Demi 
{
    const DiString&  DiTimer::GetCurrentDateTime()
    {
        static DiString dt;
        
        char datetime[128];
        struct tm *curtime;
        time_t t;
        t = time(NULL);
        curtime = localtime(&t);
        strftime(datetime, 128, "%Y-%m-%d %X", curtime);
        dt = datetime;
        return dt;
    }

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    
#include <windows.h>
    
    DiTimer::DiTimer()
    {
        LARGE_INTEGER frequent;
        
        if (QueryPerformanceFrequency(&frequent))
        {
            mTicksPerSec = frequent.QuadPart;
            Start();
        }
        else
        {
            DI_WARNING("Unable to use high performace timer!");
        }
    }
    
    void DiTimer::Start()
    {
        QueryPerformanceCounter( &mStartTime);
    }

    double DiTimer::GetElapse()
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        __int64 elapse64 = (__int64)now.QuadPart - (__int64)mStartTime.QuadPart;
        return (double)elapse64 / (double)mTicksPerSec;
    }

#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
    
#include <sys/time.h>
    
    DiTimer::DiTimer()
    {
        Start();
    }
    
    void DiTimer::Start()
    {
        mZeroClock = clock();
        gettimeofday(&mStart, NULL);
    }
    
    double DiTimer::GetElapse()
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        return (now.tv_sec-mStart.tv_sec) + (now.tv_usec-mStart.tv_usec)/1000000.0;
    }

#endif
}