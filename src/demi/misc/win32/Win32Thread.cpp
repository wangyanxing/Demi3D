
#include "MiscPch.h"
#include "Win32Thread.h"
#include <windows.h>

namespace Demi
{
    THREADLOCAL const char* DiTheradWin32::ThreadName = 0;

#if _DEBUG
    DiCriticalSectionWin32 DiTheradWin32::criticalSection;
    DiList<DiTheradWin32*> DiTheradWin32::ThreadList;
#endif

    DiTheradWin32::DiTheradWin32() :
        threadHandle(0),
        running(false),
        priority(Normal),
        stackSize(4096),
        coreId(InvalidCoreId)
    {
#if _DEBUG
        DiTheradWin32::criticalSection.Enter();
        ThreadList.push_back(this);
        this->threadListIterator = --ThreadList.end();
        DiTheradWin32::criticalSection.Leave();
#endif
    }

    DiTheradWin32::~DiTheradWin32()
    {
        if (this->IsRunning())
        {
            this->Stop();
        }

#if _DEBUG
        DiTheradWin32::criticalSection.Enter();
        ThreadList.erase(this->threadListIterator);
        DiTheradWin32::criticalSection.Leave();
#endif
    }

    void DiTheradWin32::Start()
    {
        DI_ASSERT(!this->IsRunning());
        DI_ASSERT(0 == this->threadHandle);
        this->threadHandle = CreateThread(NULL,             // lpThreadAttributes
            this->stackSize,  // dwStackSize
            ThreadProc,       // lpStartAddress
            (LPVOID) this,    // lpParameter
            CREATE_SUSPENDED, // dwCreationFlags
            NULL);            // lpThreadId
        DI_ASSERT(NULL != this->threadHandle);

        // apply thread priority
        switch (this->priority)
        {
        case Low:
            SetThreadPriority(this->threadHandle, THREAD_PRIORITY_BELOW_NORMAL);
            break;

        case Normal:
            SetThreadPriority(this->threadHandle, THREAD_PRIORITY_NORMAL);
            break;

        case High:
            SetThreadPriority(this->threadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
            break;
        }

        // select a good processor for the thread
        SYSTEM_INFO sysInfo;
        ::GetSystemInfo(&sysInfo);

        SizeT numCpuCores = sysInfo.dwNumberOfProcessors;
        DWORD threadIdealProc = 0;
        if (InvalidCoreId != this->coreId)
            threadIdealProc = this->coreId % numCpuCores;

        SetThreadIdealProcessor(this->threadHandle, threadIdealProc);

        // resume thread (since it was actived in suspended state)
        ResumeThread(this->threadHandle);

        // wait for the thread to start
        this->threadStartedEvent.Wait();
    }

    DWORD WINAPI DiTheradWin32::ThreadProc(LPVOID self)
    {
        DI_ASSERT(0 != self);
        DiTheradWin32* threadObj = (DiTheradWin32*)self;
        DiTheradWin32::SetMyThreadName(threadObj->GetName().c_str());
        threadObj->threadStartedEvent.Signal();
        threadObj->DoWork();
        return 0;
    }

    void DiTheradWin32::SetMyThreadName(const char* n)
    {
        // first update our own internal thread-name pointer
        ThreadName = n;

        // update the Windows thread name so that it shows up correctly
        // in the Debugger
        struct THREADNAME_INFO
        {
            DWORD dwType;     // must be 0x1000
            LPCSTR szName;    // pointer to name (in user address space)
            DWORD dwThreadID; // thread ID (-1 = caller thread)
            DWORD dwFlags;    // reserved for future use, must be zero
        };

        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = n;
        info.dwThreadID = ::GetCurrentThreadId();
        info.dwFlags = 0;
        __try
        {
            RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
        }
        __except (EXCEPTION_CONTINUE_EXECUTION)
        {
        }
    }

    ThreadId DiTheradWin32::GetMyThreadId()
    {
        return ::GetCurrentThreadId();
    }

    void DiTheradWin32::Stop()
    {
        DI_ASSERT(this->IsRunning());
        DI_ASSERT(0 != this->threadHandle);

        // signal the thread to stop
        this->stopRequestEvent.Signal();

        // call the wakeup-thread method, may be derived in a subclass
        // if the threads needs to be woken up, it is important that this
        // method is called AFTER the stopRequestEvent is signalled!
        this->EmitWakeupSignal();

        // wait for the thread to terminate
        WaitForSingleObject(this->threadHandle, INFINITE);
        CloseHandle(this->threadHandle);
        this->threadHandle = 0;
    }

#if _DEBUG
    DiVector<DiTheradWin32::ThreadDebugInfo> DiTheradWin32::GetRunningThreadDebugInfos()
    {
        DiVector<DiTheradWin32::ThreadDebugInfo> infos;
        DiTheradWin32::criticalSection.Enter();
        for (auto it = ThreadList.begin(); it != ThreadList.end(); ++it)
        {
            DiTheradWin32* cur = *it;
            if (cur->IsRunning())
            {
                ThreadDebugInfo info;
                info.threadName = cur->GetName();
                info.threadPriority = cur->GetPriority();
                info.threadStackSize = cur->GetStackSize();
                infos.push_back(info);
            }
        }
        DiTheradWin32::criticalSection.Leave();
        return infos;
    }
#endif
}