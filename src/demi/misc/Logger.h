/**************************************************************************
    File:       Logger.h
    Creator:    demiwangya
**************************************************************************/

#pragma once

#include <fstream>
#include <iostream>
#include "FileStream.h"

namespace Demi  
{
    class DI_MISC_API DiLogger
    {
    public:

        DiLogger(){}

        virtual         ~DiLogger(){}

        virtual void    OutputLog(const char* msg, const char* levelInfo, const char* fileName, long line) = 0;

        virtual void    OutputLog(const char* msg, const char* levelInfo) = 0;
    };

    class DI_MISC_API DiFileLogger : public DiLogger
    {
    public:

        DiFileLogger();

        ~DiFileLogger();

    public:

        bool            Init(const char* szFileName);
        
        void            Shutdown();

        void            OutputLog(const char* msg, const char* levelInfo,
                           const char* fileName, long line);

        void            OutputLog(const char* msg, const char* levelInfo);

    private:

        DiOfstream      mFile;
    };

}

