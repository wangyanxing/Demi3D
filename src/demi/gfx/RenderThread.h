
/********************************************************************
    File:       RenderThread.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Thread.h"

namespace Demi
{
    class DI_GFX_API DiRenderThread : public DiThread, public DiBase
    {
    public:

        DiRenderThread();

        ~DiRenderThread();

    public:

        void        EmitWakeupSignal();

        void        DoWork();

        void        WaitForFlushing();

        void        WaitForFlushOver();

        void        DoFlush();

        void        DoFinishFlush();

        void        DoQuit();

        HWND        GetRenderWindowHandle();

    private:

        void        RunCommands();

    private:

        bool        mQuit;

        int         mFlush;
    };
}