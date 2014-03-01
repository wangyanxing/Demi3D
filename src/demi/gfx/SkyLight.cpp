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
#include "SkyLight.h"
#include "GpuProgram.h"
#include "SceneManager.h"

namespace Demi 
{
    DiSkyLight::DiSkyLight()
        :DiLight(LIGHT_SKY),
        mDirection(DiVec3::ZERO),
        mGroundColor(DiColor::White)
    {
    }

    DiSkyLight::~DiSkyLight(void)
    {
    }

    const DiAABB& DiSkyLight::GetBoundingBox(void) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiSkyLight::Update(DiCamera* camera)
    {
        DiSceneManager* sm = camera->GetSceneManager();
        if (sm->GetCurrentPass() == GEOMETRY_PASS)
            camera->GetSceneManager()->GetVisibleLights().skyLight = this;
    }
}