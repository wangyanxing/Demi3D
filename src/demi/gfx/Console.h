
/********************************************************************
    File:       Console.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32

#include "Logger.h"

namespace Demi
{
    struct DiLogInfo
    {
        DiLogInfo()
        {
            type = 0;
            hwnd = 0;
        }
        ulong       type;
        DiWndHandle hwnd;
        char message[4096];
    };

    class DI_GFX_API DiConsoleLogger : public DiLogger, public DiBase
    {
    public:

        DiConsoleLogger(bool createconsole);

        DiConsoleLogger(bool createconsole, const DiString& consoleFile);

        ~DiConsoleLogger();
        
    public:

        static DiString s_sConsoleWndName;

        void        OutputLog(const char* msg, const char* levelInfo, const char* fileName, long line);

        void        OutputLog(const char* msg, const char* levelInfo);

        void        CreateConsoleWindow();

    protected:

        void        Init(bool createConsole);

        DiString    mConsoleFile;

        HWND        mWnd;
    };
}

#endif