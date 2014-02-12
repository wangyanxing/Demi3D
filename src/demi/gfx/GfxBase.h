


#ifndef DiGfxBase_h__
#define DiGfxBase_h__


namespace Demi
{
    class DI_GFX_API DiBase
    {
    public:

        static DiGfxDriver*         Driver;

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
