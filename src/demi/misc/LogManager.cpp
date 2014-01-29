
#include "MiscPch.h"
#include "LogManager.h"
#include "PlatformFunction.h"
#include "PathLib.h"

using namespace Demi;

_IMPLEMENT_SINGLETON(DiLogManager)

const char* levelTag[MAX_LOG_LEVEL] =
{        
    "[ERROR]",
    "[WARNING]",
    "",
    "[DEBUG]"
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

    DiString logfile = DiPathLib::GetApplicationPath() + logFileName;
    fl->Init(logfile.c_str());

    mLogLevel = (1 << LOG_LEVEL_ERROR)
              | (1 << LOG_LEVEL_WARNING) 
              | (1 << LOG_LEVEL_LOG)
              | (1 << LOG_LEVEL_DEBUG);
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
}

void Demi::DiLogManager::OutputUnformat(LogLevel level, const char* szFileName,
                                unsigned int uiLine, const char* szLog )
{
    if (!mLogLevel & (1<<level))
        return;

    printf("%s %s\n", levelTag[level], szLog);

    for (auto it = mLogger.begin(); it != mLogger.end(); ++it)
    {
        DiLogger* lg = *it;
        lg->OutputLog(szLog, levelTag[level], szFileName, uiLine);
    }
}