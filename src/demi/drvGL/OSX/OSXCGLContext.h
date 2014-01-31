
/********************************************************************
    File:       OSXCGLContext.h
    Creator:    demiwangya
*********************************************************************/

#ifndef OSXCGLContext_h__
#define OSXCGLContext_h__

#include "GLContext.h"

namespace Demi
{
    class DI_GLDRV_API DiOSXCGLContext : public DiGLContext
    {
    public:

        DiOSXCGLContext(DiOSXGLUtil* util, DiWndHandle wnd);

        DiOSXCGLContext();

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