

#ifndef MiscDefines_h__
#define MiscDefines_h__

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable : 4251)   // dll-interface
#   pragma warning(disable : 4244)
#   pragma warning(disable : 4275)
#   pragma warning(disable : 4127)
#   pragma warning(disable : 4996)   // _CRT_SECURE_NO_WARNINGS
#endif

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DI_MISC_API
#   else
#       ifdef DI_MISC_EXPORT
#           define DI_MISC_API __declspec(dllexport)
#       else
#           define DI_MISC_API __declspec(dllimport)
#       endif
#   endif

#   if defined(_DEBUG) || defined(DEBUG)
#       define DEMI_DEBUG 1
#   else
#       define DEMI_DEBUG 0
#   endif

#   define stricmp _stricmp

#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
// Enable GCC symbol visibility
#   if defined( DEMI_GCC_VISIBILITY )
#       define DI_MISC_API  __attribute__ ((visibility("default")))
#   else
#       define DI_MISC_API
#   endif
#endif


#if (DEMI_COMPILER == DEMI_COMPILER_MSVC)
#   define SAFE_SPRINTF sprintf_s
#else
#   define SAFE_SPRINTF snprintf
#endif


#if DEMI_PLATFORM == DEMI_PLATFORM_LINUX || DEMI_PLATFORM == DEMI_PLATFORM_OSX || DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   define stricmp strcasecmp

#   ifdef DEBUG
#       define DEMI_DEBUG 1
#   else
#       define DEMI_DEBUG 0
#   endif

#endif

//-----------------------------------------------------------------------------
#define DI_ERROR(...) \
    Demi::DiLogManager::GetInstancePtr()->Output(Demi::LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__); \
    Demi::DiLogManager::GetInstancePtr()->Error( __FILE__ , __LINE__ , __VA_ARGS__);

#define DI_WARNING(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_WARNING,\
    __FILE__ , __LINE__ , __VA_ARGS__)

#define DI_LOG(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_LOG, __FILE__ , __LINE__ , __VA_ARGS__)

#define DI_INFO(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_LOG, nullptr, 0, __VA_ARGS__)

#if DEMI_DEBUG == 1
#    define DI_DEBUG(...) Demi::DiLogManager::GetInstancePtr()->Output( Demi::LOG_LEVEL_DEBUG,\
        __FILE__ , __LINE__ , __VA_ARGS__)
#else
#    define DI_DEBUG(...)
#endif

//-----------------------------------------------------------------------------
#if DEMI_DEBUG == 0
#   define DISABLE_MEMORY_TRACE
#endif

#ifndef SAFE_DELETE
#    define SAFE_DELETE(P)       if(P) { delete (P); (P) = NULL; }
#endif

#ifndef SAFE_ARRAY_DELETE
#    define SAFE_ARRAY_DELETE(P) if(P) { delete[] (P); (P) = NULL; }
#endif

#ifndef SAFE_RELEASE
#    define SAFE_RELEASE(P)      if(P) { (P)->Release(); (P) = NULL; }
#endif

#define PROFILE(name) Demi::DiProfileSample kSample(name)
#define PROFILE_BEGINE(name) Demi::DiProfileManager::GetInstancePtr()->Start(name);
#define PROFILE_END() Demi::DiProfileManager::GetInstancePtr()->Stop();

typedef int IndexT;
typedef int SizeT;
const int InvalidIndex = -1;

#define THREADLOCAL __declspec(thread)
#define PREFAST_SUPPRESS_WARNING(W) __pragma(warning(suppress:W))

typedef void *THREAD_HANDLE;
typedef void *EVENT_HANDLE;

#define BIT(index) (1 << (index))

#endif