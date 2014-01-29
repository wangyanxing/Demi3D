
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
        
        // For example "D:\Demi\Bin\Debug\Sample_demi.exe"
        static const DiString& GetApplicationFileName();

        // For example "D:\Demi\Bin\Debug\"
        static const DiString& GetApplicationPath();

        static DiString        msAppFile;
    };
}