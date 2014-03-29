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

#ifndef DiGLES2UniformCache_h__
#define DiGLES2UniformCache_h__

#include "ES2Prerequisites.h"

namespace Demi
{
    /** adapted from Ogre
     */
    class DI_GLES2_API DiGLES2UniformCache
    {
    public:

        DiGLES2UniformCache();

        ~DiGLES2UniformCache();

    public:

        /// Clear out the cache
        void clearCache();

        /** Update a uniform
        @return A boolean value indicating whether this uniform needs to be updated in the GL.
        */
        bool updateUniform(GLint location, const void *value, GLsizei length);
    };
}

#endif
