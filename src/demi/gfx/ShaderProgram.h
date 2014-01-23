
/********************************************************************
    File:        ShaderProgram.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuProgram.h"

namespace Demi 
{
    class DiShaderProgram : public DiBase
    {
    public:

        DiShaderProgram(const DiString& name);

        virtual                     ~DiShaderProgram();

    public:

        DiAssetType                 GetAssetType() const;

        BOOL                        LoadingComplete() const;

        BOOL                        Load(DiDataStreamPtr data);

        BOOL                        Load();

        BOOL                        Load(const DiString& filename);

        DiString                    GetShaderFileName() const { return mShaderFileName; }

        void                        SetCode(DiShaderType,const DiString& str);

        DiString&                   GetCode(){return mCodes;}

        bool                        Compile(const DiCompileDesc& desc);

        bool                        Compile();

        DiShaderType                GetShaderType() const { return mShaderType; }

        void                        SetShaderType(DiShaderType val) { mShaderType = val; }

        DiShaderInstance*           GetShader();

        void                        Bind(const DiShaderEnvironment& shaderEnv);

        static DiString&            GetDefaultVsProfile() { return sDefaultVsProfile; }

        static DiString&            GetDefaultPsProfile() { return sDefaultPsProfile; }
        
        static DiString&            GetDefaultProfile(DiShaderType type) 
        {
            return type == SHADER_VERTEX ? sDefaultVsProfile : sDefaultPsProfile; 
        }

        DiString&                   GetName(){return mFileName;}

    protected:

        DiString                    mFileName;

        DiString                    mCodes;

        DiString                    mRowCodes;

        DiString                    mShaderFileName;
        
        DiShaderType                mShaderType;
        
        DiShaderInstance*           mShader;

        static DiString             sDefaultVsProfile;

        static DiString             sDefaultPsProfile;
    };
}