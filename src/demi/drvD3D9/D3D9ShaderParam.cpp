
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
                case DiGpuVariable::VARIABLE_FLOAT:
                    {
                        float val = any_cast<float>(data);
                        const float fdata[4] = {val, 0,0,0};
                
                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID,fdata, 1);
                        else
                            sm->SetPixelShaderConstantF(regID, fdata, 1);

                        break;
                    }
                case DiGpuVariable::VARIABLE_FLOAT2:
                    {
                        DiVec2 vec2 = any_cast<DiVec2>(data);
                        const float fdata[4] = {vec2.x, vec2.y, 0, 0};

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, fdata, 1);
                        else
                            sm->SetPixelShaderConstantF(regID, fdata, 1);

                        break;
                    }
                case DiGpuVariable::VARIABLE_FLOAT3:
                    {
                        DiVec3 vec3 = any_cast<DiVec3>(data);
                        const float fdata[4] = {vec3.x, vec3.y, vec3.z, 0};
                
                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, fdata, 1);
                        else
                            sm->SetPixelShaderConstantF(regID, fdata, 1);

                        break;
                    }
                case DiGpuVariable::VARIABLE_FLOAT4:
                    {
                        DiVec4 vec4 = any_cast<DiVec4>(data);

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, vec4.ptr(), 1);
                        else
                            sm->SetPixelShaderConstantF(regID, vec4.ptr(), 1);

                        break;
                    }
                case DiGpuVariable::VARIABLE_COLOR:
                    {
                        DiColor c = any_cast<DiColor>(data);
                        DiVec4 vec4(c.r,c.g,c.b,c.a);

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, vec4.ptr(), 1);
                        else
                            sm->SetPixelShaderConstantF(regID, vec4.ptr(), 1);

                        break;
                    }
                case DiGpuVariable::VARIABLE_FLOAT4_ARRAY:
                    {
                        DiPair<DiVec4*,uint32> v4Arr = any_cast<DiPair<DiVec4*,uint32>>(data);

                        if (type == SHADER_VERTEX)
                            sm->SetVertexShaderConstantF(regID, v4Arr.first->ptr(), v4Arr.second);
                        else
                            sm->SetPixelShaderConstantF(regID, v4Arr.first->ptr(), v4Arr.second);

                        break;
                    }
                case DiGpuVariable::VARIABLE_SAMPLER2D:
                case DiGpuVariable::VARIABLE_SAMPLERCUBE:
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
            DiD3D9VSInstance* ps = static_cast<DiD3D9VSInstance*>(mMaterial.GetPixelShader()->GetShader());
            ps->LoadParameters(this);
        }
    }

}