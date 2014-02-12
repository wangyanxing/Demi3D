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

#ifndef OSXGLUtil_h__
#define OSXGLUtil_h__

#include "GLUtil.h"

namespace Demi
{
    class DI_GLDRV_API DiOSXGLUtil : public DiGLUtil
    {
    public:

        DiOSXGLUtil();

        ~DiOSXGLUtil();

    public:

        void*           GetProcAddress(const DiString& procname);

        DiWindow*       CreateNewWindow();

    private:

        DiVector<int>   mFSAALevels;

        bool            mHasPixelFormatARB;

        bool            mHasMultisample;

        bool            mHasHardwareGamma;
    };
}

#endif