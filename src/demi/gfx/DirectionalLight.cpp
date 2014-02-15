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
        camera->GetSceneManager()->GetVisibleLights().dirLights.push_back(this);
    }

    void DiDirLight::SetupShadowCamera(DiSceneManager* sceneManager)
    {
        if (!mParentNode)
            return;

        int splits = 0;
        DiCamera* camera = sceneManager->GetCamera();
        float nearSplit = camera->GetNearClipDistance();
        float farSplit;

        while (splits < MAX_CASCADE_SPLITS)
        {
            // If split is completely beyond camera far clip, we are done
            if (nearSplit > camera->GetFarClipDistance())
                break;

            farSplit = DiMath::Min(camera->GetFarClipDistance(), mShadowParams.splits[splits]);
            if (farSplit <= nearSplit)
                break;

            // Setup the shadow camera for the split
            DiCamera* shadowCamera = sceneManager->GetCameraPool().GetCamera();
            mShadowParams.shadowCameras[splits] = shadowCamera;
            mShadowParams.shadowNearSplits[splits] = nearSplit;
            mShadowParams.shadowFarSplits[splits] = farSplit;
            SetupShadowCameraImpl(sceneManager, shadowCamera, nearSplit, farSplit);

            nearSplit = farSplit;
            ++splits;
        }
    }

    void DiDirLight::SetupShadowCameraImpl(DiSceneManager* sceneManager, DiCamera* shadowCamera,
        float nearSplit, float farSplit)
    {
        DiCamera* camera = sceneManager->GetCamera();
        float extrusionDistance = camera->GetFarClipDistance();

        DiVec3 worldDirection = mParentNode->GetDerivedOrientation() * mDirection;

        // Calculate initial position & rotation
        DiVec3 pos = camera->GetDerivedPosition() - extrusionDistance * worldDirection;
        shadowCamera->SetPosition(pos);
        shadowCamera->SetOrientation(mParentNode->GetDerivedOrientation());

        // Calculate main camera shadowed frustum in light's view space
        farSplit = DiMath::Min(farSplit, camera->GetFarClipDistance());

        // Use the scene Z bounds to limit frustum size if applicable
        /*if (mShadowParams.focus)
        {
            nearSplit = DiMath::Max(minZ, nearSplit);
            farSplit = DiMath::Min(maxZ, farSplit);
        }*/
    }
}