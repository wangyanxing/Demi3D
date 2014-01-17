
/********************************************************************
    File:       GLPrerequisites.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DiGLStateCache;
    class DiGLDriver;
    class DiGLContext;
    class DiGLUtil;
    class DiGLBufferManager;

#ifdef WIN32
    class DiWin32GLUtil;
    class DiWin32GLContext;
#endif
}