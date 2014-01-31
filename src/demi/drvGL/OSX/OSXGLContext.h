
/********************************************************************
    File:       OSXGLContext.h
    Creator:    demiwangya
*********************************************************************/

#ifndef OSXGLContext_h__
#define OSXGLContext_h__

#include "GLContext.h"

namespace Demi
{
    class DI_GLDRV_API DiOSXGLContext : public DiGLContext
    {
    public:

        DiOSXGLContext(DiOSXGLUtil* util, DiWndHandle wnd);

        ~DiOSXGLContext();

    public:

        void            BeginContext();

        void            EndContext();

        void            Release();

        DiGLContext*    Clone() const;

        void            SwapBuffer();

    private:

        DiOSXGLUtil*    mGLUtil;
    };
}

#endif