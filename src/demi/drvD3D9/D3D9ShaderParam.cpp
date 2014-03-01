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
#include "DrvD3D9Pch.h"
#include "D3D9ShaderParam.h"
#include "D3D9Shader.h"

#include "Material.h"
#include "AssetManager.h"
#include "Texture.h"

namespace Demi
{

    DiD3D9ShaderParam::DiD3D9ShaderParam(DiMaterial& mat)
        :DiShaderParameter(mat)
    {
    }

    DiD3D9ShaderParam::~DiD3D9ShaderParam()
    {
    }

    void DiD3D9ShaderParam::Bind() const
    {
        IDirect3DDevice9* sm = DiD3D9Driver::Device;

        auto env = Driver->GetShaderEnvironment();
        
        // bind built-in (global) uniforms
        for (auto it = mBuiltinFuncs.begin(); it != mBuiltinFuncs.end(); ++it)
            it->func(env,it->type,it->reg);

        for (uint32 i = 0; i < NUM_VARIABLE_TYPES; ++i)
        {
            for (auto it = mShaderParams[i].begin(); it != mShaderParams[i].end(); ++it)
            {
                const DiAny& data = it->second;
                if (data.isEmpty())
                    continue;
                
                DiString name = it->first;

                auto d3dstuff = mD3DConsts.find(name);
                if (d3dstuff == mD3DConsts.end())
                    continue;

                DiShaderType type = d3dstuff->second.first;
                uint32 regID = d3dstuff->second.second;

                switch (i)
                {
                case DiShaderParameter::VARIABLE_FLOAT:
                    {
                        float val = any_cast<float>(data);
                        const float fdata[4] = {val, 0,0,0};
                
                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID,fdata, 1);
                        else
                            sm->SetPixelShaderConstantF(regID, fdata, 1);

                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT2:
                    {
                        DiVec2 vec2 = any_cast<DiVec2>(data);
                        const float fdata[4] = {vec2.x, vec2.y, 0, 0};

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, fdata, 1);
                        else
                            sm->SetPixelShaderConstantF(regID, fdata, 1);

                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT3:
                    {
                        DiVec3 vec3 = any_cast<DiVec3>(data);
                        const float fdata[4] = {vec3.x, vec3.y, vec3.z, 0};
                
                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, fdata, 1);
                        else
                            sm->SetPixelShaderConstantF(regID, fdata, 1);

                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT4:
                    {
                        DiVec4 vec4 = any_cast<DiVec4>(data);

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, vec4.ptr(), 1);
                        else
                            sm->SetPixelShaderConstantF(regID, vec4.ptr(), 1);

                        break;
                    }
                case DiShaderParameter::VARIABLE_MAT4:
                {
                    DiMat4 mat4 = any_cast<DiMat4>(data);

                    if (type == SHADER_VERTEX)
                        sm->SetVertexShaderConstantF(regID, mat4[0], 4);
                    else
                        sm->SetPixelShaderConstantF(regID, mat4[0], 4);

                    break;
                }
                case DiShaderParameter::VARIABLE_COLOR:
                    {
                        DiColor c = any_cast<DiColor>(data);
                        DiVec4 vec4(c.r,c.g,c.b,c.a);

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, vec4.ptr(), 1);
                        else
                            sm->SetPixelShaderConstantF(regID, vec4.ptr(), 1);

                        break;
                    }
                case DiShaderParameter::VARIABLE_FLOAT4_ARRAY:
                    {
                        DiPair<DiVec4*,uint32> v4Arr = any_cast<DiPair<DiVec4*,uint32>>(data);

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, v4Arr.first->ptr(), v4Arr.second);
                        else
                            sm->SetPixelShaderConstantF(regID, v4Arr.first->ptr(), v4Arr.second);

                        break;
                    }
                case DiShaderParameter::VARIABLE_SAMPLER2D:
                case DiShaderParameter::VARIABLE_SAMPLERCUBE:
                    {
                        DiTexture* tex = any_cast<DiTexture*>(data);
                        tex->Bind(regID);
                    }
                    break;
                }
            }
        }
    }

    void DiD3D9ShaderParam::AddD3DParameter(const DiString& name, DiShaderType shadertype, uint32 regID)
    {
        mD3DConsts[name].first  = shadertype;
        mD3DConsts[name].second = regID;
    }

    void DiD3D9ShaderParam::LoadParameters()
    {
        if (mMaterial.GetVertexShader())
        {
            DiD3D9VSInstance* vs = static_cast<DiD3D9VSInstance*>(mMaterial.GetVertexShader()->GetShader());
            vs->LoadParameters(this);
        }
        if (mMaterial.GetPixelShader())
        {
            DiD3D9PSInstance* ps = static_cast<DiD3D9PSInstance*>(mMaterial.GetPixelShader()->GetShader());
            ps->LoadParameters(this);
        }
    }


    DiD3D9BuiltinConsts::BuiltinFuncs DiD3D9BuiltinConsts::msUniformFuncs;

    void DiD3D9BuiltinConsts::InitBuiltinFuncs()
    {
        msUniformFuncs["g_modelMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->modelMatrix);
        };

        msUniformFuncs["g_viewMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->viewMatrix);
        };

        msUniformFuncs["g_projMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->projMatrix);
        };

        msUniformFuncs["g_modelViewMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->modelViewMatrix);
        };

        msUniformFuncs["g_modelViewProjMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->modelViewProjMatrix);
        };

        msUniformFuncs["g_viewProjMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->viewProjMatrix);
        };

        msUniformFuncs["g_texMatrix"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->texMatrix);
        };

        msUniformFuncs["g_boneMatrices"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetVertexShaderConstantF(location, (float*)(&env->boneMatrices), env->numBones * 3);
        };

        msUniformFuncs["g_modelMatrices"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetVertexShaderConstantF(location, (float*)(&env->modelMatrices), env->numModelMatrices * 3);
        };

        msUniformFuncs["g_eyePosition"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->eyePosition);
        };

        msUniformFuncs["g_eyeDirection"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->eyeDirection);
        };

        msUniformFuncs["g_farnearPlane"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->farnearPlane);
        };

        msUniformFuncs["g_time"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->time);
        };

        msUniformFuncs["g_viewportSize"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->viewportSize);
        };

        msUniformFuncs["g_globalAmbient"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->globalAmbient);
        };

        msUniformFuncs["g_ambientColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->ambientColor);
        };

        msUniformFuncs["g_diffuseColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->diffuseColor);
        };

        msUniformFuncs["g_specularColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->specularColor);
        };

        msUniformFuncs["g_opacity"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->opacity);
        };

        msUniformFuncs["g_shininess"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->shininess);
        };

        msUniformFuncs["g_texelOffsets"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->texelOffsets);
        };

        msUniformFuncs["g_numDirLights"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->numDirLights);
        };

        msUniformFuncs["g_dirLightsColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetPixelShaderConstantF(location, (float*)(&env->dirLightsColor[0].r), env->numDirLights);
        };

        msUniformFuncs["g_dirLightsDir"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetPixelShaderConstantF(location, (float*)(&env->dirLightsDir[0].x), env->numDirLights);
        };

        msUniformFuncs["g_numPointLights"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->numPointLights);
        };

        msUniformFuncs["g_pointLightsColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetPixelShaderConstantF(location, (float*)(&env->pointLightsColor[0].r), env->numPointLights);
        };

        msUniformFuncs["g_pointLightsPosition"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetPixelShaderConstantF(location, (float*)(&env->pointLightsPosition[0].x), env->numPointLights);
        };

        msUniformFuncs["g_pointLightsAttenuation"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetPixelShaderConstantF(location, (float*)(&env->pointLightsAttenuation[0].x), env->numPointLights);
        };

        msUniformFuncs["g_hasSkyLight"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->hasSkyLight);
        };

        msUniformFuncs["g_skyLightColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->skyLightColor);
        };

        msUniformFuncs["g_groundColor"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->groundColor);
        };

        msUniformFuncs["g_skyLightDir"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->skyLightDir);
        };

        msUniformFuncs["g_fixedDepthBias"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->fixedDepthBias);
        };

        msUniformFuncs["g_gradientScaleBias"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->gradientScaleBias);
        };

        msUniformFuncs["g_shadowMapParams"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->shadowMapParams);
        };

        msUniformFuncs["g_firstCascadeTexMat"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            static_cast<DiD3D9Driver*>(DiBase::Driver)->SetShaderConsts(type, location, env->firstCascadeTexMat);
        };

        msUniformFuncs["g_texMatrixScaleBias"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            DiD3D9Driver::Device->SetPixelShaderConstantF(location, (float*)(&env->texMatrixScaleBias[0].x), (env->numShadowCascades - 1));
        };

        msUniformFuncs["g_shadowTexture0"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            env->shadowTexture[0]->Bind(location);
        };

        msUniformFuncs["g_shadowTexture1"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            env->shadowTexture[1]->Bind(location);
        };

        msUniformFuncs["g_shadowTexture2"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            env->shadowTexture[2]->Bind(location);
        };

        msUniformFuncs["g_shadowTexture3"] = [](const DiShaderEnvironment* env, DiShaderType type, uint32 location) {
            env->shadowTexture[3]->Bind(location);
        };
    }
}