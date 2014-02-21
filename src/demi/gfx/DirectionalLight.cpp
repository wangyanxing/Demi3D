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
        

        /*
        DiCamera* camera = sceneManager->GetCamera();
        UpdateSplitDist(camera);

        int cur_num_splits = 4;
        for (int i = 0; i < cur_num_splits; i++)
        {
            UpdateFrustumPoints(mShadowFrustums[i], camera->GetDerivedPosition(), camera->GetDirection());
            DiCamera* shadowCamera = sceneManager->GetCameraPool().GetCamera();

            float extrusionDistance = camera->GetFarClipDistance();
            DiVec3 worldDirection = mParentNode->GetDerivedOrientation() * mDirection;

            // Calculate initial position & rotation
            DiVec3 pos = camera->GetDerivedPosition() - extrusionDistance * worldDirection;
            shadowCamera->SetPosition(pos);
            shadowCamera->SetOrientation(mParentNode->GetDerivedOrientation());
        }
        */
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
        if (false/*mShadowParams.focus*/)
        {
            nearSplit = DiMath::Max(sceneManager->GetZRange().x, nearSplit);
            farSplit  = DiMath::Min(sceneManager->GetZRange().y, farSplit);
        }

        DiCamera* splitFrustum = camera->GetSplitFrustum(nearSplit, farSplit);

        // Transform frustum volume to light space
        const DiQuat& parentOrientation = shadowCamera->GetDerivedOrientation();
        const DiVec3& parentPosition = shadowCamera->GetDerivedPosition();
        splitFrustum->MoveRelative(parentPosition);
        splitFrustum->Rotate(parentOrientation);

        DiAABB shadowbox = splitFrustum->GetBoundingBox();

        DI_DELETE splitFrustum;

        shadowCamera->SetProjectionType(PT_ORTHOGRAPHIC);
        shadowCamera->SetAspectRatio(1.0);
        shadowCamera->SetAutoAspectRatio(false);
        shadowCamera->SetNearClipDistance(0.1f);
        shadowCamera->SetFarClipDistance(shadowbox.GetMaximum().z);

        // Center shadow camera on the bounding box. Can not snap to texels yet as the shadow map viewport is unknown
        QuantizeShadowCamera(shadowCamera, DiRect(0, 0, 0, 0), shadowbox);
    }

    void DiDirLight::QuantizeShadowCamera(DiCamera* shadowCamera, const DiRect& shadowViewport, const DiAABB& viewBox)
    {
        float shadowMapWidth = (float)(shadowViewport.Width());

        float minX = viewBox.GetMinimum().x;
        float minY = viewBox.GetMinimum().y;
        float maxX = viewBox.GetMaximum().x;
        float maxY = viewBox.GetMaximum().y;

        DiVec2 center((minX + maxX) * 0.5f, (minY + maxY) * 0.5f);
        DiVec2 viewSize(maxX - minX, maxY - minY);

        // Quantize size to reduce swimming
        // Note: if size is uniform and there is no focusing, quantization is unnecessary
        if (mShadowParams.nonUniform)
        {
            viewSize.x = ceilf(sqrtf(viewSize.x / mShadowParams.quantize));
            viewSize.y = ceilf(sqrtf(viewSize.y / mShadowParams.quantize));
            viewSize.x = DiMath::Max(viewSize.x * viewSize.x * mShadowParams.quantize, mShadowParams.minView);
            viewSize.y = DiMath::Max(viewSize.y * viewSize.y * mShadowParams.quantize, mShadowParams.minView);
        }

        shadowCamera->SetOrthoWindow(viewSize.x, viewSize.y);

        DiVec3 adjust(center.x, center.y, 0.0f);
        shadowCamera->MoveRelative(adjust);

        DiQuat rot = shadowCamera->GetDerivedOrientation();

        // If the shadow map viewport is known, snap to whole texels
        if (shadowMapWidth > 0.0f)
        {
            DiVec3 viewPos(rot.Inverse() * shadowCamera->GetDerivedPosition());
            // Take into account that shadow map border will not be used
            float invActualSize = 1.0f / (shadowMapWidth - 2.0f);
            DiVec2 texelSize(viewSize.x * invActualSize, viewSize.y * invActualSize);
            DiVec3 snap(-fmodf(viewPos.x, texelSize.x), -fmodf(viewPos.y, texelSize.y), 0.0f);
            shadowCamera->MoveRelative(snap);
        }
    }
}