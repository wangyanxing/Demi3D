
#include "GLShaderParam.h"
#include "GLShader.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"

#include "Material.h"

namespace Demi
{
    DiGLShaderParam::DiGLShaderParam(DiMaterial& mat)
        :DiShaderParameter(mat)
    {

    }

    DiGLShaderParam::~DiGLShaderParam()
    {

    }

    void DiGLShaderParam::Bind() const
    {

    }

    void DiGLShaderParam::LoadParameters()
    {
        if (!mMaterial.GetVertexShader() || !mMaterial.GetPixelShader())
            return;

        DiGLShaderInstance* vs = static_cast<DiGLShaderInstance*>(mMaterial.GetVertexShader()->GetShader());
        DiGLShaderInstance* ps = static_cast<DiGLShaderInstance*>(mMaterial.GetPixelShader()->GetShader());

        auto gldriver = static_cast<DiGLDriver*>(Driver);
        auto linker = gldriver->GetShaderLinker(vs, ps);
        linker->LoadConstants();
    }
}