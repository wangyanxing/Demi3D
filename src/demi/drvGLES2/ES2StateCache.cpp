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
#include "ES2StateCache.h"

#define ENABLE_GLES2_SATECACHE

namespace Demi
{
    DiGLES2StateCache::DiGLES2StateCache()
    {
        clearCache();
    }

    DiGLES2StateCache::~DiGLES2StateCache()
    {
        mColourMask.clear();
        mClearColour.clear();
        mActiveBufferMap.clear();
        mEnableVector.clear();
        mTexUnitsMap.clear();
    }

    void DiGLES2StateCache::initializeCache()
    {
        CHECK_GL_ERROR(glBlendEquation(GL_FUNC_ADD));
        CHECK_GL_ERROR(glBlendFunc(GL_ONE, GL_ZERO));
        CHECK_GL_ERROR(glCullFace(mCullFace));
        CHECK_GL_ERROR(glDepthFunc(mDepthFunc));
        CHECK_GL_ERROR(glDepthMask(mDepthMask));
        CHECK_GL_ERROR(glStencilMask(mStencilMask));
        CHECK_GL_ERROR(glClearDepthf(mClearDepth));
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
        CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
        CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, 0));
        CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
        CHECK_GL_ERROR(glClearColor(mClearColour[0], mClearColour[1], mClearColour[2], mClearColour[3]));
        CHECK_GL_ERROR(glColorMask(mColourMask[0], mColourMask[1], mColourMask[2], mColourMask[3]));
    }

    void DiGLES2StateCache::clearCache()
    {
        mDepthMask = GL_TRUE;
        mPolygonMode = GL_FILL;
        mBlendEquation = GL_FUNC_ADD;
        mCullFace = GL_BACK;
        mDepthFunc = GL_LESS;
        mStencilMask = 0xFFFFFFFF;
        mActiveTextureUnit = 0;
        mDiscardBuffers = 0;
        mClearDepth = 1.0f;
        mLastBoundTexID = 0;

        // Initialize our cache variables and also the GL so that the
        // stored values match the GL state
        mBlendFuncSource = GL_ONE;
        mBlendFuncDest = GL_ZERO;

        mClearColour.resize(4);
        mClearColour[0] = mClearColour[1] = mClearColour[2] = mClearColour[3] = 0.0f;

        mColourMask.resize(4);
        mColourMask[0] = mColourMask[1] = mColourMask[2] = mColourMask[3] = GL_TRUE;

        mEnableVector.reserve(25);
        mEnableVector.clear();
        mActiveBufferMap.clear();
        mTexUnitsMap.clear();

        mEnableVector.reserve(64);
        mEnabledVertexAttribs.clear();
    }

    void DiGLES2StateCache::bindGLBuffer(GLenum target, GLuint buffer, GLenum attach, bool force)
    {
#ifdef ENABLE_GLES2_SATECACHE
        bool update = false;
        auto i = mActiveBufferMap.find(target);
        if (i == mActiveBufferMap.end())
        {
            // Haven't cached this state yet.  Insert it into the map
            mActiveBufferMap.insert(BindBufferMap::value_type(target, buffer));
            update = true;
        }
        else if ((*i).second != buffer || force) // Update the cached value if needed
        {
            (*i).second = buffer;
            update = true;
        }

        // Update GL
        if (update)
        {
            if (target == GL_FRAMEBUFFER)
            {
                CHECK_GL_ERROR(glBindFramebuffer(target, buffer));
            }
            else if (target == GL_RENDERBUFFER)
            {
                CHECK_GL_ERROR(glBindRenderbuffer(target, buffer));
            }
            else
            {
                CHECK_GL_ERROR(glBindBuffer(target, buffer));
            }
        }
#else
        if (target == GL_FRAMEBUFFER)
        {
            CHECK_GL_ERROR(glBindFramebuffer(target, buffer));
        }
        else if (target == GL_RENDERBUFFER)
        {
            CHECK_GL_ERROR(glBindRenderbuffer(target, buffer));
        }
        else
        {
            CHECK_GL_ERROR(glBindBuffer(target, buffer));
        }
#endif
    }

    void DiGLES2StateCache::deleteGLBuffer(GLenum target, GLuint buffer, GLenum attach, bool force)
    {
        // Buffer name 0 is reserved and we should never try to delete it
        if (buffer == 0)
            return;

#ifdef ENABLE_GLES2_SATECACHE
        auto i = mActiveBufferMap.find(target);

        if (i != mActiveBufferMap.end() && ((*i).second == buffer || force))
        {
            if (target == GL_FRAMEBUFFER)
            {
                CHECK_GL_ERROR(glDeleteFramebuffers(1, &buffer));
            }
            else if (target == GL_RENDERBUFFER)
            {
                CHECK_GL_ERROR(glDeleteRenderbuffers(1, &buffer));
            }
            else
            {
                CHECK_GL_ERROR(glDeleteBuffers(1, &buffer));
            }

            // Currently bound buffer is being deleted, update the cached value to 0,
            // which it likely the buffer that will be bound by the driver.
            // An update will be forced next time we try to bind on this target.
            (*i).second = 0;
        }
#else
        if (target == GL_FRAMEBUFFER)
        {
            CHECK_GL_ERROR(glDeleteFramebuffers(1, &buffer));
        }
        else if (target == GL_RENDERBUFFER)
        {
            CHECK_GL_ERROR(glDeleteRenderbuffers(1, &buffer));
        }
        else
        {
            CHECK_GL_ERROR(glDeleteBuffers(1, &buffer));
        }
#endif
    }

    void DiGLES2StateCache::invalidateStateForTexture(GLuint texture)
    {
#ifdef ENABLE_GLES2_SATECACHE
        mTexUnitsMap.erase(texture);
#endif
    }

    // TODO: Store as high/low bits of a GLuint, use vector instead of map for TexParameteriMap
    void DiGLES2StateCache::setTexParameteri(GLenum target, GLenum pname, GLint param)
    {
#ifdef ENABLE_GLES2_SATECACHE
        // Check if we have a map entry for this texture id. If not, create a blank one and insert it.
        auto it = mTexUnitsMap.find(mLastBoundTexID);
        if (it == mTexUnitsMap.end())
        {
            TextureUnitParams unit;
            mTexUnitsMap[mLastBoundTexID] = unit;

            // Update the iterator
            it = mTexUnitsMap.find(mLastBoundTexID);
        }

        // Get a local copy of the parameter map and search for this parameter
        TexParameteriMap &myMap = (*it).second.mTexParameteriMap;
        auto i = myMap.find(pname);

        if (i == myMap.end())
        {
            // Haven't cached this state yet.  Insert it into the map
            myMap.insert(TexParameteriMap::value_type(pname, param));

            // Update GL
            CHECK_GL_ERROR(glTexParameteri(target, pname, param));
        }
        else
        {
            // Update the cached value if needed
            if ((*i).second != param)
            {
                (*i).second = param;

                // Update GL
                CHECK_GL_ERROR(glTexParameteri(target, pname, param));
            }
        }
#else
        CHECK_GL_ERROR(glTexParameteri(target, pname, param));
#endif
    }

    void DiGLES2StateCache::setTexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
#ifdef ENABLE_GLES2_SATECACHE
        // Check if we have a map entry for this texture id. If not, create a blank one and insert it.
        TexUnitsMap::iterator it = mTexUnitsMap.find(mLastBoundTexID);
        if (it == mTexUnitsMap.end())
        {
            TextureUnitParams unit;
            mTexUnitsMap[mLastBoundTexID] = unit;

            // Update the iterator
            it = mTexUnitsMap.find(mLastBoundTexID);
        }

        // Get a local copy of the parameter map and search for this parameter
        TexParameterfMap &myMap = (*it).second.mTexParameterfMap;
        auto i = myMap.find(pname);

        if (i == myMap.end())
        {
            // Haven't cached this state yet.  Insert it into the map
            myMap.insert(TexParameterfMap::value_type(pname, param));

            // Update GL
            CHECK_GL_ERROR(glTexParameterf(target, pname, param));
        }
        else
        {
            // Update the cached value if needed
            if ((*i).second != param)
            {
                (*i).second = param;

                // Update GL
                CHECK_GL_ERROR(glTexParameterf(target, pname, param));
            }
        }
#else
        CHECK_GL_ERROR(glTexParameterf(target, pname, param));
#endif
    }

    void DiGLES2StateCache::getTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
#ifdef ENABLE_GLES2_SATECACHE
        // Check if we have a map entry for this texture id.
        TexUnitsMap::iterator it = mTexUnitsMap.find(mLastBoundTexID);

        // Get a local copy of the parameter map and search for this parameter
        TexParameterfMap::iterator i = (*it).second.mTexParameterfMap.find(pname);

        params = &(*i).second;
#else
        CHECK_GL_ERROR(glGetTexParameterfv(target, pname, params));
#endif
    }

    void DiGLES2StateCache::bindGLTexture(GLenum target, GLuint texture)
    {
#ifdef ENABLE_GLES2_SATECACHE
        mLastBoundTexID = texture;
#endif
        // Update GL
        CHECK_GL_ERROR(glBindTexture(target, texture));
    }

    bool DiGLES2StateCache::activateGLTextureUnit(size_t unit)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mActiveTextureUnit != unit)
        {
            if (unit < MAX_TEXTURE_LAYERS)
            {
                CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + unit));
                mActiveTextureUnit = unit;
                return true;
            }
            else if (!unit)
                return true;
            else
                return false;
        }
        else
            return true;
#else
        CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + unit));
        return true;
#endif
    }

    // TODO: Store as high/low bits of a GLuint
    void DiGLES2StateCache::setBlendFunc(GLenum source, GLenum dest)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mBlendFuncSource != source || mBlendFuncDest != dest)
        {
            mBlendFuncSource = source;
            mBlendFuncDest = dest;

            CHECK_GL_ERROR(glBlendFunc(source, dest));
        }
#else
        CHECK_GL_ERROR(glBlendFunc(source, dest));
#endif
    }

    void DiGLES2StateCache::setBlendEquation(GLenum eq)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mBlendEquation != eq)
        {
            mBlendEquation = eq;

            CHECK_GL_ERROR(glBlendEquation(eq));
        }
#else
        CHECK_GL_ERROR(glBlendEquation(eq));
#endif
    }

    void DiGLES2StateCache::setDepthMask(GLboolean mask)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mDepthMask != mask)
        {
            mDepthMask = mask;

            CHECK_GL_ERROR(glDepthMask(mask));
        }
#else
        CHECK_GL_ERROR(glDepthMask(mask));
#endif
    }

    void DiGLES2StateCache::setDepthFunc(GLenum func)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mDepthFunc != func)
        {
            mDepthFunc = func;

            CHECK_GL_ERROR(glDepthFunc(func));
        }
#else
        CHECK_GL_ERROR(glDepthFunc(func));
#endif
    }

    void DiGLES2StateCache::setClearDepth(GLclampf depth)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mClearDepth != depth)
        {
            mClearDepth = depth;

            CHECK_GL_ERROR(glClearDepthf(depth));
        }
#else
        CHECK_GL_ERROR(glClearDepthf(depth));
#endif
    }

    void DiGLES2StateCache::setClearColour(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if ((mClearColour[0] != red) ||
            (mClearColour[1] != green) ||
            (mClearColour[2] != blue) ||
            (mClearColour[3] != alpha))
        {
            mClearColour[0] = red;
            mClearColour[1] = green;
            mClearColour[2] = blue;
            mClearColour[3] = alpha;

            CHECK_GL_ERROR(glClearColor(mClearColour[0], mClearColour[1], mClearColour[2], mClearColour[3]));
        }
#else
        CHECK_GL_ERROR(glClearColor(mClearColour[0], mClearColour[1], mClearColour[2], mClearColour[3]));
#endif
    }

    void DiGLES2StateCache::setColourMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if ((mColourMask[0] != red) ||
            (mColourMask[1] != green) ||
            (mColourMask[2] != blue) ||
            (mColourMask[3] != alpha))
        {
            mColourMask[0] = red;
            mColourMask[1] = green;
            mColourMask[2] = blue;
            mColourMask[3] = alpha;

            CHECK_GL_ERROR(glColorMask(mColourMask[0], mColourMask[1], mColourMask[2], mColourMask[3]));
        }
#else
        CHECK_GL_ERROR(glColorMask(mColourMask[0], mColourMask[1], mColourMask[2], mColourMask[3]));
#endif
    }

    void DiGLES2StateCache::setStencilMask(GLuint mask)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mStencilMask != mask)
        {
            mStencilMask = mask;

            CHECK_GL_ERROR(glStencilMask(mask));
        }
#else
        CHECK_GL_ERROR(glStencilMask(mask));
#endif
    }

    void DiGLES2StateCache::setEnabled(GLenum flag)
    {
#ifdef ENABLE_GLES2_SATECACHE
        bool found = std::find(mEnableVector.begin(), mEnableVector.end(), flag) != mEnableVector.end();
        if (!found)
        {
            mEnableVector.push_back(flag);

            CHECK_GL_ERROR(glEnable(flag));
        }
#else
        CHECK_GL_ERROR(glEnable(flag));
#endif
    }

    void DiGLES2StateCache::setDisabled(GLenum flag)
    {
#ifdef ENABLE_GLES2_SATECACHE
        auto iter = std::find(mEnableVector.begin(), mEnableVector.end(), flag);
        if (iter != mEnableVector.end())
        {
            mEnableVector.erase(iter);

            CHECK_GL_ERROR(glDisable(flag));
        }
#else
        CHECK_GL_ERROR(glDisable(flag));
#endif
    }

    void DiGLES2StateCache::setVertexAttribEnabled(GLuint attrib)
    {
#ifdef ENABLE_GLES2_SATECACHE
        bool found = std::find(mEnabledVertexAttribs.begin(), mEnabledVertexAttribs.end(), attrib) != mEnabledVertexAttribs.end();
        if (!found)
        {
            mEnabledVertexAttribs.push_back(attrib);

            CHECK_GL_ERROR(glEnableVertexAttribArray(attrib));
        }
#else
        CHECK_GL_ERROR(glEnableVertexAttribArray(attrib));
#endif
    }

    void DiGLES2StateCache::setVertexAttribDisabled(GLuint attrib)
    {
#ifdef ENABLE_GLES2_SATECACHE
        auto iter = std::find(mEnabledVertexAttribs.begin(), mEnabledVertexAttribs.end(), attrib);
        if (iter != mEnabledVertexAttribs.end())
        {
            mEnabledVertexAttribs.erase(iter);

            CHECK_GL_ERROR(glDisableVertexAttribArray(attrib));
        }
#else
        CHECK_GL_ERROR(glDisableVertexAttribArray(attrib));
#endif
    }

    void DiGLES2StateCache::setCullFace(GLenum face)
    {
#ifdef ENABLE_GLES2_SATECACHE
        if (mCullFace != face)
        {
            mCullFace = face;

            CHECK_GL_ERROR(glCullFace(face));
        }
#else
        CHECK_GL_ERROR(glCullFace(face));
#endif
    }
}