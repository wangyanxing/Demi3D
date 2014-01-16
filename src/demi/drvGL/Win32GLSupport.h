

/********************************************************************
    File:       Win32GLSupport.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GLSupport.h"

namespace Demi
{
    class DI_GLDRV_API DiWin32GLSupport : public DiGLSupport
    {
    public:

        DiWin32GLSupport();

        ~DiWin32GLSupport();

    public:

        void            InitExtensions();

        static DiString TranslateWGLError();

    private:

        void            InitWGL();

        DiVector<int>   mFSAALevels;

        bool            mHasPixelFormatARB;

        bool            mHasMultisample;

        bool            mHasHardwareGamma;

        HDC             mHDC;
    };
}