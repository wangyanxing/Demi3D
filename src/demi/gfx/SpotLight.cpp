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
#include "SpotLight.h"
#include "GpuProgram.h"
#include "SceneManager.h"
#include "CullNode.h"

namespace Demi 
{
    DiSpotLight::DiSpotLight()
        :DiLight(LIGHT_SPOT)
    {
    }

    DiSpotLight::~DiSpotLight(void)
    {
    }

    const DiAABB& DiSpotLight::GetBoundingBox(void) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiSpotLight::Update(DiCamera* camera)
    {
        DiSceneManager* sm = camera->GetSceneManager();
        if (sm->GetCurrentPass() == GEOMETRY_PASS)
            sm->GetVisibleLights().spotLight = this;
    }
    
    DiVec3 DiSpotLight::GetDerivedDirection() const
    {
        if(mParentNode)
        {
            return mParentNode->GetDerivedOrientation() * mDirection;
        }
        else
        {
            return mDirection;
        }
    }
    
    DiVec3 DiSpotLight::GetDerivedPosition() const
    {
        if(mParentNode)
        {
            return mParentNode->GetDerivedPosition();
        }
        else
        {
            return DiVec3::ZERO;
        }
    }

}