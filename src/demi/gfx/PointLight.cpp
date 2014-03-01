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
#include "CullNode.h"
#include "SceneManager.h"

namespace Demi 
{
    DiPointLight::DiPointLight()
        :DiLight(LIGHT_POINT),
        mAttenuationBegin(0),
        mAttenuationEnd(50)
    {
    }

    DiPointLight::~DiPointLight(void)
    {
    }

    const DiAABB& DiPointLight::GetBoundingBox( void ) const
    {
        if (!mParentNode)
        {
            mAABB.SetInfinite();
        }
        else
        {
            DiVec3 temp(mAttenuationEnd, mAttenuationEnd, mAttenuationEnd);
            DiVec3 pos = mParentNode->GetDerivedPosition();
            mAABB.SetExtents(pos - temp, pos + temp);
        }
        return mAABB;
    }

    void DiPointLight::Update(DiCamera* camera)
    {
        DiSceneManager* sm = camera->GetSceneManager();
        if (sm->GetCurrentPass() == GEOMETRY_PASS)
            camera->GetSceneManager()->GetVisibleLights().pointLights.push_back(this);
    }
}