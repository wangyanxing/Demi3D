
#include "MemoryTrace.h"
#include "PathLib.h"
#include "Timer.h"

namespace Demi
{
    DiMemoryTrace::DiMemoryTrace()
    {
        Init();
    }

    DiMemoryTrace::~DiMemoryTrace()
    {        
        SaveMemoryLeak();
    }

    void DiMemoryTrace::Alloc(void* pMemory, size_t stSize, const char* pszFileName, uint32 uiLine)
    {
        ++mAllocIndex;

        UINT64 ulHashIndex = AddresToHash(pMemory);

        AllocInfoListIter it = mAllocInfoList.find(ulHashIndex);

        DI_ASSERT( it == mAllocInfoList.end() );
    

        if ( it == mAllocInfoList.end() )
        {
            AllocInfo kAllocInfo;
            kAllocInfo.uiIndex = mAllocIndex;
            kAllocInfo.uiSize = static_cast<uint32>(stSize);

            mAllocInfoList[ulHashIndex] = kAllocInfo;

            WriteAllocInfo((LONG)pMemory,stSize, pszFileName, uiLine);
        }
    }

    void DiMemoryTrace::Free(void* pMemory)
    {
        UINT64 ulHashIndex = AddresToHash(pMemory);
        AllocInfoListIter it = mAllocInfoList.find(ulHashIndex);
    
        //DI_ASSERT( it != m_kAllocInfoList.end() );

        if ( it != mAllocInfoList.end() )
        {
            AllocInfo& kAllocInfo = (it->second);
            WriteFreeInfo((LONG)(pMemory), kAllocInfo.uiIndex);

            mAllocInfoList.erase(it);
        }
    }

    void DiMemoryTrace::Init()
    {
        DiString fileName = DiPathLib::GetApplicationPath() + "MemoryLeak.log";

        mMemoryLeakFile.open(fileName.c_str());
        if (!mMemoryLeakFile)
        {
            DI_ERROR("Cannot create memory leak report file");
        }

        fileName = DiPathLib::GetApplicationPath() + "DiMemoryTrace.log";
        mMemoryTraceFile.open(fileName.c_str());
        if (!mMemoryTraceFile)
        {
            DI_ERROR("Cannot create memory trace report file");
        }
    }

    void DiMemoryTrace::Close()
    {
        mMemoryLeakFile.close();
        mMemoryTraceFile.close();
    }

    void DiMemoryTrace::WriteAllocInfo(UINT_PTR ptr, size_t stSize, const char* pszFileName, uint32 uiLine)
    {
        DiString datetime = DiTimer::GetCurrentDateTime();
        DiString log;
        log.Format(
            "%s, Addr [%x], Alloc:(%d), file : %s(line:%d) size = %d Byte.\n",
            datetime.c_str(), ptr, mAllocIndex, pszFileName, uiLine, stSize);
        mMemoryTraceFile << log.c_str();
    }

    void DiMemoryTrace::WriteFreeInfo(UINT_PTR ptr, uint32 uiAllocIndex)
    {
        DiString datetime = DiTimer::GetCurrentDateTime();
        DiString log;
        log.Format(
            "%s, Addr [%x], Free(%d).\n",
            datetime.c_str(), ptr, uiAllocIndex);
        mMemoryTraceFile << log.c_str();
    }

    void DiMemoryTrace::SaveMemoryLeak()
    {    
        if ( mAllocInfoList.size() != 0 )
        {
            mMemoryLeakFile << "Memory leaks detected:\n";

            for (auto it = mAllocInfoList.begin(); it != mAllocInfoList.end(); ++it)
            {
                const AllocInfo& rkAllocInfo = it->second;

                DiString log;
                log.Format("Alloc(%d) size = %d Byte\n", rkAllocInfo.uiIndex, rkAllocInfo.uiSize);
                mMemoryLeakFile << log.c_str();
            }
        }
        else
        {
            mMemoryLeakFile << "No memory leak detected.\n";
        }
    }

}