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

#ifndef ScriptDefines_h__
#define ScriptDefines_h__

#if (DEMI_PLATFORM == DEMI_PLATFORM_WIN32)
#   ifdef DEMI_STATIC_API
#       define DI_SCRIPT_API
#   else
#       ifdef DI_SCRIPT_EXPORT
#           define DI_SCRIPT_API __declspec(dllexport)
#       else
#           define DI_SCRIPT_API __declspec(dllimport)
#       endif
#   endif
#elif (DEMI_PLATFORM == DEMI_PLATFORM_IOS || DEMI_PLATFORM == DEMI_PLATFORM_OSX)
#   ifdef DEMI_STATIC_API
#       define DI_SCRIPT_API
#   else
#       if defined( DEMI_GCC_VISIBILITY )
#           define DI_SCRIPT_API __attribute__ ((visibility("default")))
#       else
#           define DI_SCRIPT_API
#       endif
#   endif
#endif

/// Lots of generated code in here which triggers the new VC CRT security warnings
#if !defined( _CRT_SECURE_NO_DEPRECATE )
#   define _CRT_SECURE_NO_DEPRECATE
#endif

#endif