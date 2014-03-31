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

#include "DrvGLES2Pch.h"
#include "Win32EGLUtil.h"
#include "Win32EGLWindow.h"

namespace Demi
{
    DiWin32EGLUtil::DiWin32EGLUtil()
    {
        mNativeDisplay = GetNativeDisplay();
        mGLDisplay = GetGLDisplay();

        // Video mode possibilities
        DEVMODE DevMode;
        DevMode.dmSize = sizeof(DEVMODE);
        for (DWORD i = 0; EnumDisplaySettings(NULL, i, &DevMode); ++i)
        {
            if (DevMode.dmBitsPerPel < 16)
                continue;

            mCurrentMode.first.first = DevMode.dmPelsWidth;
            mCurrentMode.first.second = DevMode.dmPelsHeight;
            mCurrentMode.second = 0;
            mOriginalMode = mCurrentMode;
            mVideoModes.push_back(mCurrentMode);
        }

        EGLConfig *glConfigs;
        int config, nConfigs = 0;

        EGLint const attrib_list[] = 
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BUFFER_SIZE, 32,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
        };

        glConfigs = ChooseGLConfig(attrib_list, &nConfigs);

        for (config = 0; config < nConfigs; config++)
        {
            int caveat, samples;

            GetGLConfigAttrib(glConfigs[config], EGL_CONFIG_CAVEAT, &caveat);

            if (caveat != EGL_SLOW_CONFIG)
            {
                GetGLConfigAttrib(glConfigs[config], EGL_SAMPLES, &samples);
                DiString str;
                str.SetInt(samples);
                mSampleLevels.push_back(str);
            }
        }

        free(glConfigs);
    }

    DiWin32EGLUtil::~DiWin32EGLUtil()
    {
    }

    void DiWin32EGLUtil::SwitchMode(uint32& width, uint32& height, short& frequency)
    {
        if (!mRandr)
            return;

        int size = 0;
        int newSize = -1;

        VideoModes::iterator mode;
        VideoModes::iterator end = mVideoModes.end();
        VideoMode *newMode = 0;

        for (mode = mVideoModes.begin(); mode != end; size++)
        {
            if (mode->first.first >= static_cast<int>(width) &&
                mode->first.second >= static_cast<int>(height))
            {
                if (!newMode ||
                    mode->first.first < newMode->first.first ||
                    mode->first.second < newMode->first.second)
                {
                    newSize = size;
                    newMode = &(*mode);
                }
            }

            VideoMode* lastMode = &(*mode);

            while (++mode != end && mode->first == lastMode->first)
            {
                if (lastMode == newMode && mode->second == frequency)
                {
                    newMode = &(*mode);
                }
            }
        }
    }

    NativeDisplayType DiWin32EGLUtil::GetNativeDisplay()
    {
        return EGL_DEFAULT_DISPLAY; // TODO
    }

    EGLDisplay DiWin32EGLUtil::GetGLDisplay()
    {
        if (!mGLDisplay)
        {
            mNativeDisplay = GetNativeDisplay();
            return DiEGLUtil::GetGLDisplay();
        }
        return mGLDisplay;
    }

    DiWindow* DiWin32EGLUtil::CreateNewWindow()
    {
        return DI_NEW DiWin32EGLWindow(this);
    }
}