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

#ifndef DiGfxBase_h__
#define DiGfxBase_h__

#include "GfxDefines.h"

namespace Demi
{
    class DI_GFX_API DiBase
    {
    public:

        static DiGfxDriver*         Driver;
        
        static DiRenderWindow*      ActiveRenderWindow;

        static DiWndHandle          mMainHwnd;

        static DiCommandManager*    CommandMgr;

#    if (USE_MEMORY_TRACE == 1)
        void* operator             new (size_t stSize);

        void* operator             new[] (size_t stSize);
        
        void* operator             new( size_t stSize, const char* pszFileName, unsigned int uiLine );
        
        void* operator             new[](size_t stSize, const char* pszFileName, unsigned int uiLine);

        void  operator             delete(void* pMemory);
        
        void  operator             delete[](void* pMemory);
        
        void  operator             delete(void* pMemory, const char* pszFileName, unsigned int uiLine);
        
        void  operator             delete[](void* pMemory, const char* pszFileName, unsigned int uiLine);
#    else 
        void* operator             new (size_t stSize);
        
        void* operator             new[] (size_t stSize);
        
        void  operator             delete (void* pMemory);
        
        void  operator             delete[](void* pMemory);
#    endif
    };
}

#endif
