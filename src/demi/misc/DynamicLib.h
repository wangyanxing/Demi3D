
/********************************************************************
    File:       DynamicLib.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#ifdef WIN32
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#    define DYNLIB_EXTENSION ".dll"

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#else
#error "Win32 only now!"
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