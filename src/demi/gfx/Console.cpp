
#ifdef WIN32

#include "Console.h"
#include "GfxDriver.h"
#include "PathLib.h"

namespace Demi
{
    Demi::DiString DiConsoleLogger::s_sConsoleWndName = "The console of PhiloTrinity";

    DiConsoleLogger::DiConsoleLogger(bool createconsole):DiLogger()
        ,mWnd(NULL)
    {
        mConsoleFile = DiPathLib::GetApplicationPath() + "Console.exe";

        Init(createconsole);
    }

    DiConsoleLogger::DiConsoleLogger( bool createconsole, const DiString& consoleFile ):DiLogger()
        ,mWnd(NULL),mConsoleFile(consoleFile)
    {
        Init(createconsole);
    }

    DiConsoleLogger::~DiConsoleLogger()
    {

    }

    void DiConsoleLogger::OutputLog( const char* szMessage, const char* levelInfo, const char*, long )
    {
        static DiLogInfo logInfo;
        if (::IsWindow(mWnd))
        {
            DiString msg = szMessage;
            DiString lv = levelInfo;

            logInfo.type = LOG_LEVEL_LOG;
            if (lv == "error")
                logInfo.type = LOG_LEVEL_ERROR;
            else if (lv == "info")
                logInfo.type = LOG_LEVEL_LOG;
            else if (lv == "debug")
                logInfo.type = LOG_LEVEL_DEBUG;
            else if (lv == "warning")
                logInfo.type = LOG_LEVEL_WARNING;
            else
                logInfo.type = 0xFFFF;

            if (Driver)
                logInfo.hwnd = mMainHwnd;

             ZeroMemory(logInfo.message, sizeof(logInfo.message));
             strcpy_s(logInfo.message, 4096, msg.c_str());
             COPYDATASTRUCT cpd;
             cpd.dwData = 0;
             cpd.cbData = sizeof(logInfo);
             cpd.lpData = &logInfo;
             ::SendMessage(mWnd, WM_COPYDATA, 0, (LPARAM)(LPVOID)&cpd);
        }
    }

    void DiConsoleLogger::Init( bool createConsole )
    {
        mWnd = ::FindWindowA(NULL,s_sConsoleWndName.c_str());
        if (NULL == mWnd)
        {
            if (createConsole)
            {
                ::WinExec(mConsoleFile.c_str(), SW_SHOW);
                mWnd = ::FindWindowA(NULL,s_sConsoleWndName.c_str());
            }
        }
    }

    void DiConsoleLogger::CreateConsoleWindow()
    {
        mWnd = ::FindWindowA(NULL,s_sConsoleWndName.c_str());
        if (NULL == mWnd)
        {
            ::WinExec(mConsoleFile.c_str(), SW_SHOW);
            mWnd = ::FindWindowA(NULL,s_sConsoleWndName.c_str());
        }
    }
}

#endif