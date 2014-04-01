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
#include "GLES2Driver.h"
#include "EAGLES2Context.h"
#include "EAGL2Util.h"
#include "DiEAGL2Window.h"
#include "macUtils.h"

#import  <UIKit/UIScreen.h>

namespace Demi
{
    DiEAGL2Util::DiEAGL2Util()
    {
        mCurrentOSVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    }

    DiEAGL2Util::~DiEAGL2Util()
    {
    }

    DiString DiEAGL2Util::GetDisplayName(void)
    {
        return DiString::BLANK;
	}

    CFDictionaryRef DiEAGL2Util::ChooseGLConfig(const GLint *attribList, GLint *nElements)
    {
        // TODO: DJR - implement
        CFDictionaryRef configs = NULL;

        return configs;
    }

    GLint DiEAGL2Util::GetGLConfigAttrib(CFDictionaryRef glConfig, GLint attribute, GLint *value)
    {
        // TODO: DJR - implement
        GLint status = 0;

        return status;
    }

    CFDictionaryRef DiEAGL2Util::GetGLConfigFromContext(DiEAGLES2Context* context)
    {
        // TODO: DJR - implement
        CFDictionaryRef glConfig = 0;

        return glConfig;
    }

    CFDictionaryRef DiEAGL2Util::GetGLConfigFromDrawable(CAEAGLLayer *drawable,
                                                    unsigned int *w, unsigned int *h)
    {
        // TODO: DJR - implement
        CFDictionaryRef glConfig = 0;

        return glConfig;
    }

    //------------------------------------------------------------------------
    // A helper class for the implementation of selectFBConfig
    //------------------------------------------------------------------------
    class GLConfigAttribs
    {
        public:
            GLConfigAttribs(const int* attribs)
            {
                for (int i = 0; attribs[2*i] != 0; i++)
                {
                    fields[attribs[2*i]] = attribs[2*i+1];
                }
            }

            void load(DiEAGL2Util* const glSupport, CFDictionaryRef glConfig)
            {
                std::map<int,int>::iterator it;

                for (it = fields.begin(); it != fields.end(); it++)
                {
                    it->second = 0;

                    glSupport->GetGLConfigAttrib(glConfig, it->first, &it->second);
                }
            }

            bool operator>(GLConfigAttribs& alternative)
            {
                return false;
            }

            std::map<int,int> fields;
    };

    CFDictionaryRef DiEAGL2Util::SelectGLConfig(const int* minAttribs, const int *maxAttribs)
    {
        // TODO: DJR - implement
        CFDictionaryRef glConfig = 0;
        int nConfigs = 0;//, config;

        glConfig = ChooseGLConfig(minAttribs, &nConfigs);

        if (!nConfigs)
        {
            return 0;
        }

        return glConfig;
    }
    
    DiWindow * DiEAGL2Util::NewWindow(const DiString &name,
                                        uint32 width, uint32 height,
                                        bool fullScreen)
    {
        DiDiEAGL2Window *window = DI_NEW DiDiEAGL2Window(this);
        window->create(name, width, height, fullScreen, miscParams);

        return window;
    }

    DiEAGLES2Context * DiEAGL2Util::CreateNewContext(CFDictionaryRef &glconfig,
                                                     CAEAGLLayer *drawable, EAGLSharegroup *group) const
    {
        DiEAGLES2Context *context = new DiEAGLES2Context(drawable, group);
        if (context == NULL)
        {
            DI_WARNING("Fail to create new context");
            return context;
        }

        // Initialise GL3W
        if (gleswInit())
        {
            DI_WARNING("Failed to initialize GL3W");
        }

        return context;
    }

    void * DiEAGL2Util::GetProcAddress(const DiString& name)
    {
        return nullptr;
    }
    
    bool DiEAGL2Util::interfaceOrientationIsSupported(NSString *orientation)
    {
        NSArray *supportedOrientations = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"UISupportedInterfaceOrientations"];

        return [supportedOrientations containsObject:orientation];
    }

    bool DiEAGL2Util::portraitIsSupported()
    {
        NSArray *supportedOrientations = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"UISupportedInterfaceOrientations"];

        return ([supportedOrientations containsObject:@"UIInterfaceOrientationPortrait"] || 
                [supportedOrientations containsObject:@"UIInterfaceOrientationPortraitUpsideDown"]);
    }
}
