
#include "MiscPch.h"
#include "DynamicLib.h"
#include "PathLib.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#  define WIN32_LEAN_AND_MEAN
#  if !defined(NOMINMAX) && defined(_MSC_VER)
#    define NOMINMAX // required to stop windows.h messing up std::min
#  endif
#  include <windows.h>
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX || DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "MacUtils.h"
#   include <dlfcn.h>
#endif

namespace Demi
{
    DiMap<DiString, DiDynLib*> DiDynLib::Libs;

    DiDynLib::DiDynLib(const DiString& name)
        :mInst(NULL),
         mName(name)
    {
    }

    DiDynLib::~DiDynLib()
    {
    }

    bool DiDynLib::Load()
    {
        DI_LOG("Loading dynamic plugin : %s", mName.c_str());
        DiString file = mName;

        if (!DiString::EndsWith(file, DYNLIB_EXTENSION, false))
        {
            file += DYNLIB_EXTENSION;
        }

        DiString moduleFileName = DiPathLib::GetApplicationPath() + file;

        mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(moduleFileName.c_str());

        if (!mInst)
        {
            DI_WARNING("Cannot load dynamic library: %s", moduleFileName.c_str());
            DI_WARNING("System error: %s", DynlibError().c_str());
            return false;
        }
        return true;
    }

    bool DiDynLib::Unload()
    {
        DI_LOG("Unloading dynamic plugin : %s", mName.c_str());
        if (DYNLIB_UNLOAD(mInst))
        {
            DI_WARNING("Cannot unload dynamic library: %s", mName.c_str());
            return false;
        }
        return true;
    }

    void* DiDynLib::GetSymbol(const DiString& strName) const throw()
    {
        return (void*)DYNLIB_GETSYM(mInst, strName.c_str());
    }

    DiString DiDynLib::DynlibError(void)
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL
            );
        DiString ret = (char*)lpMsgBuf;
        LocalFree(lpMsgBuf);
        return ret;
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        return DiString(dlerror());
#else
        return DiString("");
#endif
    }

    bool DiDynLib::HasLib(const DiString& libname)
    {
        return Libs.find(libname) != Libs.end();
    }

    DiDynLib* DiDynLib::LoadLib(const DiString& libname)
    {
        auto i = Libs.find(libname);
        if (i != Libs.end())
        {
            return i->second;
        }
        else
        {
            DiDynLib* pLib = DI_NEW DiDynLib(libname);
            pLib->Load();
            Libs[libname] = pLib;
            return pLib;
        }
    }

    bool DiDynLib::UnloadLib(const DiString& libname)
    {
        auto i = Libs.find(libname);
        if (i != Libs.end())
        {
            bool ret = i->second->Unload();
            DI_DELETE(i->second);
            Libs.erase(i);
            return ret;
        }
        else
        {
            DI_WARNING("Could not find the dynamic lib : %s", libname.c_str());
            return false;
        }
    }

}