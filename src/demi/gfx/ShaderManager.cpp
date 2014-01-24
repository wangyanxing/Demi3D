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
        mCommonFlag.insert( MapNameFlagsIt::value_type( "USE_FOG"            , CSF_USE_FOG ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "USE_COLOR"          , CSF_USE_COLOR ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "DOUBLE_SIDED"       , CSF_DOUBLE_SIDED ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "GAMMA_CORRECTION"   , CSF_GAMMA_CORRECTION ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "USE_LIGHTMAP"       , CSF_USE_LIGHTMAP ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "USE_MAP"             , CSF_USE_MAP ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "USE_ENV_MAP"         , CSF_USE_ENV_MAP ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "ENV_MAP_ADD"         , CSF_ENV_MAP_ADD ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "ENV_MAP_MUL"         , CSF_ENV_MAP_MUL ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "ENV_MAP_MIX"         , CSF_ENV_MAP_MIX ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "ENV_MAP_REFRACT"     , CSF_ENV_MAP_REFRACT ));
        mCommonFlag.insert( MapNameFlagsIt::value_type( "SKINNED"            , CSF_SKINNED));
    }

    void DiShaderManager::ModifyMarcosByFlag(uint64 flag, DiCompileDesc& desc)
    {
        MapNameFlagsIt it    = mCommonFlag.begin();
        MapNameFlagsIt itEnd = mCommonFlag.end();

        for(it = mCommonFlag.begin(); it != itEnd; ++it)
        {
            if(flag & it->second)
                desc.AddMarco(it->first);
            else
                desc.RemoveMarco(it->first);
        }
    }

    DiShaderProgram* DiShaderManager::LoadShader(const DiString& filename, DiShaderType type, uint64 flag)
    {
        DiShaderProgram* shader = NULL;

        DiString file = filename + Driver->GetShaderFileExtension();

        ShaderLibsIt it = mShaderLibs.find(file);
        if(it != mShaderLibs.end())
        {
            DiMap<uint64,DiShaderProgram*>::iterator flagIt =
                it->second.find(flag);

            if(flagIt != it->second.end())
                return flagIt->second;
        }

        shader = DI_NEW DiShaderProgram(file, type);
        BOOL ret = shader->Load();

        if(!ret)
        {
            DI_WARNING("Failed to load the shader : %s", file.c_str());
            DI_DELETE shader;
            return NULL;
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

        ShaderLibsIt it;
        ShaderLibsIt itEnd = mShaderLibs.end();

        for(it = mShaderLibs.begin(); it != itEnd; ++it)
        {
            DiMap<uint64,DiShaderProgram*>::iterator fIt;
            DiMap<uint64,DiShaderProgram*>::iterator fItEnd = it->second.end();
            for(fIt = it->second.begin(); fIt != fItEnd; ++fIt)
            {
                SAFE_DELETE(fIt->second);
            }
        }

        mShaderLibs.clear();
    }

    uint64 DiShaderManager::CoverterFlags(const DiVector<DiString>& flags)
    {
        uint64 flag = 0;
        for(size_t i = 0; i < flags.size(); i++){
            MapNameFlagsIt it = mCommonFlag.find(flags[i]);
            if(it != mCommonFlag.end()){
                flag |= it->second;
            }
        }
        return flag;
    }

    DiString DiShaderManager::CoverterFlags(uint64 flags)
    {
        DiString ret;
        MapNameFlagsIt it     = mCommonFlag.begin();
        MapNameFlagsIt itEnd = mCommonFlag.end();
        for(it = mCommonFlag.begin(); it != itEnd; ++it){
            if(flags & it->second){
                ret += it->first;
                ret += ",";
            }
        }
        ret.TrimRight(",");
        return ret;
    }
}