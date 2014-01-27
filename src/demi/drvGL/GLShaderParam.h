
/********************************************************************
    File:       GLShaderVariable.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "ShaderParam.h"
#include "GLShader.h"

namespace Demi
{
    class DI_GLDRV_API DiGLShaderParam : public DiShaderParameter
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