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

#ifndef DiRenderThread_h__
#define DiRenderThread_h__


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

        DiWndHandle GetRenderWindowHandle();

    private:

        void        RunCommands();

    private:

        bool        mQuit;

        int         mFlush;
    };
}

#endif
