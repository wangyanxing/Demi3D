
/********************************************************************
    File:       Win32Interlocked.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_MISC_API InterlockedWin32
    {
    public:
        
        inline static int Increment(int volatile& var)
        {
            return InterlockedIncrement((volatile LONG*)&var);
        }
        
        inline static int Decrement(int volatile& var)
        {
            return InterlockedDecrement((volatile LONG*)&var);
        }

        inline static void Add(int volatile& var, int add)
        {
            InterlockedExchangeAdd((volatile LONG*)&var, add);
        }
    };
}