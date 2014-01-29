
/********************************************************************
    File:       DemoDefines.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DemoDefines_h__
#define DemoDefines_h__

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DEMI_DEMO_API
#   else
#       ifdef DI_DEMO_EXPORT
#           define DEMI_DEMO_API __declspec(dllexport)
#       else
#           define DEMI_DEMO_API __declspec(dllimport)
#       endif
#   endif
#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
#   ifdef DEMI_STATIC_API
#       define DEMI_DEMO_API
#   else
#       if defined( DEMI_GCC_VISIBILITY )
#           define DEMI_DEMO_API  __attribute__ ((visibility("default")))
#       else
#           define DEMI_DEMO_API
#       endif
#   endif
#endif

#endif // DemoDefines_h__