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

#ifndef DiGLES2StateCache_h__
#define DiGLES2StateCache_h__

#include "ES2Prerequisites.h"

namespace Demi
{
    /** adapted from Ogre
     */
    class DI_GLES2_API DiGLES2StateCache
    {
    public:

        DiGLES2StateCache();

        ~DiGLES2StateCache();

    public:

        void initializeCache();

        void clearCache();

        void bindGLBuffer(GLenum target, GLuint buffer, GLenum attach = 0, bool force = false);

        void deleteGLBuffer(GLenum target, GLuint buffer, GLenum attach = 0, bool force = false);

        void bindGLTexture(GLenum target, GLuint texture);

        void setTexParameteri(GLenum target, GLenum pname, GLint param);

        void setTexParameterf(GLenum target, GLenum pname, GLfloat params);

        void getTexParameterfv(GLenum target, GLenum pname, GLfloat *params);

        void invalidateStateForTexture(GLuint texture);

        bool activateGLTextureUnit(size_t unit);

        GLenum getBlendEquation(void) const { return mBlendEquation; }

        void setBlendEquation(GLenum eq);

        void setBlendFunc(GLenum source, GLenum dest);

        GLboolean getDepthMask(void) const { return mDepthMask; }

        void setDepthMask(GLboolean mask);

        GLenum getDepthFunc(void) const { return mDepthFunc; }

        void setDepthFunc(GLenum func);

        GLclampf getClearDepth(void) const { return mClearDepth; }

        void setClearDepth(GLclampf depth);

        void setClearColour(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

        DiVector<GLboolean> & getColourMask(void) { return mColourMask; }

        void setColourMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

        GLuint getStencilMask(void) const { return mStencilMask; }

        void setStencilMask(GLuint mask);

        void setEnabled(GLenum flag);

        void setDisabled(GLenum flag);

        void setVertexAttribEnabled(GLuint attrib);

        void setVertexAttribDisabled(GLuint attrib);

        unsigned int getDiscardBuffers(void) const { return mDiscardBuffers; }

        void setDiscardBuffers(unsigned int flags) { mDiscardBuffers = flags; }

        GLenum getPolygonMode(void) const { return mPolygonMode; }

        void setPolygonMode(GLenum mode) { mPolygonMode = mode; }

        GLenum getCullFace(void) const { return mCullFace; }

        void setCullFace(GLenum face);

    private:
        typedef DiHashMap<GLenum, GLuint> BindBufferMap;
        typedef DiHashMap<GLenum, GLint> TexParameteriMap;
        typedef DiHashMap<GLenum, GLfloat> TexParameterfMap;

        struct TextureUnitParams
        {
            ~TextureUnitParams()
            {
                mTexParameteriMap.clear();
                mTexParameterfMap.clear();
            }

            TexParameteriMap mTexParameteriMap;
            TexParameterfMap mTexParameterfMap;
        };

        typedef DiHashMap<GLuint, TextureUnitParams> TexUnitsMap;

        /* These variables are used for caching OpenGL state.
        They are cached because state changes can be quite expensive,
        which is especially important on mobile or embedded systems.
        */

        /// A map of different buffer types and the currently bound buffer for each type
        BindBufferMap mActiveBufferMap;
        /// A map of texture parameters for each texture unit
        TexUnitsMap mTexUnitsMap;
        /// Array of each OpenGL feature that is enabled i.e. blending, depth test, etc.
        DiVector<GLenum> mEnableVector;
        /// Stores the current clear colour
        DiVector<GLclampf> mClearColour;
        /// Stores the current colour write mask
        DiVector<GLboolean> mColourMask;
        /// Stores the currently enabled vertex attributes
        DiVector<GLuint> mEnabledVertexAttribs;
        /// Stores the current depth write mask
        GLboolean mDepthMask;
        /// Stores the current polygon rendering mode
        GLenum mPolygonMode;
        /// Stores the current blend equation
        GLenum mBlendEquation;
        /// Stores the current blend source function
        GLenum mBlendFuncSource;
        /// Stores the current blend destination function
        GLenum mBlendFuncDest;
        /// Stores the current face culling setting
        GLenum mCullFace;
        /// Stores the current depth test function
        GLenum mDepthFunc;
        /// Stores the current stencil mask
        GLuint mStencilMask;
        /// Stores the last bound texture id
        GLuint mLastBoundTexID;
        /// Stores the currently active texture unit
        GLenum mActiveTextureUnit;
        /// Mask of buffers who contents can be discarded if GL_EXT_discard_framebuffer is supported
        unsigned int mDiscardBuffers;
        /// Stores the current depth clearing colour
        GLclampf mClearDepth;
    };
}

#endif
