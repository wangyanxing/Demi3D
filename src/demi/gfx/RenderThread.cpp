
#include "GfxPch.h"
#include "RenderThread.h"

namespace Demi
{
    DiRenderThread::DiRenderThread()
        :mQuit(false),
        mFlush(0)
    {

    }

    DiRenderThread::~DiRenderThread()
    {

    }

    void DiRenderThread::EmitWakeupSignal()
    {

    }

    void DiRenderThread::DoWork()
    {
        while (true)
        {
            WaitForFlushing();

            if (mQuit)
            {
                DoFinishFlush();
                break;
            }

            // run buffered render commands
            RunCommands();
        }
    }

    void DiRenderThread::RunCommands()
    {
        if (!*(int*)&mFlush != 0)
            return;



        DoFinishFlush();
    }

    void DiRenderThread::WaitForFlushing()
    {
        while (!*(volatile int*)&mFlush)
        {
            if (mQuit)
                break;
            ::Sleep(0);
        }
    }

    HWND DiRenderThread::GetRenderWindowHandle()
    {
        return NULL;
    }

    void DiRenderThread::WaitForFlushOver()
    {
        while (*(volatile int*)&mFlush)
        {
            if (GetRenderWindowHandle())
            {
                MSG msg;
                while (PeekMessage(&msg, GetRenderWindowHandle(), 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
    }

    void DiRenderThread::DoFlush()
    {
        mFlush = 1;
    }

    void DiRenderThread::DoFinishFlush()
    {
        mFlush = 0;
    }

    void DiRenderThread::DoQuit()
    {
        mQuit = true;
    }
}