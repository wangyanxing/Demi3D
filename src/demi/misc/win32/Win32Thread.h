
/********************************************************************
    File:       Win32Thread.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Win32Criticalsection.h"
#include "Win32Event.h"

namespace Demi
{
    typedef DWORD CoreId;

    static const CoreId InvalidCoreId     = 0xffffffff;
    static const CoreId MainThreadCore    = 0;
    static const CoreId RenderThreadCore  = 1;

    typedef DWORD ThreadId;
    static const ThreadId InvalidThreadId = 0xffffffff;

    class DiTheradWin32
    {
    public:
        DiTheradWin32();

        virtual     ~DiTheradWin32();

        enum Priority
        {
            Low,
            Normal,
            High,
        };

    public:

        inline void         SetPriority(Priority p)
        {
            this->priority = p;
        }

        inline Priority     GetPriority() const
        {
            return this->priority;
        }

        void                SetCoreId(CoreId coreId)
        {
            this->coreId = coreId;
        }
        
        CoreId              GetCoreId() const
        {
            return this->coreId;
        }

        inline void         SetStackSize(int s)
        {
            this->stackSize = s;
        }
        
        inline int          GetStackSize() const
        {
            return this->stackSize;
        }
        
        void                SetName(const DiString& n)
        {
            this->name = n;
        }
        
        const DiString&     GetName() const
        {
            return this->name;
        }

        DI_MISC_API void    Start();
        
        DI_MISC_API void    Stop();
        
        inline bool         IsRunning() const
        {
            return (0 != this->threadHandle);
        }

        static DI_MISC_API void SetMyThreadName(const char* n);
        
        static const char*      GetMyThreadName()
        {
            return ThreadName;
        }

        /// get the thread ID of this thread (0 == MainThread)
        static ThreadId     GetMyThreadId();

#if _DEBUG
        struct ThreadDebugInfo
        {
            DiString threadName;
            DiTheradWin32::Priority threadPriority;
            CoreId threadCoreId;
            SizeT threadStackSize;
        };
        
        static DiVector<ThreadDebugInfo> GetRunningThreadDebugInfos();
#endif

    protected:

        virtual void        EmitWakeupSignal() {}
        
        virtual void        DoWork() {}
        
        inline bool         ThreadStopRequested() const
        {
            return this->stopRequestEvent.Peek();
        }

    private:

        static DWORD WINAPI ThreadProc(LPVOID self);

        HANDLE              threadHandle;
        DiEventWin32        threadStartedEvent;
        DiEventWin32        stopRequestEvent;
        bool                running;
        Priority            priority;
        SizeT               stackSize;
        DiString            name;
        CoreId              coreId;
        THREADLOCAL static const char* ThreadName;

#if _DEBUG
        static DiCriticalSectionWin32    criticalSection;
        static DiList<DiTheradWin32*>    ThreadList;
        DiList<DiTheradWin32*>::iterator threadListIterator;
#endif

    };    
}