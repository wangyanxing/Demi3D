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

#ifndef DiGLShaderParam_h__
#define DiGLShaderParam_h__


#include "ShaderParam.h"
#include "GLShader.h"

namespace Demi
{
    class DI_GLES2_API DiGLShaderParam : public DiShaderParameter
    {
    public:

        DiGLShaderParam(DiMaterial& mat);

        ~DiGLShaderParam();

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

        DiGLShaderLinker*   mShaderLinker;
        
        DiMap<GLuint, DiGLUniforms::BindingFunc> mBuiltinFuncs;
    };
}

#endif
