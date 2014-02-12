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
#if 0
        while (!*(volatile int*)&mFlush)
        {
            if (mQuit)
                break;
            ::Sleep(0);
        }
#endif
    }

    DiWndHandle DiRenderThread::GetRenderWindowHandle()
    {
        return NULL;
    }

    void DiRenderThread::WaitForFlushOver()
    {
#if 0
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
#endif
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