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

#ifndef DiEAGL2Util_h__
#define DiEAGL2Util_h__

#include "ES2Prerequisites.h"
#include "ES2Util.h"

#import <CoreFoundation/CoreFoundation.h>


#ifdef __OBJC__
// Forward declarations
@class CAEAGLLayer;
@class UIWindow;
@class NSString;
typedef UIWindow *NativeWindowType;
#endif

namespace Demi
{
    class DI_GLES2_API DiEAGL2Util : public DiGLES2Util
    {
        public:
            DiEAGL2Util();
        
            virtual ~DiEAGL2Util();

            void Start(void) {}
        
            void Stop(void) {}
        
            virtual DiString GetDisplayName(void);
        
			CFDictionaryRef ChooseGLConfig(const GLint *attribList, GLint *nElements);
        
            GLint GetGLConfigAttrib(CFDictionaryRef fbConfig, GLint attribute, GLint *value);
        
            float GetCurrentOSVersion(void) { return mCurrentOSVersion; }
        
            void* GetProcAddress(const DiString& name);

            DiWindow * NewWindow(const DiString& name,
                                        uint32 width, uint32 height,
                                        bool fullScreen);

#ifdef __OBJC__
            bool portraitIsSupported(void);
        
            bool interfaceOrientationIsSupported(NSString *orientation);

            DiEAGLES2Context * CreateNewContext(CFDictionaryRef &glconfig, CAEAGLLayer *drawable, EAGLSharegroup *group) const;
        
            CFDictionaryRef GetGLConfigFromContext(DiEAGLES2Context context);
        
            CFDictionaryRef GetGLConfigFromDrawable(CAEAGLLayer *drawable, unsigned int *w, unsigned int *h);
#endif
			CFDictionaryRef SelectGLConfig(const int* minAttribs, const int *maxAttribs);
        
        protected:
            float mCurrentOSVersion;
	};
}

#endif
