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

#ifndef DiShaderManager_h__
#define DiShaderManager_h__


#include "Singleton.h"
#include "GpuProgram.h"
#include "ShaderProgram.h"

namespace Demi 
{
    // shader common flags
    #define CSF_USE_FOG             (uint64)0x1
    #define CSF_USE_COLOR           (uint64)0x2        //using vertex color
    #define CSF_DOUBLE_SIDED        (uint64)0x4
    #define CSF_GAMMA_CORRECTION    (uint64)0x8
    #define CSF_USE_LIGHTMAP        (uint64)0x10
    #define CSF_USE_MAP             (uint64)0x20
    #define CSF_USE_ENV_MAP         (uint64)0x40
    #define CSF_ENV_MAP_ADD         (uint64)0x80
    #define CSF_ENV_MAP_MUL         (uint64)0x100
    #define CSF_ENV_MAP_MIX         (uint64)0x200
    #define CSF_ENV_MAP_REFRACT     (uint64)0x400
    #define CSF_SKINNED             (uint64)0x800

    ////////////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiShaderManager : public DiSingleton<DiShaderManager>, public DiBase
    {
    public:
        DiShaderManager(void);

        virtual                ~DiShaderManager(void);

        _DECLARE_SINGLETON(DiShaderManager)

    public:

        DiShaderProgram*        LoadShader(const DiString& filename, DiShaderType type, uint64 flag);

        void                    UnloadAllShaders();

        uint64                  CoverterFlags(const DiVector<DiString>& flags);

        DiString                CoverterFlags(uint64 flags);

    protected:

        void                    InitCommonFlags();
        
        void                    ModifyMarcosByFlag(uint64 flag, DiCompileDesc& desc);

    protected:

        typedef DiMap<DiString, uint64>   MapNameFlags;
        typedef MapNameFlags::iterator    MapNameFlagsIt;

        MapNameFlags            mCommonFlag;

        typedef DiMap<DiString, DiMap<uint64,DiShaderProgram*>>    ShaderLibs;
        typedef ShaderLibs::iterator ShaderLibsIt;

        ShaderLibs              mShaderLibs;
    };

}

#endif
