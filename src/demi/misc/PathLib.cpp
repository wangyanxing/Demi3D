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
#include "PathLib.h"

namespace Demi
{
    DiString DiPathLib::msAppFile;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32

    const DiString& _GetAppFileName()
    {
        static DiString appFile;

        char moduleFile[MAX_PATH];
        GetModuleFileNameA(0, moduleFile, MAX_PATH);
        appFile = moduleFile;

        return appFile;
    }

#define _SLASH "\\"

#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include <mach-o/dyld.h>
    
    const DiString& _GetAppFileName()
    {
        static DiString ret;
        uint32_t pathNameSize = 1024;
        char pathName[1024];
        
        if (!_NSGetExecutablePath(pathName, &pathNameSize))
        {
            char real[PATH_MAX];
            
            if (realpath(pathName, real) != NULL)
            {
                pathNameSize = strlen(real);
                strncpy(pathName, real, pathNameSize);
            }
            
            ret = pathName;
        }
        
        
        
        return ret;
    }

#define _SLASH "/"

#endif

    const DiString& DiPathLib::GetApplicationFileName()
    {
        if (msAppFile.empty())
            msAppFile = _GetAppFileName();
        return msAppFile;
    }

    const DiString& DiPathLib::GetApplicationPath()
    {
        static DiString path;

        const DiString& appFile = GetApplicationFileName();
        size_t pos = appFile.rfind(_SLASH);
        path = appFile.substr(0, pos) + _SLASH;
        return path;
    }

}