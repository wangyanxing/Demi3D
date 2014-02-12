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

#ifndef DiConsole_h__
#define DiConsole_h__


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

#endif
