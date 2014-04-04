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
#include "ES2ShaderParam.h"
#include "ES2Shader.h"
#include "GLES2Driver.h"
#include "ES2TypeMappings.h"

#include "Material.h"

namespace Demi
{
    DiGLES2ShaderParam::DiGLES2ShaderParam(DiMaterial& mat)
        : DiShaderParameter(mat)
        , mShaderLinker(nullptr)
    {
    }

    DiGLES2ShaderParam::~DiGLES2ShaderParam()
    {
    }

    void DiGLES2ShaderParam::Bind() const
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
                int samplerUnit = 0;

                if (i == VARIABLE_SAMPLER2D || i == VARIABLE_SAMPLERCUBE)
                {
                    auto sampler = mShaderLinker->GetSampler(it->first);
                    if (!sampler)
                        continue;
                    location = sampler->location;
                    samplerUnit = (int)sampler->unit;
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
                        glUniform1fv(location, 1, &val);
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT2:
                    {
                        DiVec2 vec2 = any_cast<DiVec2>(data);
                        glUniform2fv(location, 1, vec2.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT3:
                    {
                        DiVec3 vec3 = any_cast<DiVec3>(data);
                        glUniform3fv(location, 1, vec3.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT4:
                    {
                        DiVec4 vec4 = any_cast<DiVec4>(data);
                        glUniform4fv(location, 1, vec4.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_MAT4:
                    {
                        DiMat4 vec4 = any_cast<DiMat4>(data);
                        vec4.transpose(vec4);
                        glUniformMatrix4fv(location, 1, GL_FALSE, vec4[0]);
                        break;
                    }
                case DiShaderParameter::VARIABLE_COLOR:
                    {
                        DiColor c = any_cast<DiColor>(data);
                        DiVec4 vec4(c.r,c.g,c.b,c.a);
                        glUniform4fv(location, 1, vec4.ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT4_ARRAY:
                    {
                        DiPair<DiVec4*,uint32> v4Arr = any_cast<DiPair<DiVec4*,uint32>>(data);
                        glUniform4fv(location, v4Arr.second, v4Arr.first->ptr());
                        break;
                    }
                case DiShaderParameter::VARIABLE_SAMPLER2D:
                case DiShaderParameter::VARIABLE_SAMPLERCUBE:
                    {
                        glUniform1iv(location, 1, &samplerUnit);
                        DiTexture* tex = any_cast<DiTexture*>(data);
                        tex->Bind((uint32)samplerUnit);
                    }
                    break;
                }
            }
        }
    }

    void DiGLES2ShaderParam::LoadParameters()
    {
        if (!mMaterial.GetVertexShader() || !mMaterial.GetPixelShader())
        {
            DI_WARNING("Null vertex or pixle shader instance.");
            return;
        }

        DiGLES2ShaderInstance* vs = static_cast<DiGLES2ShaderInstance*>(mMaterial.GetVertexShader()->GetShader());
        DiGLES2ShaderInstance* ps = static_cast<DiGLES2ShaderInstance*>(mMaterial.GetPixelShader()->GetShader());

        auto gldriver = static_cast<DiGLES2Driver*>(Driver);
        mShaderLinker = gldriver->GetShaderLinker(vs, ps);
        
        mShaderLinker->Link();
        mShaderLinker->LoadConstants(this);
    }
}