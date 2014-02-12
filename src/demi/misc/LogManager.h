

#ifndef DiLogManager_h__
#define DiLogManager_h__


#include "Array.h"
#include "Singleton.h"
#include "Logger.h"

namespace Demi  
{
    enum LogLevel
    {        
        LOG_LEVEL_ERROR   = 0,
        LOG_LEVEL_WARNING = 1,
        LOG_LEVEL_LOG     = 2,
        LOG_LEVEL_DEBUG   = 3,

        MAX_LOG_LEVEL,
    };

    class DI_MISC_API DiLogManager : public DiSingleton<DiLogManager>
    {
    public:

        DiLogManager(void);

        ~DiLogManager(void);

        _DECLARE_SINGLETON(DiLogManager)

        typedef DiVector<DiLogger*>    Loggers;

    public:

        void            Init(const DiString& logFileName);

        void            RegisterLogger(DiLogger* lg);

        void            UnregisterLogger(DiLogger* lg);

        void            Shutdown();

        void            Output(LogLevel level, const char* log);

        void            Output(LogLevel level,
                            const char* szFileName, unsigned int uiLine, 
                            const char* pacFormat, ... );

        void            OutputUnformat(LogLevel level,
                            const char* szFileName, unsigned int uiLine, 
                            const char* sString);

        void            Error(const char* szFileName, unsigned int uiLine, 
                            const char* pacFormat, ... );

    protected:

        uint32          mLogLevel : 4;

        LogLevel        mVerbosityLevel;

        Loggers         mLogger;
    };
}




#endif
