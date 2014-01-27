
#include "MiscPch.h"
#include "Timer.h"

namespace Demi 
{
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

    double DiTimer::GetCurTime()
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        __int64 nowtime = (__int64)now.QuadPart;
        return (double)nowtime / (double)mTicksPerSec;
    }

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
            DI_ERROR("Unable to use high performace timer!");
        }
    }

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

}