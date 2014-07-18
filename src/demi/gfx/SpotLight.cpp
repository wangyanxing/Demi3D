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

    void DiSpotLight::_UpdateShadowCamera()
    {
        if(mShadowCameras.empty())
            return;
        
        mShadowCameras[0]->SetProjectionType(PT_PERSPECTIVE);
        mShadowCameras[0]->SetFOVy(DiRadian(DiDegree(mShadowCameraFov)));
        if(mParentNode)
        {
            mShadowCameras[0]->SetPosition(mParentNode->GetDerivedPosition());
            mShadowCameras[0]->LookAt(DiVec3::ZERO);
            //mShadowCameras[0]->SetOrientation(mParentNode->GetDerivedOrientation());
        }
        mShadowCameras[0]->SetNearClipDistance(mShadowCameraNear);
        mShadowCameras[0]->SetFarClipDistance(mShadowCameraFar);
        
        static const DiMat4 PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE(
                                                                          0.5,    0,    0,  0.5,
                                                                          0,   -0.5,    0,  0.5,
                                                                          0,      0,    1,    0,
                                                                          0,      0,    0,    1);

        DiMat4 mat;
        mat.makeTransform(mShadowCameras[0]->GetDerivedPosition(), DiVec3::UNIT_SCALE, mShadowCameras[0]->GetDerivedOrientation());
       
        auto mt = PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE *
        mShadowCameras[0]->GetProjectionMatrix()* mat.inverse();
    }
}