
/********************************************************************
    File:       OSXFunction.h
    Creator:    demiwangya
*********************************************************************/

#ifndef OSXFunction_h__
#define OSXFunction_h__

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
