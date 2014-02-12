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

    extern DiOptimisedUtil* GetOptimisedUtilSSE(void);

    DiOptimisedUtil* DiOptimisedUtil::DetectImplementation( void )
    {
        if(false)
         {
             return GetOptimisedUtilSSE();
         }
         else
        {
            return GetOptimisedUtilGeneral();
        }
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