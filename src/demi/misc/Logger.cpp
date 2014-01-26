#include "Logger.h"
#include "Timer.h"

using namespace Demi;

DiFileLogger::DiFileLogger()
{
}

DiFileLogger::~DiFileLogger()
{
    Shutdown();
}

bool DiFileLogger::Init(const char* szFileName)
{
    DI_ASSERT(szFileName);
    if (!szFileName) 
        return false;

    mFile.open(szFileName);
    if (mFile)
        return true;

    return false;
}

void DiFileLogger::Shutdown()
{
}

void DiFileLogger::OutputLog(const char* szMessage, const char* levelInfo,
                           const char* fileName, long line)
{
    DiString datetime = DiTimer::GetCurrentDateTime();

    DiString logPre;

    DiString file = fileName;
    file = file.ExtractFileName();
    DiString log;
    log.Format(
        "%s %s(%d)\r\n"
        "%s : %s\r\n\r\n", 
        datetime.c_str(),
        file.c_str(), line,
        levelInfo, szMessage);

    log = logPre + log;

    mFile << log.c_str();
}