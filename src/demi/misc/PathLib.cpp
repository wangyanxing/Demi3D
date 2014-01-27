
#include "MiscPch.h"
#include "PathLib.h"

namespace Demi
{
#ifdef WIN32
    const DiString& DiPathLib::GetApplicationFileName()
    {
        static DiString appFile;

        char moduleFile[MAX_PATH];
        GetModuleFileNameA(0, moduleFile, MAX_PATH);
        appFile = moduleFile;

        return appFile;
    }
#endif

    const DiString& DiPathLib::GetApplicationPath()
    {
        static DiString path;

        const DiString& appFile = GetApplicationFileName();
        size_t pos = appFile.rfind("\\");
        path = appFile.substr(0, pos) + "\\";
        return path;
    }
}