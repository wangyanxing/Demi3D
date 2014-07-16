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
#include "Camera.h"
#include "SceneManager.h"
#include "CullNode.h"
#include "RenderWindow.h"

namespace Demi 
{
    DiDirLight::DiDirLight()
        :DiLight(LIGHT_DIRECTIONAL),
        mDirection(DiVec3::UNIT_Z)
    {
    }

    DiDirLight::~DiDirLight(void)
    {
    }

    const DiAABB& DiDirLight::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiDirLight::Update(DiCamera* camera)
    {
        DiSceneManager* sm = camera->GetSceneManager();
        if (sm->GetCurrentPass() == GEOMETRY_PASS)
            sm->GetVisibleLights().dirLights.push_back(this);
    }
    
    void DiDirLight::SetDirection(const Demi::DiVec3 &vec)
    {
        mDirection = vec;
    }

    void CreateFrustumPointsFromCascadeInterval(float fCascadeIntervalBegin,
        float fCascadeIntervalEnd,
        DiMat4& vProjection,
        DiVec3* pvCornerPointsWorld)
    {
    }

    void DiDirLight::SetupShadowCamera(DiSceneManager* sceneManager)
    {
        
    }
    
    DiVec3 DiDirLight::GetDerivedDirection() const
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
    
    DiVec3 DiDirLight::GetDerivedPosition() const
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