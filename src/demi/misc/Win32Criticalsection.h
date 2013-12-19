
/********************************************************************
    File:       Win32Criticalsection.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_MISC_API DiCriticalSectionWin32
    {
    public:
        
        DiCriticalSectionWin32()
        {
            InitializeCriticalSectionAndSpinCount(&mCriticalSection, 1024);
        }
        
        ~DiCriticalSectionWin32()
        {
            DeleteCriticalSection(&mCriticalSection);
        }
        
        inline void Enter() const
        {
            EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&mCriticalSection));
        }
        
        inline void Leave() const
        {
            LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&mCriticalSection));
        }

    private:
        CRITICAL_SECTION mCriticalSection;
    };
}