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

#ifndef Win32Interlocked_h__
#define Win32Interlocked_h__

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
#endif // Win32Interlocked_h__