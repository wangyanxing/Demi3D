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

#ifndef DiShaderProgram_h__
#define DiShaderProgram_h__


#include "GpuProgram.h"

namespace Demi 
{
    class DI_GFX_API DiShaderProgram : public DiBase
    {
    public:

        DiShaderProgram(const DiString& name, DiShaderType shaderType);

        virtual                     ~DiShaderProgram();

    public:

        bool                        LoadingComplete() const;

        bool                        Load(DiDataStreamPtr data);

        bool                        Load();

        bool                        Load(const DiString& filename);

        DiString                    GetShaderFileName() const { return mShaderFileName; }

        void                        SetCode(DiShaderType,const DiString& str);

        DiString&                   GetCode(){ return mRawCodes; }

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

        DiString                    mRawCodes;

        DiString                    mShaderFileName;
        
        DiShaderType                mShaderType;
        
        DiShaderInstance*           mShader;

        static DiString             sDefaultVsProfile;

        static DiString             sDefaultPsProfile;
    };
}

#endif
