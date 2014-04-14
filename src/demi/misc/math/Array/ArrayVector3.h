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

/// This file is adapted from Ogre 2.0 (unstable version)
#ifndef __ArrayVector3_H__
#define __ArrayVector3_H__

//This file is a proxy, it redirects to the proper file depending on platform
#include "ArrayConfig.h"

#if DEMI_CPU == DEMI_CPU_X86 && DEMI_USE_SIMD == 1
    #if DEMI_DOUBLE_PRECISION == 1
        #include "SSE2/Double/ArrayVector3.h"
    #else
        #include "SSE2/Single/ArrayVector3.h"
    #endif
#elif DEMI_CPU == DEMI_CPU_ARM && DEMI_USE_SIMD == 1
    #if DEMI_DOUBLE_PRECISION == 1
        #error Double precision with SIMD on ARM is not supported
    #else
        #include "NEON/Single/ArrayVector3.h"
    #endif
#else
    #include "C/ArrayVector3.h"
#endif

#endif
