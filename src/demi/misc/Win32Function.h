
/********************************************************************
    File:       Win32Function.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_MISC_API DiPlatformFunc
    {
    public:

        static void ErrorDlg(const char* msg);

        static bool AssertDlg(const char* msg, const char* file, int line, bool& ignore);
    };
}