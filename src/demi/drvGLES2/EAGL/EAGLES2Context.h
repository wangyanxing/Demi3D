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

#ifndef DiDiEAGLES2Context_H__
#define DiDiEAGLES2Context_H__

#include "ES2Prerequisites.h"
#include "ES2Context.h"

#ifdef __OBJC__
#   import <QuartzCore/CAEAGLLayer.h>
#endif

namespace Demi
{
    class DI_GLES2_API DiEAGLES2Context : public DiGLES2Context
    {
        protected:
#ifdef __OBJC__
            CAEAGLLayer *mDrawable;
            EAGLContext *mContext;
#endif

        public:
#ifdef __OBJC__
            DiEAGLES2Context(CAEAGLLayer *drawable, EAGLSharegroup *group);
            CAEAGLLayer * GetDrawable() const;
            EAGLContext * GetContext() const;
#endif
            virtual ~DiEAGLES2Context();

            virtual void BeginContext();
        
            virtual void EndContext();
        
            virtual DiGLES2Context * Clone() const;

            bool CreateFramebuffer();
        
            void DestroyFramebuffer();

            /* The pixel dimensions of the backbuffer */
            GLint mBackingWidth;
            GLint mBackingHeight;

            /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
            GLuint mViewRenderbuffer;
            GLuint mViewFramebuffer;

            /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
            GLuint mDepthRenderbuffer;

            bool mIsMultiSampleSupported;
            GLsizei mNumSamples;
            GLuint mFSAAFramebuffer;
            GLuint mFSAARenderbuffer;
    };
}

#endif
