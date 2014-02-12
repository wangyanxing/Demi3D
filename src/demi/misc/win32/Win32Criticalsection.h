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

#ifndef Win32Criticalsection_h__
#define Win32Criticalsection_h__

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

#endif // Win32Criticalsection_h__