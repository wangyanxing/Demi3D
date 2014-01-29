
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
    const DiString& _GetAppFileName()
    {
        return DiString::BLANK;
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