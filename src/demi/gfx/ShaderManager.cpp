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
#include "ShaderManager.h"

_IMPLEMENT_SINGLETON(Demi::DiShaderManager);

namespace Demi 
{
    DiShaderManager::DiShaderManager(void)
    {
        InitCommonFlags();
    }

    DiShaderManager::~DiShaderManager(void)
    {
        UnloadAllShaders();
    }

    void DiShaderManager::InitCommonFlags(void)
    {
        mCommonFlag["USE_FOG"]          = SHADER_FLAG_USE_FOG;
        mCommonFlag["USE_COLOR"]        = SHADER_FLAG_USE_COLOR;
        mCommonFlag["DOUBLE_SIDED"]     = SHADER_FLAG_DOUBLE_SIDED;
        mCommonFlag["GAMMA_CORRECTION"] = SHADER_FLAG_GAMMA_CORRECTION;
        mCommonFlag["USE_LIGHTMAP"]     = SHADER_FLAG_USE_LIGHTMAP;
        mCommonFlag["USE_MAP"]          = SHADER_FLAG_USE_MAP;
        mCommonFlag["USE_ENV_MAP"]      = SHADER_FLAG_USE_ENV_MAP;
        mCommonFlag["ENV_MAP_ADD"]      = SHADER_FLAG_ENV_MAP_ADD;
        mCommonFlag["ENV_MAP_MUL"]      = SHADER_FLAG_ENV_MAP_MUL;
        mCommonFlag["ENV_MAP_MIX"]      = SHADER_FLAG_ENV_MAP_MIX;
        mCommonFlag["ENV_MAP_REFRACT"]  = SHADER_FLAG_ENV_MAP_REFRACT;
        mCommonFlag["SKINNED"]          = SHADER_FLAG_SKINNED;
        mCommonFlag["SHADOW_DISABLED"]  = SHADER_FLAG_SHADOW_DISABLED;
        mCommonFlag["SHADOW_LOW"]       = SHADER_FLAG_SHADOW_LOW;
        mCommonFlag["SHADOW_MEDIUM"]    = SHADER_FLAG_SHADOW_MEDIUM;
        mCommonFlag["SHADOW_HIGH"]      = SHADER_FLAG_SHADOW_HIGH;
    }

    void DiShaderManager::ModifyMarcosByFlag(uint64 flag, DiCompileDesc& desc)
    {
        for (auto it = mCommonFlag.begin(); it != mCommonFlag.end(); ++it)
        {
            if(flag & it->second)
                desc.AddMarco(it->first);
            else
                desc.RemoveMarco(it->first);
        }
    }

    DiShaderProgram* DiShaderManager::LoadShader(const DiString& filename, DiShaderType type, uint64 flag)
    {
        DiShaderProgram* shader = nullptr;

        DiString file = filename + Driver->GetShaderFileExtension();

        auto it = mShaderLibs.find(file);
        if(it != mShaderLibs.end())
        {
            auto flagIt = it->second.find(flag);

            if(flagIt != it->second.end())
                return flagIt->second;
        }

        shader = DI_NEW DiShaderProgram(file, type);
        bool ret = shader->Load();

        if(!ret)
        {
            DI_WARNING("Failed to load the shader : %s", file.c_str());
            DI_DELETE shader;
            return nullptr;
        }

        DiCompileDesc desc;
        desc.entryName = type == SHADER_VERTEX ? "vs_main" : "ps_main";
        desc.profile = DiShaderProgram::GetDefaultProfile(type);

        ModifyMarcosByFlag(flag,desc);
        shader->Compile(desc);

        mShaderLibs[file].insert(DiMap<uint64, DiShaderProgram*>::iterator::value_type(flag, shader));

        return shader;
    }

    void DiShaderManager::UnloadAllShaders()
    {
        DI_DEBUG("unloading all shaders..");
        for (auto it = mShaderLibs.begin(); it != mShaderLibs.end(); ++it)
        {
            for (auto fIt = it->second.begin(); fIt != it->second.end(); ++fIt)
            {
                SAFE_DELETE(fIt->second);
            }
        }

        mShaderLibs.clear();
    }

    uint64 DiShaderManager::CoverterFlags(const DiVector<DiString>& flags)
    {
        uint64 flag = 0;
        for(size_t i = 0; i < flags.size(); i++)
        {
            auto it = mCommonFlag.find(flags[i]);
            if(it != mCommonFlag.end())
                flag |= it->second;
        }
        return flag;
    }

    DiString DiShaderManager::CoverterFlags(uint64 flags)
    {
        DiString ret;
        for (auto it = mCommonFlag.begin(); it != mCommonFlag.end(); ++it)
        {
            if(flags & it->second)
            {
                ret += it->first;
                ret += ",";
            }
        }
        ret.TrimRight(",");
        return ret;
    }
}