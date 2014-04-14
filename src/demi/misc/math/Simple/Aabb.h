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
#ifndef __Aabb_H__
#define __Aabb_H__

//This file is a proxy, it redirects to the proper file depending on platform
#include "../Array/ArrayConfig.h"

/*#if DEMI_CPU == DEMI_CPU_X86 && defined( DEMI_USE_SIMD )
    #if DEMI_DOUBLE_PRECISION == 1
        #include "SSE2/Double/Aabb.h"
    #else
        #include "SSE2/Single/Aabb.h"
    #endif
#else*/
    #include "C/Aabb.h"
//#endif

#endif
