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
#ifndef DiLogger_h__
#define DiLogger_h__


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



#endif
