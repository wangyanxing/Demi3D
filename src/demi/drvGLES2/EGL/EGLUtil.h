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

#ifndef DiEGLUtil_h__
#define DiEGLUtil_h__

#include "ES2Util.h"

namespace Demi
{
    class DI_GLES2_API DiEGLUtil : public DiGLES2Util
    {
    public:

        DiEGLUtil();

        virtual             ~DiEGLUtil();

    public:

        void*               GetProcAddress(const DiString& procname);

        void                Start(void);

        void                Stop(void);

        EGLDisplay          GetGLDisplay(void);

        void                SetGLDisplay(EGLDisplay val);

        virtual DiString    GetDisplayName(void);
        
        EGLConfig*          ChooseGLConfig(const EGLint *attribList, EGLint *nElements);

        EGLConfig*          GetConfigs(EGLint *nElements);

        EGLBoolean          GetGLConfigAttrib(EGLConfig fbConfig, EGLint attribute, EGLint *value);

        ::EGLContext        CreateNewContext(EGLDisplay eglDisplay, ::EGLConfig glconfig, ::EGLContext shareList) const;

        ::EGLConfig         GetGLConfigFromContext(::EGLContext context);

        ::EGLConfig         GetGLConfigFromDrawable(::EGLSurface drawable,
            unsigned int *w, unsigned int *h);
        
        ::EGLConfig         SelectGLConfig(const EGLint* minAttribs, const EGLint *maxAttribs);

        void                SwitchMode(void);

        virtual void        SwitchMode(uint32& width, uint32& height, short& frequency) = 0;

    protected:

        EGLDisplay mGLDisplay;

        NativeDisplayType mNativeDisplay;

        bool mIsExternalDisplay;

        bool mRandr;
        
        typedef DiPair<uint32, uint32> ScreenSize;
        typedef short Rate;
        typedef DiPair<ScreenSize, Rate> VideoMode;
        typedef DiVector<VideoMode> VideoModes;
        
        VideoModes  mVideoModes;
        
        VideoMode   mOriginalMode;
        
        VideoMode   mCurrentMode;

        StringVec   mSampleLevels;
    };
}
#endif // Win32GLUtil_h__