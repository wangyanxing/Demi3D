#pragma once

#ifdef DEMI_STATIC_API
#    define DEMI_DEMO_API
#else
#    ifdef DI_DEMO_EXPORT
#        define DEMI_DEMO_API  __declspec(dllexport)
#    else
#        define DEMI_DEMO_API __declspec(dllimport)
#    endif
#endif
