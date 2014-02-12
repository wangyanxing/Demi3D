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
#include "PointLight.h"
#include "GpuProgram.h"

namespace Demi 
{
    DiPointLight::DiPointLight(DiSceneManager* mgr)
        :DiLight(DiLight::TYPE_POINT,mgr),
        mPosition(DiVec3::ZERO),
        mAttenuationBegin(0),
        mAttenuationEnd(50)
    {
    }

    DiPointLight::~DiPointLight(void)
    {
    }

    const DiAABB& DiPointLight::GetBoundingBox( void ) const
    {
        DiVec3 temp(mAttenuationEnd, mAttenuationEnd, mAttenuationEnd);
        mAABB.SetExtents(mPosition - temp, mPosition + temp);
        return mAABB;
    }

    DiString& DiPointLight::GetType()
    {
        static DiString type = "PointLight";
        return type;
    }
}