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
#include "ES2UniformCache.h"

namespace Demi
{
    DiGLES2UniformCache::DiGLES2UniformCache()
    {
    }

    DiGLES2UniformCache::~DiGLES2UniformCache()
    {
    }

    void DiGLES2UniformCache::clearCache()
    {
    }

    bool DiGLES2UniformCache::updateUniform(GLint location, const void *value, GLsizei length)
    {
        return true;
    }
}