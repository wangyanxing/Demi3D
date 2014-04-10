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

#include "MiscPch.h"
#include "LogManager.h"
#include "PlatformFunction.h"
#include "PathLib.h"

#include <stdlib.h>

using namespace Demi;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32

void _SetConsoleColor(LogLevel level)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wColor = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

    if (level == LOG_LEVEL_ERROR)
        wColor = FOREGROUND_INTENSITY | FOREGROUND_RED;
    else if (level == LOG_LEVEL_WARNING)
        wColor = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;

    SetConsoleTextAttribute(hConsole, wColor);
}
#else
void _SetConsoleColor(LogLevel){}
#endif

_IMPLEMENT_SINGLETON(DiLogManager)

const char* levelTag[MAX_LOG_LEVEL] =
{        
    "[Error] ",
    "[Warning] ",
    "",
    "[Debug] "
};

DiLogManager::DiLogManager(void)
{
    mVerbosityLevel = LOG_LEVEL_LOG;
}

DiLogManager::~DiLogManager(void)
{
    Shutdown();
}

void DiLogManager::Init(const DiString& logFileName)
{
    mVerbosityLevel = LOG_LEVEL_LOG;

    DiFileLogger* fl = DI_NEW DiFileLogger();
    mLogger.push_back(fl);

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
    DiString logfile = logFileName;
#else
    DiString logfile = DiPathLib::GetApplicationPath() + logFileName;
#endif
    fl->Init(logfile.c_str());

    mLogLevel = (1 << LOG_LEVEL_ERROR)
              | (1 << LOG_LEVEL_WARNING) 
              | (1 << LOG_LEVEL_LOG)
              | (1 << LOG_LEVEL_DEBUG);

    DI_INFO("********************************");
    DI_INFO("***  Demi3D is initializing  ***");
    DI_INFO("********************************");
}

void DiLogManager::Shutdown()
{
    if (!mLogger.empty())
    {
        DI_DELETE mLogger[0];
        mLogger.clear();
    }
}

void DiLogManager::Output(LogLevel level, const char* szFileName,
                        unsigned int uiLine, const char* pacFormat, ... )
{
    const int MAX_CHAR = 10 * 1024;
    static char szLog[MAX_CHAR + 1];
    szLog[MAX_CHAR] = '\0';

    va_list args;
    va_start(args, pacFormat);
    SAFE_VSPRINTF(szLog, MAX_CHAR, pacFormat, args);
    va_end(args);

    OutputUnformat(level, szFileName, uiLine, szLog);
}

void Demi::DiLogManager::RegisterLogger( DiLogger* lg )
{
    mLogger.push_back(lg);
}

void Demi::DiLogManager::UnregisterLogger( DiLogger* lg )
{
    Loggers::iterator it = mLogger.find(lg);
    if (it != mLogger.end())
    {
        mLogger.erase(it);
    }
}

void Demi::DiLogManager::Error( const char* szFileName, unsigned int uiLine, const char* pacFormat, ... )
{
    const int MAX_CHAR = 10 * 1024;
    static char szLog[MAX_CHAR + 1];
    szLog[MAX_CHAR] = '\0';

    va_list args;    
    va_start(args, pacFormat);    
    SAFE_VSPRINTF(szLog, MAX_CHAR, pacFormat, args);
    va_end(args);

    OutputUnformat(LOG_LEVEL_ERROR,szFileName,uiLine,szLog);

    DiString fileName = szFileName;
    DiString lines;
    lines.SetInt(uiLine);
    DiString outstr("A fatal error occured:\n\nFilename:");
    outstr += fileName;
    outstr += "\n\nLine:";
    outstr += lines;
    outstr += "\n\nDescription:";
    outstr += szLog;
    DiPlatformFunc::ErrorDlg(outstr.c_str());

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    __asm int 3
#endif
}

void Demi::DiLogManager::OutputUnformat(LogLevel level, const char* szFileName,
                                unsigned int uiLine, const char* szLog )
{
    if (!mLogLevel & (1<<level))
        return;

    _SetConsoleColor(level);

    printf("%s%s\n", levelTag[level], szLog);

    for (auto it = mLogger.begin(); it != mLogger.end(); ++it)
    {
        DiLogger* lg = *it;
        lg->OutputLog(szLog, levelTag[level], szFileName, uiLine);
    }
}

void Demi::DiLogManager::Output(LogLevel level, const char* log)
{
    _SetConsoleColor(level);

    printf("%s%s\n", levelTag[level], log);

    for (auto it = mLogger.begin(); it != mLogger.end(); ++it)
    {
        (*it)->OutputLog(levelTag[level], log);
    }
}
