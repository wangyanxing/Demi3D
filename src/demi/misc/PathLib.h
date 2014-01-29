
/********************************************************************
    File:       Timer.h
    Creator:    demiwangya
*********************************************************************/

#ifndef PathLib_h__
#define PathLib_h__

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

#endif // PathLib_h__