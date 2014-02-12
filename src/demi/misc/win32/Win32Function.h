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

#ifndef Win32Function_h__
#define Win32Function_h__

#include "MiscPrerequisites.h"

namespace Demi
{
    class DI_MISC_API DiPlatformFunc
    {
    public:

        static void ErrorDlg(const char* msg);

        static bool AssertDlg(const char* msg, const char* file, int line, bool& ignore);
    };
}

#endif // !Win32Function_h__
