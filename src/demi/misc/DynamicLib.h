
/********************************************************************
    File:       DynamicLib.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#    define DYNLIB_EXTENSION ".dll"

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX || DEMI_PLATFORM == DEMI_PLATFORM_IOS
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) mac_loadDylib( a )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#    define DYNLIB_EXTENSION ".dylib"

#else
#   error "Unsupported platform, no implementation for DiDynLib!"
#endif

namespace Demi
{
    class DI_MISC_API DiDynLib
    {
    
    protected:

        DiDynLib(const DiString& name);

        ~DiDynLib();

    public:

        static bool      HasLib(const DiString& libname);

        static DiDynLib* LoadLib(const DiString& libname);

        static bool      UnloadLib(const DiString& libname);

    protected:

        bool            Load();

        bool            Unload();

    public:

        const DiString& GetName(void) const { return mName; }

        void*           GetSymbol(const DiString& strName) const throw();

    protected:

        DiString        DynlibError(void);

    protected:

        DYNLIB_HANDLE   mInst;

        DiString        mName;

    protected:

        static DiMap<DiString, DiDynLib*> Libs;
    };
}