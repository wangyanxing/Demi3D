
/********************************************************************
    File:       Timer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_MISC_API DiPathLib
    {
    public:
        
#ifdef WIN32
        // For example "D:\Demi\Bin\Debug\Sample_demi.exe"
        static const DiString& GetApplicationFileName();
#endif

        // For example "D:\Demi\Bin\Debug\"
        static const DiString& GetApplicationPath();
    };
}