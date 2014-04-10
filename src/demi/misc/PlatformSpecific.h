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

#ifndef PlatformSpecific_h__
#define PlatformSpecific_h__

#define DEMI_PLATFORM_WIN32   1
#define DEMI_PLATFORM_OSX     2
#define DEMI_PLATFORM_IOS     3
#define DEMI_PLATFORM_LINUX   4
#define DEMI_PLATFORM_ANDROID 5

#define DEMI_CPU_UNKNOWN    0
#define DEMI_CPU_X86        1
#define DEMI_CPU_PPC        2
#define DEMI_CPU_ARM        3
#define DEMI_CPU_MIPS       4

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

#if (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) || \
    (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
#   define DEMI_CPU DEMI_CPU_X86

#elif DEMI_PLATFORM == DEMI_PLATFORM_APPLE && defined(__BIG_ENDIAN__)
#   define DEMI_CPU DEMI_CPU_PPC
#elif DEMI_PLATFORM == DEMI_PLATFORM_APPLE
#   define DEMI_CPU DEMI_CPU_X86
#elif DEMI_PLATFORM == DEMI_PLATFORM_APPLE_IOS && (defined(__i386__) || defined(__x86_64__))
#   define DEMI_CPU DEMI_CPU_X86
#elif defined(__arm__) || defined(_M_ARM) || defined(__arm64__) || defined(_aarch64_)
#   define DEMI_CPU DEMI_CPU_ARM
#elif defined(__mips64) || defined(__mips64_)
#   define DEMI_CPU DEMI_CPU_MIPS
#else
#   define DEMI_CPU DEMI_CPU_UNKNOWN
#endif

//////////////////////////////////////////////////////////////////////////
// SIMD

#define DEMI_USE_SIMD 1
#define DEMI_DOUBLE_PRECISION 0
#define DEMI_RESTRICT_ALIASING 1

#define DEMI_MALLOC_SIMD DiAlignedMemory::Allocate
#define DEMI_FREE_SIMD DiAlignedMemory::Deallocate

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   define DEMI_ALIGNED_DECL(type, var, alignment)  __declspec(align(alignment)) type var

#elif (DEMI_COMPILER == DEMI_COMPILER_GNUC) || (DEMI_COMPILER == DEMI_COMPILER_CLANG)
#   define DEMI_ALIGNED_DECL(type, var, alignment)  type var __attribute__((__aligned__(alignment)))

#else
#   define DEMI_ALIGNED_DECL(type, var, alignment)  type var
#endif

#if DEMI_CPU == DEMI_CPU_X86
#   define DEMI_SIMD_ALIGNMENT  16

#else
#   define DEMI_SIMD_ALIGNMENT  16
#endif

#define DEMI_SIMD_ALIGNED_DECL(type, var)   DEMI_ALIGNED_DECL(type, var, DEMI_SIMD_ALIGNMENT)

#if DEMI_USE_SIMD == 1
#if   DEMI_DOUBLE_PRECISION == 0 && DEMI_CPU == DEMI_CPU_X86 && DEMI_COMPILER == DEMI_COMPILER_MSVC
#   define __DEMI_HAVE_SSE  1
#elif DEMI_DOUBLE_PRECISION == 0 && DEMI_CPU == DEMI_CPU_X86 && (DEMI_COMPILER == DEMI_COMPILER_GNUC || DEMI_COMPILER == DEMI_COMPILER_CLANG) && \
    DEMI_PLATFORM != DEMI_PLATFORM_IOS
#   define __DEMI_HAVE_SSE  1
#endif

/* Define whether or not Ogre compiled with NEON support.
*/
#if DEMI_DOUBLE_PRECISION == 0 && DEMI_CPU == DEMI_CPU_ARM && (DEMI_COMPILER == DEMI_COMPILER_GNUC || DEMI_COMPILER == DEMI_COMPILER_CLANG) && defined(__ARM_NEON__)
#   define __DEMI_HAVE_NEON  1
#endif
#endif

#ifndef __DEMI_HAVE_SSE
#   define __DEMI_HAVE_SSE  0
#endif

#if DEMI_USE_SIMD == 0 || !defined(__DEMI_HAVE_NEON)
#   define __DEMI_HAVE_NEON  0
#endif

#ifndef DEMI_RESTRICT_ALIASING
#   define DEMI_RESTRICT_ALIASING 0
#endif

#if DEMI_RESTRICT_ALIASING != 0
#   if DEMI_COMPILER == DEMI_COMPILER_MSVC
#       define RESTRICT_ALIAS __restrict   //MSVC
#   else
#       define RESTRICT_ALIAS __restrict__ //GCC... and others?
#   endif
#       else
#   define RESTRICT_ALIAS
#endif

//////////////////////////////////////////////////////////////////////////

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

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   if DEMI_COMPILER_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif

#ifdef __linux__
#   include <endian.h>
#   if __BYTE_ORDER == 1234
#      define DEMI_ENDIAN DEMI_LITTLE_ENDIAN
#   else
#      define DEMI_ENDIAN DEMI_BIG_ENDIAN
#   endif
#else
#   if defined(__hppa__) || \
       defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
       (defined(__MIPS__) && defined(__MISPEB__)) || \
       defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
       defined(__sparc__)
#      define DEMI_ENDIAN DEMI_BIG_ENDIAN
#   else
#      define DEMI_ENDIAN DEMI_LITTLE_ENDIAN
#   endif
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

typedef void *THREAD_HANDLE;
typedef void *EVENT_HANDLE;

#define _MS_ALIGN(num) __declspec(align(num))

#define DEFINE_ALIGNED_DATA( type, name, alignment ) _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_STATIC( type, name, alignment ) static _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_CONST( type, name, alignment ) const _declspec(align(alignment)) type name;

#endif // PlatformSpecific_h__
