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


#ifndef Win32GLUtil_h__
#define Win32GLUtil_h__

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

        DiWindow*       CreateNewWindow();

        virtual bool    SupportsPBuffers();

    private:

        void            InitWGL();

        DiVector<int>   mFSAALevels;

        bool            mHasPixelFormatARB;

        bool            mHasMultisample;

        bool            mHasHardwareGamma;

        HDC             mHDC;
    };
}
#endif // Win32GLUtil_h__