
/********************************************************************
    File:       PlatformSpecific.h
    Creator:    demiwangya
*********************************************************************/

#ifndef PlatformSpecific_h__
#define PlatformSpecific_h__

#define DEMI_PLATFORM_WIN32   1
#define DEMI_PLATFORM_OSX     2
#define DEMI_PLATFORM_IOS     3

#define DEMI_BUILD_32         1
#define DEMI_BUILD_64         2

#define DEMI_COMPILER_MSVC    1
#define DEMI_COMPILER_GNUC    2
#define DEMI_COMPILER_CLANG   3

#define DEMI_LITTLE_ENDIAN    1
#define DEMI_BIG_ENDIAN       2

#if defined( _MSC_VER )
#   define DEMI_COMPILER        DEMI_COMPILER_MSVC
#   define DEMI_COMPILER_VER    _MSC_VER
#elif defined( __clang__ )
#   define DEMI_COMPILER        DEMI_COMPILER_CLANG
#   define DEMI_COMPILER_VER (((__clang_major__)*100) + \
    (__clang_minor__ * 10) + \
    __clang_patchlevel__)
#elif defined( __GNUC__ )
#   define DEMI_COMPILER        DEMI_COMPILER_GNUC
#   define DEMI_COMPILER_VER (((__GNUC__)*100) + \
    (__GNUC_MINOR__ * 10) + \
    __GNUC_PATCHLEVEL__)
#else
#error "Demi3D doesn't support this compiler at this moment."
#endif

#if (DEMI_COMPILER == DEMI_COMPILER_MSVC)
#   if DEMI_COMPILER_VER <= 1500
#       error Demi Engine needs at least vc2010.
#   endif
#endif

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define DEMI_PLATFORM DEMI_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define DEMI_PLATFORM DEMI_PLATFORM_IOS
#   else
#       define DEMI_PLATFORM DEMI_PLATFORM_OSX
#   endif
#else
#error "Demi3D doesn't support this platform at this moment."
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define DEMI_ARCH_TYPE DEMI_BUILD_64
#else
#   define DEMI_ARCH_TYPE DEMI_BUILD_32
#endif


#if (DEMI_COMPILER == DEMI_COMPILER_MSVC)
#   define SAFE_SPRINTF sprintf_s
#   define SAFE_VSPRINTF vsprintf_s
#else
#   define SAFE_SPRINTF snprintf
#   define SAFE_VSPRINTF vsnprintf
#endif

typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef unsigned char       uint8;
typedef unsigned char       BYTE;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
#if DEMI_COMPILER == DEMI_COMPILER_MSVC
typedef unsigned __int64    uint64;
typedef __int64             int64;
#else
typedef unsigned long long  uint64;
typedef long long           int64;
#endif
typedef float               f32;
typedef double              f64;
typedef double              real;
typedef unsigned long       ulong;
typedef int                 BOOL;

typedef void *THREAD_HANDLE;
typedef void *EVENT_HANDLE;

#define _MS_ALIGN(num) __declspec(align(num))

#define DEFINE_ALIGNED_DATA( type, name, alignment ) _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_STATIC( type, name, alignment ) static _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_CONST( type, name, alignment ) const _declspec(align(alignment)) type name;

#endif // PlatformSpecific_h__
