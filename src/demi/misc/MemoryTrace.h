

#ifndef DiMemoryTrace_h__
#define DiMemoryTrace_h__


#include "MiscDefines.h"

namespace Demi 
{
    class DI_MISC_API DiMemoryTrace
    {
    public:

        DiMemoryTrace();

        ~DiMemoryTrace();
        
    public:
        struct AllocInfo
        {
            uint32    uiIndex;
            uint32    uiSize;
        };

        typedef DiMap<uint64, AllocInfo> AllocInfoList;
        typedef AllocInfoList::iterator  AllocInfoListIter;

        static DiMemoryTrace& GetInstance()
        {
            static DiMemoryTrace sMemoryTrace;        
            return sMemoryTrace;
        }

    public:

        void             Alloc(void* pMemory, size_t stSize, const char* pszFileName, uint32 uiLine);
                         
        void             Free(void* pMemory);
                         
    private:             
        uint64           AddresToHash(void* pMemory);
                         
        void             Init();
                         
        void             Close();
                         
        void             WriteAllocInfo(long ptr, size_t stSize, const char* pszFileName, uint32 uiLine);
                         
        void             WriteFreeInfo(long ptr, uint32 uiAllocIndex);
                         
        void             SaveMemoryLeak();

    private:
        uint32           mAllocIndex;

        AllocInfoList    mAllocInfoList;

        DiOfstream       mMemoryLeakFile;

        DiOfstream       mMemoryTraceFile;
    };

    inline uint64 DiMemoryTrace::AddresToHash(void* pMemory)
    {
        return (uint64)(reinterpret_cast<size_t>(pMemory));
    }

}

#endif
