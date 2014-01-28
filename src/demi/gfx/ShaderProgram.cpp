
#include "GfxPch.h"
#include "ShaderProgram.h"
#include "AssetManager.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiString DiShaderProgram::sDefaultPsProfile = "ps_3_0";
    DiString DiShaderProgram::sDefaultVsProfile = "vs_3_0";

    DiShaderProgram::DiShaderProgram(const DiString& name, DiShaderType shaderType)
        : mFileName(name),
        mShaderType(shaderType),
        mShader(nullptr)
    {
    }

    DiShaderProgram::~DiShaderProgram()
    {
        SAFE_DELETE(mShader);
    }

    Demi::DiAssetType DiShaderProgram::GetAssetType() const
    {
        return ASSET_SHADER;
    }

    bool DiShaderProgram::Load(DiDataStreamPtr data)
    {
        mShaderFileName = data->GetName();
        mShaderFileName.ToLower();
        mRawCodes = data->GetAsString();
        return mRawCodes.empty() ? false : true;
    }

    bool DiShaderProgram::Load()
    {
        return Load(mFileName);
    }

    bool DiShaderProgram::Load(const DiString& filename)
    {
        DiDataStreamPtr buf = DiAssetManager::GetInstance().OpenArchive(filename);
        if (!buf)
        {
            return false;
        }

        Load(buf);

        return true;
    }

    bool DiShaderProgram::LoadingComplete() const
    {
        return mRawCodes.empty() ? false : true;
    }

    void DiShaderProgram::SetCode( DiShaderType type, const DiString& str )
    {
        mShaderType = type;
        mRawCodes = str;
    }

    bool DiShaderProgram::Compile(const DiCompileDesc& desc)
    {
        if (!Driver)
            return false;

        SAFE_DELETE(mShader);

        if(mShaderType == SHADER_VERTEX)
        {
            mShader = Driver->CreateVSInstance(this);
        }
        else if (mShaderType == SHADER_PIXEL)
        {
            mShader = Driver->CreatePSInstance(this);
        }
        else
        {
            DI_WARNING("Unsupported shader type : %d", mShaderType);
            return false;
        }

        mShader->mCompileDesc = desc;

        if (mRawCodes.empty())
        {
            DI_WARNING("Empty shader code : %s", mFileName.c_str());
            return false;
        }

        return mShader->Compile(mRawCodes);
    }

    bool DiShaderProgram::Compile()
    {
        if (!Driver)
            return false;

        DiCompileDesc desc;
        
        if(mShaderType == SHADER_VERTEX)
        {
            desc.entryName  = "vs_main";
            desc.profile    = sDefaultVsProfile;
        }
        else
        {
            desc.entryName  = "ps_main";
            desc.profile    = sDefaultPsProfile;
        }

        return Compile(desc);
    }

    void DiShaderProgram::Bind( const DiShaderEnvironment& shaderEnv )
    {
        if (mShader)
            mShader->Bind(shaderEnv);
    }

    DiShaderInstance* DiShaderProgram::GetShader( )
    {
        return mShader;
    }
}