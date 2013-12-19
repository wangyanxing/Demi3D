
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