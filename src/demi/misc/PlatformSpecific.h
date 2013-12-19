
/********************************************************************
    File:       PlatformSpecific.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#if defined(_MSC_VER)
#   if _MSC_VER <= 1500
#       error Demi Engine needs at least vc2010.
#   endif
#else
#   error Demi Engine doesn't support this compiler at this moment.
#endif

typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed __int64      int64;
typedef unsigned char       uint8;
typedef unsigned char       BYTE;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64    uint64;
typedef float               f32;
typedef double              f64;
typedef double              real;
typedef unsigned long       ulong;
typedef int                 BOOL;

#if defined(_WIN64)
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
#else
typedef __w64 int INT_PTR, *PINT_PTR;
typedef __w64 unsigned int UINT_PTR, *PUINT_PTR;

typedef __w64 long LONG_PTR, *PLONG_PTR;
typedef __w64 unsigned long ULONG_PTR, *PULONG_PTR;

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
#endif

typedef void *THREAD_HANDLE;
typedef void *EVENT_HANDLE;


#define _MS_ALIGN(num) __declspec(align(num))

#define DEFINE_ALIGNED_DATA( type, name, alignment ) _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_STATIC( type, name, alignment ) static _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_CONST( type, name, alignment ) const _declspec(align(alignment)) type name;
