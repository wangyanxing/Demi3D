#pragma once

#ifdef DEMI_STATIC_API
#    define DEMI_EXT_API
#else
#    ifdef DI_EXT_EXPORT
#        define DEMI_EXT_API  __declspec(dllexport)
#    else
#        define DEMI_EXT_API __declspec(dllimport)
#    endif
#endif
