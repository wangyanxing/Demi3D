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

#ifndef Win32EGLUtil_h__
#define Win32EGLUtil_h__

#include "EGLUtil.h"

namespace Demi
{
    class DI_GLES2_API DiWin32EGLUtil : public DiEGLUtil
    {
    public:

        DiWin32EGLUtil();

        ~DiWin32EGLUtil();

    public:

        virtual void SwitchMode(uint32& width, uint32& height, short& frequency);

        //Moved getNativeDisplay to native class
        NativeDisplayType GetNativeDisplay();

        //This sets up the native variables then calls EGLSupport::getGLDisplay
        EGLDisplay GetGLDisplay();

        DiWindow*  CreateNewWindow();
    };
}

#endif