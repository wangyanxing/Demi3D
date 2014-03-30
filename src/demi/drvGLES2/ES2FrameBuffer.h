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

#ifndef DiGLES2FrameBuffer_h__
#define DiGLES2FrameBuffer_h__

#include "Texture.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2FrameBuffer
    {
    public:

        DiGLES2FrameBuffer();

        ~DiGLES2FrameBuffer();

    public:

        void            AttachSurface(uint32 attachment, DiTexturePtr surface);

        void            DetarchSurface(uint32 attachment);

        void            Bind();

        void            SwapBuffers();

        void            AttachDepthBuffer(DiDepthBuffer* depthBuffer);

        void            DetachDepthBuffer();

        DiPixelFormat   GetFormat();

    private:

        void            Init();

    private:

        GLuint          mFBOId;

        DiTexturePtr    mColorBuffer[MAX_MRT];
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GLES2_API DiGLES2FBOManager
    {
    public:

        DiGLES2FBOManager(bool atimode);

        ~DiGLES2FBOManager();

    public:

        void                GetBestDepthStencil(GLenum internalFormat, GLenum *depthFormat, GLenum *stencilFormat);

    private:

        void                DetectFBOFormats();

        GLuint              TryFormat(GLenum depthFormat, GLenum stencilFormat);

        bool                TryPackedFormat(GLenum packedFormat);

        bool                CheckFormat(DiPixelFormat format) { return mProps[format].valid; }

        DiPixelFormat       GetSupportedAlternative(DiPixelFormat format);

        void                CreateTempFramebuffer(DiPixelFormat pixFmt, GLuint internalFormat, GLuint fmt, GLenum dataType, GLuint &fb, GLuint &tid);

    private:

        struct FormatProperties
        {
            bool valid; // This format can be used as RTT (FBO)

            struct Mode
            {
                size_t depth;     // Depth format (0=no depth)
                size_t stencil;   // Stencil format (0=no stencil)
            };

            DiVector<Mode> modes;
        };

        FormatProperties    mProps[PIXEL_FORMAT_MAX];

        /** Stencil and depth renderbuffers of the same format are re-used between surfaces of the
        same size and format. This can save a lot of memory when a large amount of rendertargets
        are used.
        */
        struct RBFormat
        {
            RBFormat(GLenum inFormat, size_t inWidth, size_t inHeight, uint32 fsaa) :
            format(inFormat), width(inWidth), height(inHeight), samples(fsaa)
            {}
            GLenum format;
            size_t width;
            size_t height;
            uint32 samples;
            // Overloaded comparison operator for usage in map
            bool operator < (const RBFormat &other) const
            {
                if (format < other.format)
                {
                    return true;
                }
                else if (format == other.format)
                {
                    if (width < other.width)
                    {
                        return true;
                    }
                    else if (width == other.width)
                    {
                        if (height < other.height)
                            return true;
                        else if (height == other.height)
                        {
                            if (samples < other.samples)
                                return true;
                        }
                    }
                }
                return false;
            }
        };
        struct RBRef
        {
            RBRef(){}
            RBRef(DiGLES2RenderBuffer *inBuffer) :
                buffer(inBuffer), refcount(1)
            { }
            DiGLES2RenderBuffer *buffer;
            size_t refcount;
        };
        typedef DiMap<RBFormat, RBRef> RenderBufferMap;
        RenderBufferMap mRenderBufferMap;

        GLuint mTempFBO;
    };
}

#endif