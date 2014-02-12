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
#include "DirectionalLight.h"
#include "GpuProgram.h"

namespace Demi 
{
    DiDirLight::DiDirLight(DiSceneManager* mgr)
        :DiLight(DiLight::TYPE_DIRECTIONAL,mgr),
        mDirection(DiVec3::UNIT_Z)
    {
    }

    DiDirLight::~DiDirLight(void)
    {
    }

    void DiDirLight::Bind( DiShaderEnvironment* env )
    {
    }

    const DiAABB& DiDirLight::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    DiString& DiDirLight::GetType()
    {
        static DiString type = "DirectionalLight";
        return type;
    }
}