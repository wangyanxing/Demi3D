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

#ifndef Win32Event_h__
#define Win32Event_h__

namespace Demi
{
    class DI_MISC_API DiEventWin32
    {
    public:

        DiEventWin32()
        {
            mEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
            DI_ASSERT(mEvent != 0);
        }

        ~DiEventWin32()
        {
            DI_ASSERT(mEvent != 0);
            CloseHandle(mEvent);
            mEvent = 0;
        }

    public:

        inline void Signal()
        {
            DI_ASSERT(mEvent != 0);
            SetEvent(mEvent);
        }

        inline void Wait() const
        {
            DI_ASSERT(mEvent != 0);
            WaitForSingleObject(mEvent, INFINITE);
        }
        
        inline bool WaitTimeout(int timeoutInMilliSec) const
        {
            DI_ASSERT(mEvent != 0);
            DWORD res = WaitForSingleObject(mEvent, timeoutInMilliSec);
            return (WAIT_TIMEOUT == res) ? false : true;
        }
        
        // check if event is signalled
        inline bool Peek() const
        {
            DI_ASSERT(mEvent != 0);
            DWORD res = WaitForSingleObject(mEvent, 0);
            return (WAIT_TIMEOUT == res) ? false : true;
        }

    private:
        HANDLE mEvent;
    };
}

#endif // Win32Event_h__