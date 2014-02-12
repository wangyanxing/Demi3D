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
#include "GfxPch.h"
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
    }

    void DiShaderParameter::WriteFloat4Array( const DiString& name,DiVec4* vec4,uint32 size )
    {
        auto it = mShaderParams[VARIABLE_FLOAT4_ARRAY].find(name);
        if (it != mShaderParams[VARIABLE_FLOAT4_ARRAY].end())
        {
            DiPair<DiVec4*,uint32> p(vec4,size);
            DiAny an(p);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the float4 array : %s",name.c_str());
        }
    }

    void DiShaderParameter::WriteMatrix4(const DiString& name, DiMat4 mat4)
    {
        auto it = mShaderParams[VARIABLE_MAT4].find(name);
        if (it != mShaderParams[VARIABLE_MAT4].end())
        {
            DiAny an(mat4);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the float4 variable : %s", name.c_str());
        }
    }

    void DiShaderParameter::WriteFloat4( const DiString& name,DiVec4 vec4 )
    {
        auto it = mShaderParams[VARIABLE_FLOAT4].find(name);
        if (it != mShaderParams[VARIABLE_FLOAT4].end())
        {
            DiAny an(vec4);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the float4 variable : %s",name.c_str());
        }
    }

    void DiShaderParameter::WriteFloat3( const DiString& name,DiVec3 vec3 )
    {
        auto it = mShaderParams[VARIABLE_FLOAT3].find(name);
        if (it != mShaderParams[VARIABLE_FLOAT3].end())
        {
            DiAny an(vec3);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the float3 variable : %s",name.c_str());
        }        
    }

    void DiShaderParameter::WriteFloat2( const DiString& name,DiVec2 ve2 )
    {
        auto it = mShaderParams[VARIABLE_FLOAT2].find(name);
        if (it != mShaderParams[VARIABLE_FLOAT2].end())
        {
            DiAny an(ve2);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the float2 variable : %s",name.c_str());
        }        
    }

    void DiShaderParameter::WriteFloat( const DiString& name,float ve1)
    {
        auto it = mShaderParams[VARIABLE_FLOAT].find(name);
        if (it != mShaderParams[VARIABLE_FLOAT].end())
        {
            DiAny an(ve1);
            it->second = an;
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

        auto it = mShaderParams[VARIABLE_SAMPLER2D].find(name);
        if (it != mShaderParams[VARIABLE_SAMPLER2D].end())
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            it->second = an;
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

        auto it = mShaderParams[VARIABLE_SAMPLER2D].find(name);
        if (it != mShaderParams[VARIABLE_SAMPLER2D].end())
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            it->second = an;
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

        auto it = mShaderParams[VARIABLE_SAMPLERCUBE].find(name);
        if (it != mShaderParams[VARIABLE_SAMPLERCUBE].end())
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            it->second = an;
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

        auto it = mShaderParams[VARIABLE_SAMPLERCUBE].find(name);
        if (it != mShaderParams[VARIABLE_SAMPLERCUBE].end())
        {
            DiTexture* tex = textureAsset.get();
            DiAny an(tex);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the textureCUBE variable : %s",name.c_str());
        }
    }

    bool DiShaderParameter::HasVariableType(DiShaderParameter::ParamType varType)
    {
        return !mShaderParams[VARIABLE_SAMPLERCUBE].empty();
    }

    void DiShaderParameter::CloneVarsTo( DiShaderParameter* ps )
    {
        for (int i = 0; i < NUM_VARIABLE_TYPES; ++i)
        {
            ps->mShaderParams[i] = mShaderParams[i];
        }
    }

    void DiShaderParameter::WriteColor( const DiString& name,DiColor vec4 )
    {
        auto it = mShaderParams[VARIABLE_COLOR].find(name);
        if (it != mShaderParams[VARIABLE_COLOR].end())
        {
            DiAny an(vec4);
            it->second = an;
        }
        else
        {
            DI_WARNING("Failed to write the color variable : %s",name.c_str());
        }
    }

    void DiShaderParameter::AddParameter(ParamType type, const DiString& name)
    {
        DI_ASSERT(type < NUM_VARIABLE_TYPES);
        auto it = mShaderParams[type].find(name);
        if (it != mShaderParams[type].end())
        {
            DI_WARNING("The parameter with name %s has already existed.", name.c_str());
            return;
        }
        mShaderParams[type][name] = GetDefault(type);
    }

    DiAny DiShaderParameter::GetDefault(ParamType type)
    {
        switch (type)
        {
        case VARIABLE_FLOAT:
            return DiAny(0.0f);
        case VARIABLE_FLOAT2:
            return DiAny(DiVec2::ZERO);
        case VARIABLE_FLOAT3:
            return DiAny(DiVec3::ZERO);
        case VARIABLE_FLOAT4:
            return DiAny(DiVec4::ZERO);
        case VARIABLE_MAT4:
            return DiAny(DiMat4::ZERO);
        case VARIABLE_COLOR:
            return DiAny(DiColor::Black);
        case VARIABLE_FLOAT4_ARRAY:
            return DiAny(DiPair<DiVec4*, uint32>(nullptr,0));
        case VARIABLE_SAMPLER2D:
        case VARIABLE_SAMPLERCUBE:
            return DiAny(DiTexturePtr());
        default:
            DI_WARNING("Unsupported shader parameter type :%d", type);
            return DiAny(nullptr);
        }
    }
}