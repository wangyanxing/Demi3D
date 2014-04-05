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

namespace Demi
{
    DiEAGLES2Context::DiEAGLES2Context(CAEAGLLayer *drawable, EAGLSharegroup *group)
        : 
        mBackingWidth(0),
        mBackingHeight(0),
        mViewRenderbuffer(0),
        mViewFramebuffer(0),
        mDepthRenderbuffer(0),
        mIsMultiSampleSupported(false),
        mNumSamples(0),
        mFSAAFramebuffer(0),
        mFSAARenderbuffer(0)
    {

        mDrawable = [drawable retain];

        NSUInteger renderingAPI = kEAGLRenderingAPIOpenGLES2;

        // If the group argument is not NULL, then we assume that an externally created EAGLSharegroup
        // is to be used and a context is created using that group.
        if(group)
        {
            mContext = [[EAGLContext alloc] initWithAPI:renderingAPI sharegroup:group];
        }
        else
        {
            mContext = [[EAGLContext alloc] initWithAPI:renderingAPI];
        }

        if (!mContext || ![EAGLContext setCurrentContext:mContext])
        {
            DI_WARNING("Unable to create a suitable EAGLContext");
        }
        
#ifdef __IPHONE_7_1
        IF_IOS_VERSION_IS_GREATER_THAN(7.1)
        {
            [mContext setMultiThreaded:YES];
        }
#endif
    }

    DiEAGLES2Context::~DiEAGLES2Context()
    {
        //rs->_unregisterContext(this);

        DestroyFramebuffer();

        if ([EAGLContext currentContext] == mContext)
        {
            [EAGLContext setCurrentContext:nil];
        }
        
        [mContext release];
        [mDrawable release];
    }

    bool DiEAGLES2Context::CreateFramebuffer()
    {
        DestroyFramebuffer();

        CHECK_GL_ERROR(glGenFramebuffers(1, &mViewFramebuffer));
        CHECK_GL_ERROR(glGenRenderbuffers(1, &mViewRenderbuffer));
        
        CHECK_GL_ERROR(glLabelObjectEXT(GL_BUFFER_OBJECT_EXT, mViewFramebuffer, 0, "View Framebuffer"));
        CHECK_GL_ERROR(glLabelObjectEXT(GL_BUFFER_OBJECT_EXT, mViewRenderbuffer, 0, "View Renderbuffer"));
        
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mViewFramebuffer));
        CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mViewRenderbuffer));

        if(![mContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>) mDrawable])
        {
            glGetError();
            DI_WARNING("Failed to bind the drawable to a renderbuffer object");
            return false;
        }

        CHECK_GL_ERROR(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &mBackingWidth));
        CHECK_GL_ERROR(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &mBackingHeight));
        CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mViewRenderbuffer));

#if 0
        if(mIsMultiSampleSupported && mNumSamples > 0)
        {
            // Determine how many MSAA samples to use
            GLint maxSamplesAllowed;
            glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
            int samplesToUse = (mNumSamples > maxSamplesAllowed) ? maxSamplesAllowed : mNumSamples;
            
            // Create the FSAA framebuffer (offscreen)
            CHECK_GL_ERROR(glGenFramebuffers(1, &mFSAAFramebuffer));
            CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFSAAFramebuffer));

            /* Create the offscreen MSAA color buffer.
             * After rendering, the contents of this will be blitted into mFSAAFramebuffer */
            CHECK_GL_ERROR(glGenRenderbuffers(1, &mFSAARenderbuffer));
            CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mFSAARenderbuffer));
            CHECK_GL_ERROR(glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, samplesToUse, GL_RGBA8_OES, mBackingWidth, mBackingHeight));
            CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mFSAARenderbuffer));

            // Create the FSAA depth buffer
            CHECK_GL_ERROR(glGenRenderbuffers(1, &mDepthRenderbuffer));
            CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderbuffer));
            CHECK_GL_ERROR(glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, samplesToUse, GL_DEPTH_COMPONENT16, mBackingWidth, mBackingHeight));
            CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderbuffer));

            // Validate the FSAA framebuffer
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                glGetError();
                DI_WARNING("Failed to make a complete FSAA framebuffer object");
                return false;
            }
        }
        else
#endif
        {
            CHECK_GL_ERROR(glGenRenderbuffers(1, &mDepthRenderbuffer));
            CHECK_GL_ERROR(glLabelObjectEXT(GL_BUFFER_OBJECT_EXT, mDepthRenderbuffer,0,"Depth Renderbuffer"));
            CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderbuffer));
            CHECK_GL_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mBackingWidth, mBackingHeight));
            CHECK_GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderbuffer));
        }

        CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mViewRenderbuffer));
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mViewFramebuffer));
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            glGetError();
            DI_WARNING("Failed to make a complete framebuffer object");
            return false;
        }

        return true;
    }

    void DiEAGLES2Context::DestroyFramebuffer()
    {
        CHECK_GL_ERROR(glDeleteFramebuffers(1, &mViewFramebuffer));
        mViewFramebuffer = 0;
        CHECK_GL_ERROR(glDeleteRenderbuffers(1, &mViewRenderbuffer));
        mViewRenderbuffer = 0;
        
        if(mFSAARenderbuffer)
        {
            CHECK_GL_ERROR(glDeleteRenderbuffers(1, &mFSAARenderbuffer));
            mFSAARenderbuffer = 0;
        }

        if(mFSAAFramebuffer)
        {
            CHECK_GL_ERROR(glDeleteFramebuffers(1, &mFSAAFramebuffer));
            mFSAAFramebuffer = 0;
        }
        
        if(mDepthRenderbuffer)
        {
            CHECK_GL_ERROR(glDeleteRenderbuffers(1, &mDepthRenderbuffer));
            mDepthRenderbuffer = 0;
        }
    }

    void DiEAGLES2Context::BeginContext()
    {
        BOOL ret = [EAGLContext setCurrentContext:mContext];
        if (!ret)
        {
            DI_WARNING("Failed to begin the context");
        }
    }

    void DiEAGLES2Context::EndContext()
    {
    }

    DiGLES2Context * DiEAGLES2Context::Clone() const
    {
		return new DiEAGLES2Context(mDrawable, [mContext sharegroup]);
    }

	CAEAGLLayer * DiEAGLES2Context::GetDrawable() const
	{
		return mDrawable;
	}

	EAGLContext * DiEAGLES2Context::GetContext() const
	{
		return mContext;
	}
}
