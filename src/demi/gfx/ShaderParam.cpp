
#include "ShaderParam.h"
#include "Memory.h"
#include "Material.h"
#include "AssetManager.h"
#include "Texture.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiShaderParameter::DiShaderParameter(DiMaterial& mat)
        : mMaterial(mat)
    {
    }

    DiShaderParameter::~DiShaderParameter(void)
    {
        UnloadVariables();
    }

    DiGpuVariable* DiShaderParameter::FindVariable( const DiString& name,
        DiGpuVariable::Type varType )
    {
        DiGpuVariable *var = 0;
        uint32 numVariables = (uint32)mVariables.size();

        for(uint32 i=0; i<numVariables; i++)
        {
            DiGpuVariable &v = *mVariables[i];
            if (v.GetName() == name)
            {
                var = &v;
                break;
            }
            else if(!v.GetName().empty() && v.GetName()[0] == '$')
            {
                DiString sub = v.GetName().substr(1);
                if (sub == name)
                {
                    var = &v;
                    break;
                }
            }
        }
        if(var && var->GetType() != varType)
        {
            var = 0;
        }
        return var;
    }

    DiGpuVariable* DiShaderParameter::FindVariable( const DiString& name )
    {
        DiGpuVariable *var = 0;
        uint32 numVariables = (uint32)mVariables.size();

        for(uint32 i=0; i<numVariables; i++)
        {
            DiGpuVariable &v = *mVariables[i];
            if (v.GetName() == name)
            {
                var = &v;
                break;
            }
            else if(!v.GetName().empty() && v.GetName()[0] == '$')
            {
                DiString sub = v.GetName().substr(1);
                if (sub == name)
                {
                    var = &v;
                    break;
                }
            }
        }

        if (!var)
        {
            DI_WARNING("Cannot find the variable :%s",name.c_str());
        }
        return var;
    }
    void DiShaderParameter::WriteFloat4Array( const DiString& name,DiVec4* vec4,uint32 size )
    {
        DiGpuVariable* var = FindVariable(name,DiGpuVariable::VARIABLE_FLOAT4_ARRAY);
        if (var)
        {
            DiPair<DiVec4*,uint32> p(vec4,size);
            DiAny an(p);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the float4 array : %s",name.c_str());
        }
    }

    void DiShaderParameter::WriteFloat4( const DiString& name,DiVec4 vec4 )
    {
        DiGpuVariable* var = FindVariable(name,DiGpuVariable::VARIABLE_FLOAT4);
        if (var)
        {
            DiAny an(vec4);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the float4 variable : %s",name.c_str());
        }
    }

    void DiShaderParameter::WriteFloat3( const DiString& name,DiVec3 vec3 )
    {
        DiGpuVariable* var = FindVariable(name,DiGpuVariable::VARIABLE_FLOAT3);
        if (var)
        {
            DiAny an(vec3);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the float3 variable : %s",name.c_str());
        }        
    }

    void DiShaderParameter::WriteFloat2( const DiString& name,DiVec2 ve2 )
    {
        DiGpuVariable* var = FindVariable(name,DiGpuVariable::VARIABLE_FLOAT2);
        if (var)
        {
            DiAny an(ve2);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the float2 variable : %s",name.c_str());
        }        
    }

    void DiShaderParameter::WriteFloat( const DiString& name,float ve1)
    {
        DiGpuVariable* var = FindVariable(name,DiGpuVariable::VARIABLE_FLOAT);
        if (var)
        {
            DiAny an(ve1);
            var->SetData(an);
        }
        else
        {
            //DI_WARNING("Failed to write the float variable : %s",name.c_str());
        }
    }

    DiTexturePtr DiShaderParameter::WriteTexture2D( const DiString& name, 
        const DiString& textureName )
    {
        DiAssetManager& assetMgr = DiAssetManager::GetInstance();
        DiTexturePtr textureAsset = assetMgr.GetAsset<DiTexture>(textureName);

        if (!textureAsset)
        {
            DI_WARNING("Failed to load the texture2D resource : %s",textureName.c_str());
            return DiTexturePtr();
        }

        DiGpuVariable *var = FindVariable(name, DiGpuVariable::VARIABLE_SAMPLER2D);

        if(var)
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the texture2D variable : %s",name.c_str());
            return textureAsset;
        }
        return textureAsset;
    }

    void DiShaderParameter::WriteTexture2D( const DiString& name,DiTexturePtr textureAsset )
    {
        DI_ASSERT(textureAsset);

        DiGpuVariable *var = FindVariable(name, DiGpuVariable::VARIABLE_SAMPLER2D);
        if(var)
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the texture2D variable : %s",name.c_str());
        }
    }

    DiTexturePtr DiShaderParameter::WriteTextureCUBE( const DiString& name, 
        const DiString& textureName )
    {
        DiAssetManager& assetMgr = DiAssetManager::GetInstance();
        DiTexturePtr textureAsset = assetMgr.GetAsset<DiTexture>(textureName);

        if (!textureAsset)
        {
            DI_WARNING("Failed to load the textureCUBE resource : %s",textureName.c_str());
            return DiTexturePtr();
        }

        DiGpuVariable *var = FindVariable(name, DiGpuVariable::VARIABLE_SAMPLERCUBE);

        if(var)
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the textureCUBE variable : %s",name.c_str());
            return textureAsset;
        }
        return textureAsset;
    }

    void DiShaderParameter::WriteTextureCUBE( const DiString& name,DiTexturePtr textureAsset )
    {
        DI_ASSERT(textureAsset);

        DiGpuVariable *var = FindVariable(name, DiGpuVariable::VARIABLE_SAMPLERCUBE);
        if(var)
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the textureCUBE variable : %s",name.c_str());
        }
    }

    void DiShaderParameter::LoadVariables(DiShaderProgram* prog)
    {
        prog->GetShader()->LoadVariables([this](DiGpuVariable* var) 
        {
            uint32 numVariables = (uint32)this->mVariables.size();
            for (uint32 j = 0; j < numVariables; j++)
            {
                if (this->mVariables[j]->GetName() == var->GetName())
                {
                    DI_WARNING("The variable : %s, has already existed!", var->GetName().c_str());
                    break;
                }
            }
            this->mVariables.push_back(var);
        });
    }

    void DiShaderParameter::UnloadVariables()
    {
        size_t numVariables = mVariables.size();
        for(size_t i=0; i<numVariables; i++)
        {
            DI_DELETE mVariables[i];
        }
        mVariables.clear();
    }

    void DiShaderParameter::Bind() const
    {
        uint32 numVariables = (uint32)mVariables.size();
        for(uint32 i=0; i<numVariables; i++)
        {
            const DiGpuVariable &variable = *mVariables[i];
            variable.Bind();
        }
    }

    bool DiShaderParameter::HasVariableType( DiGpuVariable::Type varType )
    {
        uint32 numVariables = (uint32)mVariables.size();

        for(uint32 i=0; i<numVariables; i++)
        {
            DiGpuVariable &v = *mVariables[i];
            if (v.GetType() == varType)
                return true;
        }
        return false;
    }

    void DiShaderParameter::CloneVarsTo( DiShaderParameter* ps )
    {
        for (auto it = mVariables.begin(); it != mVariables.end(); ++it)
        {
            DiGpuVariable* var = ps->FindVariable((*it)->GetName(),(*it)->GetType());
            if (var)
                var->SetData((*it)->GetData());
        }
    }

    void DiShaderParameter::WriteColor( const DiString& name,DiColor vec4 )
    {
        DiGpuVariable* var = FindVariable(name,DiGpuVariable::VARIABLE_COLOR);
        if (var)
        {
            DiAny an(vec4);
            var->SetData(an);
        }
        else
        {
            DI_WARNING("Failed to write the color variable : %s",name.c_str());
        }
    }
}