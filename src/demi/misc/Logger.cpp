
#include "MiscPch.h"
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
    DiString datetime = DiTimer::GetCurrentTime();

    DiString log;
    log.Format( "[%s] %s%s", datetime.c_str(), levelInfo, szMessage);
    mFile << log.c_str();

    if (fileName)
    {
        DiString file = fileName;
        file = file.ExtractFileName();
        mFile << ", " << file.c_str() << "(" << line << ")";
    }

    mFile << std::endl;
}

void Demi::DiFileLogger::OutputLog(const char* msg, const char* levelInfo)
{
    mFile << levelInfo << msg << std::endl;
}
