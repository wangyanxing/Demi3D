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

#ifndef D3D9ShaderParam_h__
#define D3D9ShaderParam_h__

#include "GpuProgram.h"
#include "ShaderParam.h"
#include "D3D9Driver.h"
#include "D3D9StateCache.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9ShaderParam : public DiShaderParameter
    {
    public:

        DiD3D9ShaderParam(DiMaterial& mat);

        ~DiD3D9ShaderParam();

    public:

        void            LoadParameters();

        void            Bind() const;

        void            AddD3DParameter(const DiString& name, DiShaderType shadertype, uint32 regID);

    private:

        typedef DiStrHash<DiPair<DiShaderType, uint32>> D3DConstMaps;

        D3DConstMaps    mD3DConsts;
    };
}

#endif // D3D9ShaderParam_h__