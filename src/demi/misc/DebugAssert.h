
/********************************************************************
    File:       DebugAssert.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "PlatformFunction.h"

#ifdef _DEMI_RELEASE
#    define DI_ASSERT(condition)
#    define DI_ASSERT_MESSAGE(condition,message)                            
#else
#   if DEMI_COMPILER == DEMI_COMPILER_MSVC
#       define DI_ASSERT(condition) DI_ASSERT_MESSAGE(condition,NULL)
#       define DI_ASSERT_MESSAGE(condition,message)                                    \
        {                                                                            \
            static bool ignoreAssert = false;                                        \
            if(!ignoreAssert && !(condition))                                        \
            {                                                                        \
                if(Demi::DiPlatformFunc::AssertDlg(#condition,__FILE__,__LINE__,ignoreAssert))    \
                {                    \
                    __asm int 3      \
                }                    \
            }                        \
        }
#   else
#   include <assert.h>
#   define DI_ASSERT(condition) assert(condition)
#   endif
#endif

#define DI_ASSERT_FAIL DI_ASSERT(false)
#undef  assert
#define assert DI_ASSERT