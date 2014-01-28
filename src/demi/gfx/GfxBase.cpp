
#include "GfxPch.h"
#include "GfxBase.h"
#include "MemoryAllocator.h"
#include "MemoryTrace.h"

namespace Demi
{
    DiGfxDriver*      DiBase::Driver     = nullptr;
    DiWndHandle       DiBase::mMainHwnd  = nullptr;
    DiCommandManager* DiBase::CommandMgr = nullptr;

#if (USE_MEMORY_TRACE == 1)

    void* DiBase::operator new( size_t stSize )
    {
        void* pMemory = DI_MALLOC(stSize);    
        DI_ASSERT(pMemory != 0);

        DiMemoryTrace::GetInstance().Alloc(pMemory, stSize, "Unknow 3rd Soure..", 0);

        return pMemory;
    }

    void* DiBase::operator new( size_t stSize, const char* pszFileName, unsigned int uiLine )
    {
        void* pMemory = DI_MALLOC(stSize);    
        DI_ASSERT(pMemory != 0);
        DiMemoryTrace::GetInstance().Alloc(pMemory, stSize, pszFileName, uiLine);
        return pMemory;
    }

    void* DiBase::operator new[]( size_t stSize )
    {
        void* pMemory = DI_MALLOC(stSize);
        DI_ASSERT(pMemory != 0);
        return pMemory;
    }

    void* DiBase::operator new[]( size_t stSize, const char* pszFileName, unsigned int uiLine )
    {
        void* pMemory = DI_MALLOC(stSize);
        DI_ASSERT(pMemory != 0);
        DiMemoryTrace::GetInstance().Alloc(pMemory, stSize, pszFileName, uiLine);
        return pMemory;
    }

    void  DiBase::operator delete[]( void* pMemory, const char* /*pszFileName*/, unsigned int /*uiLine*/ )
    {
        DI_ASSERT(pMemory != 0);
        DiMemoryTrace::GetInstance().Free(pMemory);
        DI_FREE(pMemory);
    }

    void  DiBase::operator delete( void* pMemory, const char* pszFileName, unsigned int uiLine )
    {
        DI_ASSERT(pMemory != 0);
        DiMemoryTrace::GetInstance().Free(pMemory);
        DI_FREE(pMemory);
    }

    void  DiBase::operator delete[]( void* pMemory )
    {
        DI_ASSERT(pMemory != 0);
        DiMemoryTrace::GetInstance().Free(pMemory);
        DI_FREE(pMemory);
    }

    void  DiBase::operator delete( void* pMemory )
    {
        DI_ASSERT(pMemory != 0);
        DiMemoryTrace::GetInstance().Free(pMemory);
        DI_FREE(pMemory);
    }
#    else 

    void* DiBase::operator new( size_t stSize )
    {
        void* pMemory = DI_MALLOC( stSize );
        DI_ASSERT(pMemory != 0);

        return pMemory;
    }

    void* DiBase::operator new[]( size_t stSize )
    {
        return ::operator new(stSize);
    }


    void DiBase::operator delete[]( void* pMemory )
    {
        DI_ASSERT(pMemory != 0);
        DI_FREE(pMemory);
    }

    void DiBase::operator delete( void* pMemory )
    {
        ::operator delete(pMemory);
    }
#    endif
}