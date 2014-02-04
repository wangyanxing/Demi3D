
#include "DrvGLPch.h"
#include "GLShaderParam.h"
#include "GLShader.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"

#include "Material.h"

namespace Demi
{
    DiGLShaderParam::DiGLShaderParam(DiMaterial& mat)
        : DiShaderParameter(mat)
        , mShaderLinker(nullptr)
    {
    }

    DiGLShaderParam::~DiGLShaderParam()
    {
    }

    void DiGLShaderParam::Bind() const
    {
        if (!mShaderLinker || !mShaderLinker->GetGLHandle())
            return;

        // bind built-in (global) uniforms
        auto env = Driver->GetShaderEnvironment();
        for (auto it = mBuiltinFuncs.begin(); it != mBuiltinFuncs.end(); ++it)
        {
            it->second(env, it->first);
        }

        // bind custom uniforms
        for (uint32 i = 0; i < NUM_VARIABLE_TYPES; ++i)
        {
            for (auto it = mShaderParams[i].begin(); it != mShaderParams[i].end(); ++it)
            {
                const DiAny& data = it->second;
                if (data.isEmpty())
                    continue;

                GLuint location = 0;
                uint32 samplerUnit = 0;

                if (i == VARIABLE_SAMPLER2D || i == VARIABLE_SAMPLERCUBE)
                {
                    auto sampler = mShaderLinker->GetSampler(it->first);
                    if (!sampler)
                        continue;
                    location = sampler->location;
                    samplerUnit = sampler->unit;
                }
                else
                {
                    auto constant = mShaderLinker->GetConstant(it->first);
                    if (!constant)
                        continue;
                    location = constant->location;
                }

                switch (i)
                {
                case DiShaderParameter::VARIABLE_FLOAT:
                    {
                        float val = any_cast<float>(data);
                        glUniform1fvARB(location, 1, &val);
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT2:
                    {
                        DiVec2 vec2 = any_cast<DiVec2>(data);
                        glUniform2fvARB(location, 1, vec2.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT3:
                    {
                        DiVec3 vec3 = any_cast<DiVec3>(data);
                        glUniform3fvARB(location, 1, vec3.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT4:
                    {
                        DiVec4 vec4 = any_cast<DiVec4>(data);
                        glUniform4fvARB(location, 1, vec4.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_MAT4:
                    {
                        DiMat4 vec4 = any_cast<DiMat4>(data);
                        glUniformMatrix4fvARB(location, 1, GL_TRUE, vec4[0]);
                        break;
                    }
                case DiShaderParameter::VARIABLE_COLOR:
                    {
                        DiColor c = any_cast<DiColor>(data);
                        DiVec4 vec4(c.r,c.g,c.b,c.a);
                        glUniform4fvARB(location, 1, vec4.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT4_ARRAY:
                    {
                        DiPair<DiVec4*,uint32> v4Arr = any_cast<DiPair<DiVec4*,uint32>>(data);
                        glUniform4fvARB(location, v4Arr.second, v4Arr.first->ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_SAMPLER2D:
                case DiShaderParameter::VARIABLE_SAMPLERCUBE:
                    {
                        DiTexture* tex = any_cast<DiTexture*>(data);
                        tex->Bind(samplerUnit);
                    }
                    break;
                }
            }
        }
    }

    void DiGLShaderParam::LoadParameters()
    {
        if (!mMaterial.GetVertexShader() || !mMaterial.GetPixelShader())
        {
            DI_WARNING("Null vertex or pixle shader instance.");
            return;
        }

        DiGLShaderInstance* vs = static_cast<DiGLShaderInstance*>(mMaterial.GetVertexShader()->GetShader());
        DiGLShaderInstance* ps = static_cast<DiGLShaderInstance*>(mMaterial.GetPixelShader()->GetShader());

        auto gldriver = static_cast<DiGLDriver*>(Driver);
        mShaderLinker = gldriver->GetShaderLinker(vs, ps);
        
        mShaderLinker->Link();
        mShaderLinker->LoadConstants(this);
    }
}