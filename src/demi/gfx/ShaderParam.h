
/********************************************************************
    File:       ShaderParam.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPrerequisites.h"
#include "Texture.h"
#include "GpuProgram.h"
#include "ShaderProgram.h"

namespace Demi 
{
    class DI_GFX_API DiShaderParameter : public DiBase
    {
    public:

        friend class DiMaterial;

    public:

        DiShaderParameter(DiMaterial& mat);

        virtual ~DiShaderParameter();

        typedef DiHashMap<DiString, DiAny>  ShaderParamMap;

        enum ParamType
        {
            VARIABLE_FLOAT = 0,
            VARIABLE_FLOAT2,
            VARIABLE_FLOAT3,
            VARIABLE_COLOR,
            VARIABLE_FLOAT4,
            VARIABLE_FLOAT4_ARRAY,

            VARIABLE_SAMPLER2D,
            VARIABLE_SAMPLERCUBE,

            NUM_VARIABLE_TYPES
        };

    public:

        void                    AddParameter(ParamType type, const DiString& name);

        const ShaderParamMap&   GetShaderParams(ParamType type) { return mShaderParams[type]; }

        virtual void            LoadParameters() = 0;

        void                    CloneVarsTo(DiShaderParameter* ps);

        void                    WriteFloat4(const DiString& name, DiVec4 vec4);

        void                    WriteColor(const DiString& name, DiColor vec4);

        void                    WriteFloat4Array(const DiString& name, DiVec4* vec4,uint32 size);

        void                    WriteFloat3(const DiString& name, DiVec3 vec3);

        void                    WriteFloat2(const DiString& name, DiVec2 vec2);

        void                    WriteFloat(const DiString& name, float);

        DiTexturePtr            WriteTexture2D(const DiString& name, const DiString& textureName);

        void                    WriteTexture2D(const DiString& name,DiTexturePtr texture);

        DiTexturePtr            WriteTextureCUBE(const DiString& name, const DiString& textureName);

        void                    WriteTextureCUBE(const DiString& name,DiTexturePtr texture);

        virtual void            Bind() const = 0;

        bool                    HasVariableType(ParamType varType);

        static DiAny            GetDefault(ParamType type);

    protected:

        DiMaterial&             mMaterial;

        ShaderParamMap          mShaderParams[NUM_VARIABLE_TYPES];
    };
}