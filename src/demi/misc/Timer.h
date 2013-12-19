
/********************************************************************
    File:       Timer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <time.h>

namespace Demi 
{
    class DI_MISC_API DiTimer
    {
    public:

        DiTimer();

        void                    Start();

        double                  GetElapse();

        double                  GetCurTime();

        static const DiString&  GetCurrentDateTime();

    private:

        LARGE_INTEGER           mStartTime;        

        int64                   mTicksPerSec;

        static LARGE_INTEGER    mFrequent;    
    };
}