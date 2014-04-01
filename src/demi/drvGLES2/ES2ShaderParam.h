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

#ifndef DiGLES2ShaderParam_h__
#define DiGLES2ShaderParam_h__

#include "ES2Prerequisites.h"
#include "ShaderParam.h"
#include "ES2Shader.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2ShaderParam : public DiShaderParameter
    {
    public:

        DiGLES2ShaderParam(DiMaterial& mat);

        ~DiGLES2ShaderParam();

    public:

        void            Bind() const;

        void            LoadParameters();

        void            AddBuiltinParam(const GLuint location, DiGLUniforms::BindingFunc& func)
        {
            mBuiltinFuncs[location] = func;
        }

    private:

        void            SetConstant(GLuint location, GLenum type, const float* data, uint32 count);

    private:

        DiGLES2ShaderLinker*   mShaderLinker;
        
        DiMap<GLuint, DiGLUniforms::BindingFunc> mBuiltinFuncs;
    };
}

#endif
