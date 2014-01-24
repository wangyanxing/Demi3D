
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

        virtual                 ~DiShaderParameter(void);

    public:

        void                    LoadVariables(DiShaderProgram* prog);

        void                    UnloadVariables();

        void                    CloneVarsTo(DiShaderParameter* ps);

        DiGpuVariable*          FindVariable(const DiString& name, DiGpuVariable::Type varType);

        DiGpuVariable*          FindVariable(const DiString& name);

        const DiGpuVariable*    GetVariable(size_t id) { return mVariables[id]; }

        size_t                  GetVariableNum()const { return mVariables.size(); }

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

        void                    Bind() const;

        bool                    HasVariableType(DiGpuVariable::Type varType);

    protected:

        DiShaderParameter &operator = (const DiShaderParameter&);

        typedef DiVector<DiGpuVariable*> VariableList;

        DiMaterial&             mMaterial;

        VariableList            mVariables;
    };
}