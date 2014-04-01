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
#include "GfxPch.h"
#include "OptimisedUtil.h"

namespace Demi
{
    DiOptimisedUtil* DiOptimisedUtil::sImplementation = NULL;

    extern DiOptimisedUtil* GetOptimisedUtilGeneral(void);

#if DEMI_PLATFORM != DEMI_PLATFORM_IOS
    extern DiOptimisedUtil* GetOptimisedUtilSSE(void);
#endif

    DiOptimisedUtil* DiOptimisedUtil::DetectImplementation( void )
    {
#if DEMI_PLATFORM != DEMI_PLATFORM_IOS
        return GetOptimisedUtilSSE();
#else
        return GetOptimisedUtilGeneral();
#endif
    }

    DiOptimisedUtil* DiOptimisedUtil::GetInstance( void )
    {
        if (!sImplementation)
        {
            sImplementation = DetectImplementation();
        }
        return sImplementation;
    }
}