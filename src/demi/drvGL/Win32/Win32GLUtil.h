

/********************************************************************
    File:       Win32GLUtil.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GLUtil.h"

namespace Demi
{
    class DI_GLDRV_API DiWin32GLUtil : public DiGLUtil
    {
    public:

        DiWin32GLUtil();

        ~DiWin32GLUtil();

    public:

        void            InitExtensions();

        static DiString TranslateWGLError();

        void*           GetProcAddress(const DiString& procname);

        bool            SelectPixelFormat(HDC hdc, int colourDepth, int multisample, bool hwGamma);

        void            SetMainHDC(HDC dc) { mHDC = dc; }

        HDC             GetMainHDC() const { return mHDC; }

    private:

        void            InitWGL();

        DiVector<int>   mFSAALevels;

        bool            mHasPixelFormatARB;

        bool            mHasMultisample;

        bool            mHasHardwareGamma;

        HDC             mHDC;
    };
}