
/********************************************************************
    File:       D3D9Shader.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuProgram.h"
#include "ShaderParam.h"
#include "D3D9Driver.h"
#include "D3D9ShaderParam.h"
#include "D3D9StateCache.h"

namespace Demi
{
    static DiShaderParameter::ParamType GetHlslVarType(const D3DXCONSTANT_DESC &desc)
    {
        DiShaderParameter::ParamType vt = DiShaderParameter::NUM_VARIABLE_TYPES;
        switch (desc.Type)
        {
        case D3DXPT_FLOAT:
            if (desc.Rows == 1 && desc.Columns == 1)
                vt = DiShaderParameter::VARIABLE_FLOAT;
            else if (desc.Rows == 1 && desc.Columns == 2)
                vt = DiShaderParameter::VARIABLE_FLOAT2;
            else if (desc.Rows == 1 && desc.Columns == 3)
                vt = DiShaderParameter::VARIABLE_FLOAT3;
            else if (desc.Rows == 1 && desc.Columns == 4)
                vt = DiShaderParameter::VARIABLE_FLOAT4;
            else if (desc.Rows == 4 && desc.Columns == 4)
                vt = DiShaderParameter::VARIABLE_MAT4;
            else if (desc.Rows > 1 && desc.Columns == 4)
                vt = DiShaderParameter::VARIABLE_FLOAT4_ARRAY;
            break;
        case D3DXPT_SAMPLER2D:
            vt = DiShaderParameter::VARIABLE_SAMPLER2D;
            break;
        case D3DXPT_SAMPLERCUBE:
            vt = DiShaderParameter::VARIABLE_SAMPLERCUBE;
            break;
        }
        return vt;
    }

    //////////////////////////////////////////////////////////////////////////

    class DI_D3D9DRV_API DiShaderConstants : DiBase
    {
    public:
        ID3DXConstantTable* table;

        DiShaderType type;

        int modelMatrix;
        int viewMatrix;
        int projMatrix;
        int modelViewMatrix;
        int modelViewProjMatrix;
        int viewProjMatrix;
        int texViewProjMatrix;

        int eyePosition;
        int eyeDirection;
        int eyePositionObjSpace;

        int farnearPlane;

        int boneMatrices;
        int modelMatrices;

        int globalAmbient;
        int ambientColor;
        int diffuseColor;
        int specularColor;
        int opacity;
        int shininess;

        int time;
        int viewportSize;
        int texelOffsets;

        int numDirLights;
        int dirLightsColor;
        int dirLightsDir;

        int numPointLights;
        int pointLightsColor;
        int pointLightsPosition;
        int pointLightsAttenuation;

        int hasSkyLight;
        int skyLightColor;
        int groundColor;
        int skyLightDir;

    public:

        DiShaderConstants(DiShaderType tp);

        ~DiShaderConstants(void);

        void    LoadConstants(void);

        void    BindEnvironment(const DiShaderEnvironment& shaderEnv) const;

        int     GetRegisterID(const char* name);
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_D3D9DRV_API DiShaderIncluder : public ID3DXInclude
    {
    private:
        STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE includeType, LPCSTR fileName,
            LPCVOID parentData, LPCVOID *data, uint32 *dataSize);

        STDMETHOD(Close)(THIS_ LPCVOID data);
    };

    template<DiShaderType type, class T>
    class DiD3D9ShaderInstance : public DiShaderInstance
    {
    public:

        typedef DiD3D9ShaderInstance<type, T> _Myt;

        DiD3D9ShaderInstance(DiShaderProgram* prog)
            : DiShaderInstance(type)
            , mShaderProgram(prog)
            , mD3DShader(nullptr)
            , mConstants(type)
        {
        }

        bool                Compile(const DiString& code)
        {
            return DiD3D9ShaderUtils<type, _Myt>::CompileShader(this, code);
        }

        void                Load(DWORD* data)
        {
            return DiD3D9ShaderUtils<type, _Myt*>::LoadShader(this, data);
        }

        void                Bind(const DiShaderEnvironment& shaderEnv)
        {
            DiD3D9ShaderUtils<type, _Myt*>::BindShader(this);
            mConstants.BindEnvironment(shaderEnv);
        }

        void                LoadParameters(DiD3D9ShaderParam* sm)
        {
            ID3DXConstantTable &table = *mConstants.table;

            D3DXCONSTANTTABLE_DESC desc;
            table.GetDesc(&desc);
            for (uint32 i = 0; i < desc.Constants; i++)
            {
                D3DXHANDLE constant = table.GetConstant(0, i);
                DI_ASSERT(constant);
                if (constant)
                {
                    D3DXCONSTANT_DESC cdesc;
                    uint32 count = 1;
                    table.GetConstantDesc(constant, &cdesc, &count);
                    DI_ASSERT(count == 1);
                    if (count == 1 && strncmp(cdesc.Name, "g_", 2))
                    {
                        DiShaderParameter::ParamType t = GetHlslVarType(cdesc);
                        DiString parName = cdesc.Name;
                        sm->AddParameter(t, parName);
                        sm->AddD3DParameter(parName, type, cdesc.RegisterIndex);
                    }
                }
            }
        }

        void                Release()
        {
            SAFE_RELEASE(mD3DShader);
        }

        DiShaderType        GetType()
        {
            return type;
        }

        DiShaderProgram*    mShaderProgram;

        T*                  mD3DShader;

        DiShaderConstants   mConstants;
    };

    typedef DiD3D9ShaderInstance<SHADER_VERTEX, IDirect3DVertexShader9> DiD3D9VSInstance;
    typedef DiD3D9ShaderInstance<SHADER_PIXEL, IDirect3DPixelShader9>   DiD3D9PSInstance;

    template< DiShaderType shaderTp, typename T >
    class DiD3D9ShaderUtils
    {
    public:

        static bool CompileShader(T* shaderInstance, const DiString& code)
        {
            DiMarcoDefineList marcos;
            marcos = shaderInstance->mCompileDesc.marcos;

            D3DXMACRO* defs = nullptr;
            defs = DI_NEW D3DXMACRO[marcos.size() + 1];
            for (size_t d = 0; d < marcos.size(); d++)
            {
                defs[d].Name = &marcos[d].first[0];
                defs[d].Definition = &marcos[d].second[0];
            }

            defs[marcos.size()].Name = nullptr;
            defs[marcos.size()].Definition = nullptr;

            LPD3DXBUFFER dXShaderBuffer = nullptr;
            LPD3DXBUFFER dXErrorBuffer = nullptr;

            DiShaderIncluder includer;
            const DWORD flag = D3DXSHADER_PACKMATRIX_COLUMNMAJOR | D3DXSHADER_OPTIMIZATION_LEVEL3;

            HRESULT hResult = D3DXCompileShader(
                code.c_str(),
                code.length(),
                defs,
                &includer,
                shaderInstance->mCompileDesc.entryName.c_str(),
                shaderInstance->mCompileDesc.profile.c_str(),
                flag,
                &dXShaderBuffer,
                &dXErrorBuffer,
                &shaderInstance->mConstants.table);

            if (hResult != S_OK)
            {
                DiString errorLog(reinterpret_cast <char*> (dXErrorBuffer->GetBufferPointer()));
                DiString::size_type i = errorLog.rfind("\\memory(");
                if (i != -1)
                {
                    errorLog = "Shader compile failed:" + errorLog.substr(i + 7);
                }

                DI_WARNING(errorLog.c_str());
            }

            SAFE_ARRAY_DELETE(defs);

            shaderInstance->Load(reinterpret_cast<DWORD*>(dXShaderBuffer->GetBufferPointer()));

            SAFE_RELEASE(dXShaderBuffer);
            SAFE_RELEASE(dXErrorBuffer);

            return hResult == S_OK;
        }

        template<DiShaderType shaderTp, typename T>
        static void BindShader(T shaderInstance)
        {
            static_assert(false, "unsupported class type!");
        }

        template<DiShaderType shaderTp, typename T>
        static void LoadShader(T shaderInstance, DWORD* data)
        {
            static_assert(false, "unsupported class type!");
        }
    };

    template<>
    class DiD3D9ShaderUtils<SHADER_VERTEX, DiD3D9VSInstance*>
    {
    public:
        static void BindShader(DiD3D9VSInstance* shaderInstance)
        {
            DiD3D9Driver::StateCache->SetVertexShader(shaderInstance->mD3DShader);
        }

        static void LoadShader(DiD3D9VSInstance* shaderInstance, DWORD* data)
        {
            if (shaderInstance->mD3DShader)
                shaderInstance->mD3DShader->Release();

            shaderInstance->mD3DShader = DiD3D9Driver::CreateVertexShader(data);
            DI_ASSERT(shaderInstance->mD3DShader);

            shaderInstance->mConstants.LoadConstants();
        }
    };

    template<>
    class DiD3D9ShaderUtils<SHADER_PIXEL, DiD3D9PSInstance*>
    {
    public:
        static void BindShader(DiD3D9PSInstance* shaderInstance)
        {
            DiD3D9Driver::StateCache->SetPixelShader(shaderInstance->mD3DShader);
        }

        static void LoadShader(DiD3D9PSInstance* shaderInstance, DWORD* data)
        {
            if (shaderInstance->mD3DShader)
                shaderInstance->mD3DShader->Release();

            shaderInstance->mD3DShader = DiD3D9Driver::CreatePixelShader(data);
            DI_ASSERT(shaderInstance->mD3DShader);

            shaderInstance->mConstants.LoadConstants();
        }
    };
}