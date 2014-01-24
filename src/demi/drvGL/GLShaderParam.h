
/********************************************************************
    File:       GLShaderVariable.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "ShaderParam.h"

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

    private:

    };
}