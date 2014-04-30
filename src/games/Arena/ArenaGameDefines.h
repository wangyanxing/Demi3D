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

#ifndef ArenaGameDefines_h__
#define ArenaGameDefines_h__

#include "ArenaPrerequisites.h"
#include "ArenaStateDefines.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#    define ATTR_NORETURN
#    define ATTR_UNUSED( name ) name
#else
#   define  ATTR_NORETURN           __attribute__((noreturn))
#   define  ATTR_UNUSED( name )     name __attribute__((unused))
#endif

namespace Demi
{
    typedef uint32  ArObjID;

    const ArObjID   ATTR_UNUSED(INVALID_OBJ_ID) = 0xFFFFFFFF;
    const int32     INVALID_INT_VALUE           = 0x7fffffff;
    const float     INVALID_FLOAT_VALUE         = 1.7014117e+38f;

    enum ArWalkMode
    {
        ENUM_WALK_MODE_STOP = 0,
        ENUM_WALK_MODE_WALK,
    };

    enum ArModalityType
    {
        MODALITY_STAND = 0,
        MODALITY_WALK = 1,
        MODALITY_RUN = 2,
        MODALITY_JUMP = 3,
    };
}

#endif