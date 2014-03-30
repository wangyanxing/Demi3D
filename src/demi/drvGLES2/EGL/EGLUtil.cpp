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
#include "EGLUtil.h"
#include <sstream>

namespace Demi
{
    //------------------------------------------------------------------------
    // A helper class for the implementation of selectFBConfig
    //------------------------------------------------------------------------
    class GLConfigAttribs
    {
    public:
        GLConfigAttribs(const int* attribs)
        {
            fields[EGL_CONFIG_CAVEAT] = EGL_NONE;

            for (int i = 0; attribs[2 * i] != EGL_NONE; i++)
            {
                fields[attribs[2 * i]] = attribs[2 * i + 1];
            }
        }

        void Load(DiEGLUtil *const glSupport, EGLConfig glConfig)
        {
            std::map<int, int>::iterator it;

            for (it = fields.begin(); it != fields.end(); it++)
            {
                it->second = EGL_NONE;

                glSupport->GetGLConfigAttrib(glConfig, it->first, &it->second);
            }
        }

        bool operator>(GLConfigAttribs& alternative)
        {
            // Caveats are best avoided, but might be needed for anti-aliasing
            if (fields[EGL_CONFIG_CAVEAT] != alternative.fields[EGL_CONFIG_CAVEAT])
            {
                if (fields[EGL_CONFIG_CAVEAT] == EGL_SLOW_CONFIG)
                {
                    return false;
                }

                if (fields.find(EGL_SAMPLES) != fields.end() &&
                    fields[EGL_SAMPLES] < alternative.fields[EGL_SAMPLES])
                {
                    return false;
                }
            }

            std::map<int, int>::iterator it;

            for (it = fields.begin(); it != fields.end(); it++)
            {
                if (it->first != EGL_CONFIG_CAVEAT &&
                    fields[it->first] > alternative.fields[it->first])
                {
                    return true;
                }
            }

            return false;
        }

        std::map<int, int> fields;
    };

    //////////////////////////////////////////////////////////////////////////

    DiEGLUtil::DiEGLUtil()
        : mGLDisplay(0),
        mNativeDisplay(0),
        mRandr(false)
    {
    }

    DiEGLUtil::~DiEGLUtil()
    {
    }

    void* DiEGLUtil::GetProcAddress(const DiString& procname)
    {
        return (void*)eglGetProcAddress((const char*)procname.c_str());
    }

    void DiEGLUtil::Start(void)
    {
    }

    void DiEGLUtil::Stop(void)
    {
        eglTerminate(mGLDisplay);
        EGL_CHECK_ERROR
    }

    EGLDisplay DiEGLUtil::GetGLDisplay(void)
    {
        EGLint major = 0, minor = 0;

        mGLDisplay = eglGetDisplay(mNativeDisplay);
        EGL_CHECK_ERROR

        if (mGLDisplay == EGL_NO_DISPLAY)
        {
            DI_WARNING("Cannot open EGLDisplay: %s", GetDisplayName().c_str());
        }

        if (eglInitialize(mGLDisplay, &major, &minor) == EGL_FALSE)
        {
            DI_WARNING("Couldn`t initialize EGLDisplay");
        }
        EGL_CHECK_ERROR
            return mGLDisplay;
    }

    DiString DiEGLUtil::GetDisplayName(void)
    {
        return DiString::BLANK;
    }

    void DiEGLUtil::SetGLDisplay(EGLDisplay val)
    {
        mGLDisplay = val;
    }

    EGLConfig* DiEGLUtil::ChooseGLConfig(const EGLint *attribList, EGLint *nElements)
    {
        EGLConfig *configs;

        if (eglChooseConfig(mGLDisplay, attribList, NULL, 0, nElements) == EGL_FALSE)
        {
            DI_WARNING("Failed to choose config");
            *nElements = 0;
            return 0;
        }
        EGL_CHECK_ERROR
        configs = (EGLConfig*)malloc(*nElements * sizeof(EGLConfig));
        if (eglChooseConfig(mGLDisplay, attribList, configs, *nElements, nElements) == EGL_FALSE)
        {
            DI_WARNING("Failed to choose config");

            *nElements = 0;
            free(configs);
            return 0;
        }
        EGL_CHECK_ERROR
        return configs;
    }

    EGLConfig* DiEGLUtil::GetConfigs(EGLint *nElements)
    {
        EGLConfig *configs;

        if (eglGetConfigs(mGLDisplay, NULL, 0, nElements) == EGL_FALSE)
        {
            DI_WARNING("Failed to choose config");

            *nElements = 0;
            return 0;
        }
        EGL_CHECK_ERROR
        configs = (EGLConfig*)malloc(*nElements * sizeof(EGLConfig));
        if (eglGetConfigs(mGLDisplay, configs, *nElements, nElements) == EGL_FALSE)
        {
            DI_WARNING("Failed to choose config");

            *nElements = 0;
            free(configs);
            return 0;
        }
        EGL_CHECK_ERROR
        return configs;
    }

    EGLBoolean DiEGLUtil::GetGLConfigAttrib(EGLConfig fbConfig, EGLint attribute, EGLint *value)
    {
        EGLBoolean status;

        status = eglGetConfigAttrib(mGLDisplay, fbConfig, attribute, value);
        EGL_CHECK_ERROR
        return status;
    }

    ::EGLContext DiEGLUtil::CreateNewContext(EGLDisplay eglDisplay, ::EGLConfig glconfig, ::EGLContext shareList) const
    {
        EGLint contextAttrs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE, EGL_NONE
        };
        ::EGLContext context = ((::EGLContext) 0);
        if (eglDisplay == ((EGLDisplay)0))
        {
            context = eglCreateContext(mGLDisplay, glconfig, shareList, contextAttrs);
            EGL_CHECK_ERROR
        }
        else
        {
            context = eglCreateContext(eglDisplay, glconfig, 0, contextAttrs);
            EGL_CHECK_ERROR
        }

        if (context == ((::EGLContext) 0))
        {
            DI_WARNING("Fail to create New context");
            return 0;
        }

        return context;
    }

    ::EGLConfig DiEGLUtil::GetGLConfigFromContext(::EGLContext context)
    {
        ::EGLConfig glConfig = 0;

        if (eglQueryContext(mGLDisplay, context, EGL_CONFIG_ID, (EGLint *)&glConfig) == EGL_FALSE)
        {
            DI_WARNING("Fail to get config from context");
            return 0;
        }
        EGL_CHECK_ERROR
        return glConfig;
    }

    ::EGLConfig DiEGLUtil::GetGLConfigFromDrawable(::EGLSurface drawable, unsigned int *w, unsigned int *h)
    {
        ::EGLConfig glConfig = 0;

        if (eglQuerySurface(mGLDisplay, drawable, EGL_CONFIG_ID, (EGLint *)&glConfig) == EGL_FALSE)
        {
            DI_WARNING("Fail to get config from drawable");
            return 0;
        }
        EGL_CHECK_ERROR
        eglQuerySurface(mGLDisplay, drawable, EGL_WIDTH, (EGLint *)w);
        EGL_CHECK_ERROR
        eglQuerySurface(mGLDisplay, drawable, EGL_HEIGHT, (EGLint *)h);
        EGL_CHECK_ERROR
        return glConfig;
    }

    ::EGLConfig DiEGLUtil::SelectGLConfig(const EGLint* minAttribs, const EGLint *maxAttribs)
    {
        EGLConfig *glConfigs;
        EGLConfig glConfig = 0;
        int config, nConfigs = 0;

        glConfigs = ChooseGLConfig(minAttribs, &nConfigs);

        if (!nConfigs)
        {
            glConfigs = GetConfigs(&nConfigs);
        }

        if (!nConfigs)
            return 0;

        glConfig = glConfigs[0];

        if (maxAttribs)
        {
            GLConfigAttribs maximum(maxAttribs);
            GLConfigAttribs best(maxAttribs);
            GLConfigAttribs candidate(maxAttribs);

            best.Load(this, glConfig);

            for (config = 1; config < nConfigs; config++)
            {
                candidate.Load(this, glConfigs[config]);

                if (candidate > maximum)
                {
                    continue;
                }

                if (candidate > best)
                {
                    glConfig = glConfigs[config];

                    best.Load(this, glConfig);
                }
            }
        }

        free(glConfigs);
        return glConfig;
    }

    void DiEGLUtil::SwitchMode(void)
    {
        return SwitchMode(mOriginalMode.first.first,
            mOriginalMode.first.second, mOriginalMode.second);
    }
}