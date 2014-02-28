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
        camera->GetSceneManager()->GetVisibleLights().dirLights.push_back(this);
    }
    
    void DiDirLight::SetDirection(const Demi::DiVec3 &vec)
    {
        mDirection = vec;
        mShadowCameraDirty = true;
    }

    void CreateFrustumPointsFromCascadeInterval(float fCascadeIntervalBegin,
        float fCascadeIntervalEnd,
        DiMat4& vProjection,
        DiVec3* pvCornerPointsWorld)
    {
    }

    void DiDirLight::SetupShadowCamera(DiSceneManager* sceneManager)
    {
        if (!mParentNode)
            return;

        DiCamera* camera = sceneManager->GetCamera();
        int cur_num_splits = 4;
        for (int i = 0; i < cur_num_splits; ++i)
        {
            if (!mShadowCameras[i])
                mShadowCameras[i] = DI_NEW DiCamera(nullptr);
            DiCamera* shadowCam = mShadowCameras[i];

            GetShadowCamera(camera, shadowCam, i);
        }

#if 0        
        /*
        DI_ASSERT(ActiveRenderWindow);
        if(mParentNode->GetLastUpdateFrameNumber() == ActiveRenderWindow->GetFrameNumber())
        mShadowCameraDirty = true;
        */


        /* if(!mShadowCastEnable || !mShadowCameraDirty)
             return;*/

        mShadowCameraDirty = false;

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
#endif

#if 0
        // setup light camera

        DiCamera* camera = sceneManager->GetCamera();
        if (!mShadowCamera)
            mShadowCamera = DI_NEW DiCamera(sceneManager);
        mShadowCamera->SetPosition(-320.0f, 500.0f, -220.3f);
        mShadowCamera->LookAt(0, 0, 0);
        mShadowCamera->SetAspectRatio(1.0f);
        mShadowCamera->SetAutoAspectRatio(false);
        mShadowCamera->SetNearClipDistance(0.1f);
        mShadowCamera->SetFarClipDistance(1000.0f);
        mDirection = mShadowCamera->GetDirection().normalisedCopy();

        // frustum intervals partition
        float cascadePartitionsZeroToOne[4] = { 5, 15, 60, 100 };
        float cascadePartitionsMax = 100;

        DiMat4 matLightCameraView = mShadowCamera->GetViewMatrix();
        const DiMat4& viewMat = camera->GetViewMatrix();

        // scene aabb to light space
        const DiAABB& sceneBounds = sceneManager->GetBoundingBox();
        //DiAABB sceneBounds(DiVec3(-122.636f, -16.0159f, -59.3904f), DiVec3(122.646f, 37.3991f, 68.647f));

        DiVec3 vSceneAABBPointsLightSpace[8];
        for (int i = 0; i < 8; i++)
            vSceneAABBPointsLightSpace[i] = matLightCameraView * sceneBounds.GetAllCorners()[i];

        float fFrustumIntervalBegin, fFrustumIntervalEnd;
        DiVec3 vLightCameraOrthographicMin;  // light space frustum aabb 
        DiVec3 vLightCameraOrthographicMax;
        float fCameraNearFarRange = camera->GetFarClipDistance() - camera->GetNearClipDistance();

        static const DiVec3 g_vFLTMAX( FLT_MAX, FLT_MAX, FLT_MAX );
        static const DiVec3 g_vFLTMIN( -FLT_MAX, -FLT_MAX, -FLT_MAX );

        DiVec3 vWorldUnitsPerTexel(0, 0, 0);
        int cur_num_splits = 4;
        for (int i = 0; i < cur_num_splits; ++i)
        {
            fFrustumIntervalBegin = 0.0f;

            fFrustumIntervalEnd = cascadePartitionsZeroToOne[i];
            fFrustumIntervalBegin /= cascadePartitionsMax;
            fFrustumIntervalEnd /= cascadePartitionsMax;
            fFrustumIntervalBegin = fFrustumIntervalBegin * fCameraNearFarRange;
            fFrustumIntervalEnd = fFrustumIntervalEnd * fCameraNearFarRange;

            DiVec3 vFrustumPoints[8];
            //DiMat4 matViewCameraProjection = camera->GetProjectionMatrix();
            //CreateFrustumPointsFromCascadeInterval(fFrustumIntervalBegin, fFrustumIntervalEnd,
            //    matViewCameraProjection, vFrustumPoints);
            DiCamera* temp = camera->GetSplitFrustum(fFrustumIntervalBegin, fFrustumIntervalEnd);
            for (int t = 0; t < 8; t++)
                vFrustumPoints[t] = temp->GetWorldSpaceCorners()[t];
            DI_DELETE temp;

            vLightCameraOrthographicMin = g_vFLTMAX;
            vLightCameraOrthographicMax = g_vFLTMIN;

            DiMat4 matInverseViewCamera = viewMat.inverse();

            DiVec3 vTempTranslatedCornerPoint;
            // This next section of code calculates the min and max values for the orthographic projection.
            for (int icpIndex = 0; icpIndex < 8; ++icpIndex)
            {
                // Transform the frustum from camera view space to world space.
                //vFrustumPoints[icpIndex] = matInverseViewCamera * vFrustumPoints[icpIndex];
                // Transform the point from world space to Light Camera Space.
                vTempTranslatedCornerPoint = matLightCameraView * vFrustumPoints[icpIndex];
                // Find the closest point.
                vLightCameraOrthographicMin = DiVec3::Min(vTempTranslatedCornerPoint, vLightCameraOrthographicMin);
                vLightCameraOrthographicMax = DiVec3::Max(vTempTranslatedCornerPoint, vLightCameraOrthographicMax);
            }

            DiVec3 vDiagonal = vFrustumPoints[0] - vFrustumPoints[6];
            float fCascadeBound = vDiagonal.length();

            DiVec3 vBoarderOffset = (vDiagonal - (vLightCameraOrthographicMax - vLightCameraOrthographicMin));
            vBoarderOffset *= 0.5f;
            vBoarderOffset.z = 0;

            // Add the offsets to the projection.
            vLightCameraOrthographicMax += vBoarderOffset;
            vLightCameraOrthographicMin -= vBoarderOffset;

            float bufferSize = 1024;
            float fWorldUnitsPerTexel = fCascadeBound / bufferSize;
            vWorldUnitsPerTexel.x = fWorldUnitsPerTexel;
            vWorldUnitsPerTexel.y = fWorldUnitsPerTexel;

            float fLightCameraOrthographicMinZ = vLightCameraOrthographicMin.z;

            vLightCameraOrthographicMin /= vWorldUnitsPerTexel;
            vLightCameraOrthographicMin.floor();
            vLightCameraOrthographicMin *= vWorldUnitsPerTexel;

            vLightCameraOrthographicMax /= vWorldUnitsPerTexel;
            vLightCameraOrthographicMax.ceil();
            vLightCameraOrthographicMax *= vWorldUnitsPerTexel;

            float fNearPlane = 0.0f;
            float fFarPlane = 10000.0f;

            DiVec3 vLightSpaceSceneAABBminValue = g_vFLTMAX;  // world space scene aabb 
            DiVec3 vLightSpaceSceneAABBmaxValue = g_vFLTMIN;
            // We calculate the min and max vectors of the scene in light space. The min and max "Z" values of the  
            // light space AABB can be used for the near and far plane. This is easier than intersecting the scene with the AABB
            // and in some cases provides similar results.
            for (int index = 0; index < 8; ++index)
            {
                vLightSpaceSceneAABBminValue = DiVec3::Min(vSceneAABBPointsLightSpace[index], vLightSpaceSceneAABBminValue);
                vLightSpaceSceneAABBmaxValue = DiVec3::Max(vSceneAABBPointsLightSpace[index], vLightSpaceSceneAABBmaxValue);
            }

            // The min and max z values are the near and far planes.
            fNearPlane = abs(vLightSpaceSceneAABBminValue.z);
            fFarPlane = abs(vLightSpaceSceneAABBmaxValue.z);
            if (fFarPlane < fNearPlane) 
                std::swap(fNearPlane, fFarPlane);

            const DiQuat& parentOrientation = mParentNode->GetDerivedOrientation();
            DiVec3 derivedDir = parentOrientation * (mDirection);
            DiCamera* shadowCamera = sceneManager->GetCameraPool().GetCamera();
            mShadowParams.shadowCameras[i] = shadowCamera;

            //shadowCamera->SetPosition(camera->GetDerivedPosition());
            //shadowCamera->SetOrientation(camera->GetOrientation());
            shadowCamera->SetPosition(-320.0f, 500.0f, -220.3f);
            shadowCamera->LookAt(0, 0, 0);

            shadowCamera->SetProjectionType(PT_ORTHOGRAPHIC);
            shadowCamera->SetOrthoWindow(vLightCameraOrthographicMin.x, vLightCameraOrthographicMin.y, 
                 vLightCameraOrthographicMax.x, vLightCameraOrthographicMax.y);
            //shadowCamera->SetOrthoWindow(300, 300);
            shadowCamera->SetFarClipDistance(fFarPlane);
            shadowCamera->SetNearClipDistance(fNearPlane);
            shadowCamera->GetProjectionMatrix();

            mCascadePartitionsFrustum[i] = fFrustumIntervalEnd;

            //DI_DEBUG("Shadowcamera[%d] near=%f, far=%f",i,fNearPlane,fFarPlane);
        }

#endif

        /*
        const DiQuat& parentOrientation = mParentNode->GetDerivedOrientation();
        DiVec3 derivedDir = parentOrientation * mDirection;

        DiCamera* camera = sceneManager->GetCamera();

        if (!mShadowCamera)
            mShadowCamera = DI_NEW DiCamera(sceneManager);

        mShadowCamera->SetProjectionType(PT_ORTHOGRAPHIC);
        mShadowCamera->SetDirection(derivedDir);
        mShadowCamera->SetPosition(camera->GetDerivedPosition());

        DiVec2 eyeProjMinMax(0, 1);

        DiMat4 cascadeMatricies[MAX_CASCADE_SPLITS];
        DiVec4 cascadeScales[MAX_CASCADE_SPLITS];
        DiVec4 cascadeRescales[MAX_CASCADE_SPLITS];
        DiVec4 cascadeScaledDeltaBiases[MAX_CASCADE_SPLITS];

        float cascadeZes[MAX_CASCADE_SPLITS + 4];
        for (unsigned long i = 0; i < sizeof(cascadeZes) / sizeof(cascadeZes[0]); ++i)
            cascadeZes[i] = FLT_MAX;

        DiMat4 mCamProj = camera->GetProjectionMatrix();
        DiMat4 mCamView = camera->GetViewMatrix();
        DiMat4 mCamViewI = camera->GetViewMatrix().inverse();
        DiMat4 mCamProjI = camera->GetProjectionMatrix().inverse();
        DiMat4 mCamProjToLightSpace = mCamProjI * mCamViewI * mShadowCamera->DiFrustum::GetViewMatrix();

        //Cam Space near plane corners
        DiVec3 nearCornersLS[4] =
        {
            mCamProjToLightSpace * (DiVec3(-1, -1, eyeProjMinMax.x)),
            mCamProjToLightSpace * (DiVec3(1, -1, eyeProjMinMax.x)),
            mCamProjToLightSpace * (DiVec3(-1, 1, eyeProjMinMax.x)),
            mCamProjToLightSpace * (DiVec3(1, 1, eyeProjMinMax.x))
        };

        const DiAABB& sceneBounds = sceneManager->GetBoundingBox();
        DiVec2 zrange(FLT_MAX, -FLT_MAX);
        for (int i = 0; i < 8; i++)
        {
            float curz = (mCamView * sceneBounds.GetAllCorners()[i]).x;
            zrange.x = DiMath::Min(curz, zrange.x);
            zrange.y = DiMath::Max(curz, zrange.y);
        }

        for (int i = 0; i < 4; ++i)
        {
            //correcting light's near taking into account light frustum
            zrange.x = DiMath::Min(zrange.x, nearCornersLS[i].z);
        }

        float n = (mCamProjI * DiVec3(0, 0, eyeProjMinMax.x)).z;
        float f = (mCamProjI * DiVec3(0, 0, eyeProjMinMax.y)).z;

        int cur_num_splits = 4;
        for (int i = 0; i < cur_num_splits; i++)
        {
            float a = 0.8f;
            float w = (i + 1.0f) / cur_num_splits;
            float camSpaceZ = a * n * pow(f / n, w) + (1 - a) * (n + w * (f - n));

            //Cam Space near plane corner
            float d = (mCamProj * DiVec3(0, 0, camSpaceZ)).z;

            DiVec3 splitCornersLS[4] =
            {
                //Cam Space split plane corners
                mCamProjToLightSpace * DiVec3(-1, -1, d),
                mCamProjToLightSpace * DiVec3(1, -1, d),
                mCamProjToLightSpace * DiVec3(-1, 1, d),
                mCamProjToLightSpace * DiVec3(1, 1, d),
            };

            if (i != cur_num_splits - 1)
                cascadeZes[i] = d;
            float minX(FLT_MAX), minY(FLT_MAX), maxX(-FLT_MAX), maxY(-FLT_MAX);
            float minZ(zrange.x), maxZ(zrange.x);

            for (int j = 0; j < 4; ++j)
            {
                minX = std::min(minX, std::min(nearCornersLS[j].x, splitCornersLS[j].x));
                minY = std::min(minY, std::min(nearCornersLS[j].y, splitCornersLS[j].y));
                maxX = std::max(maxX, std::max(nearCornersLS[j].x, splitCornersLS[j].x));
                maxY = std::max(maxY, std::max(nearCornersLS[j].y, splitCornersLS[j].y));
                maxZ = std::max(maxZ, std::max(nearCornersLS[j].z, splitCornersLS[j].z));
            }

            DiCamera* shadowCamera = sceneManager->GetCameraPool().GetCamera();
            mShadowParams.shadowCameras[i] = shadowCamera;
            shadowCamera->SetPosition(camera->GetDerivedPosition());
            shadowCamera->SetDirection(derivedDir);
            shadowCamera->SetProjectionType(PT_ORTHOGRAPHIC);
            //shadowCamera->SetOrthoWindow(minX, minY, maxX, maxY);
            shadowCamera->SetOrthoWindow(100,100);
            shadowCamera->SetFarClipDistance(maxZ);
            shadowCamera->SetNearClipDistance(minZ);
            shadowCamera->GetProjectionMatrix();
        }
        */

        //DiCamera* camera = sceneManager->GetCamera();
        //UpdateSplitDist(camera);

        //int cur_num_splits = 4;
        //for (int i = 0; i < cur_num_splits; i++)
        //{
        //    UpdateFrustumPoints(mShadowFrustums[i], camera->GetDerivedPosition(), camera->GetDirection());
        //    DiCamera* shadowCamera = sceneManager->GetCameraPool().GetCamera();

        //    float extrusionDistance = camera->GetFarClipDistance();
        //    DiVec3 worldDirection = mParentNode->GetDerivedOrientation() * mDirection;

        //    // Calculate initial position & rotation
        //    DiVec3 pos = camera->GetDerivedPosition() - extrusionDistance * worldDirection;
        //    shadowCamera->SetPosition(pos);
        //    shadowCamera->SetDirection(worldDirection);
        //    mShadowParams.shadowCameras[i] = shadowCamera;

        //    shadowCamera->GetProjectionMatrix();
        //    ApplyCropMatrix(shadowCamera, mShadowFrustums[i]);
        //}
        
    }

    void DiDirLight::SetupShadowCameraImpl(DiSceneManager* sceneManager, DiCamera* shadowCamera,
        float nearSplit, float farSplit)
    {
        DiCamera* camera = sceneManager->GetCamera();
        float extrusionDistance = camera->GetFarClipDistance();
        const DiQuat& parentOrientation = mParentNode->GetDerivedOrientation();
        DiVec3 derivedDir = parentOrientation * mDirection;

        DiVec3 worldDirection = mParentNode->GetDerivedOrientation() * mDirection;

        // Calculate initial position & rotation
        DiVec3 pos = camera->GetDerivedPosition() - extrusionDistance * worldDirection;
        shadowCamera->SetPosition(camera->GetDerivedPosition());
        shadowCamera->SetDirection(derivedDir);
        
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
        const DiVec3& parentPosition = shadowCamera->GetDerivedPosition();
        splitFrustum->MoveRelative(parentPosition);
        splitFrustum->Rotate(parentOrientation);
        splitFrustum->GetProjectionMatrix();
        DiAABB shadowbox = splitFrustum->GetBoundingBox();

        DI_DELETE splitFrustum;

        float maxz = DiMath::Max(abs(shadowbox.GetMaximum().z), abs(shadowbox.GetMinimum().z));
        shadowCamera->SetProjectionType(PT_ORTHOGRAPHIC);
        shadowCamera->SetNearClipDistance(0.1f);
        shadowCamera->SetFarClipDistance(maxz);

        // Center shadow camera on the bounding box. Can not snap to texels yet as the shadow map viewport is unknown
        QuantizeShadowCamera(shadowCamera, DiRect(0, 0, 0, 0), shadowbox);

        shadowCamera->GetProjectionMatrix();
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

    void DiDirLight::GetShadowCameraForCascade(const DiCamera *cam, DiCamera *texCam,
        uint16 iteration, float nearSplit, float farSplit)
    {
        DiVec3 pos, dir;
        DiQuat q;

        // reset custom view / projection matrix in case already set
        texCam->SetCustomViewMatrix(false);
        texCam->SetCustomProjectionMatrix(false);

        // set up the shadow texture
        // Set ortho projection
        texCam->SetProjectionType(PT_ORTHOGRAPHIC);
        // set easy FOV and near dist so that texture covers far dist
        texCam->SetFOVy(DiDegree(90));
        texCam->SetAspectRatio(1.0f);

        float nearClip = cam->GetNearClipDistance();
        float farClip = cam->GetFarClipDistance();
        farClip = 12000;

        texCam->SetNearClipDistance(nearClip);
        texCam->SetFarClipDistance(farClip);

        // Calculate texCam direction, which same as directional light direction
        dir = -mDirection; // backwards since point down -z
        dir.normalise();

        //get light orientation

        DiVec3 up = DiVec3::UNIT_Y;
        // Check it's not coincident with dir
        if (DiMath::Abs(up.dotProduct(dir)) >= 1.0f)
        {
            // Use camera up
            up = DiVec3::UNIT_Z;
        }
        // cross twice to rederive, only direction is unaltered
        DiVec3 left = dir.crossProduct(up);
        left.normalise();
        up = dir.crossProduct(left);
        up.normalise();
        // Derive quaternion from axes
        q.FromAxes(left, up, dir);

        // Find minimum enclosing sphere for view frustum
        // We do this in local space so that we don't have slight precision variation between frames
        float vertical = DiMath::Tan(cam->GetFOVy()*0.5f);
        float horizontal = DiMath::Tan(cam->GetFOVy()*0.5f * cam->GetAspectRatio());

        DiVec3 topLeftFar;
        topLeftFar.x = horizontal * farSplit;
        topLeftFar.y = vertical * farSplit;
        topLeftFar.z = farSplit;

        DiVec3 bottomRightNear;
        bottomRightNear.z = nearSplit;
        bottomRightNear.x = horizontal * bottomRightNear.z;
        bottomRightNear.y = vertical * bottomRightNear.z;

        float dist = (topLeftFar.squaredLength() - bottomRightNear.squaredLength()) / (2 * (topLeftFar.z - bottomRightNear.z));

        if (dist > farSplit)
        {
            dist = farSplit;
        }

        DiVec3 localPos(0, 0, -dist); // we have now found the point along frustum center which is equi-distant to the opposing corner positions

        float diameter = 2.0f * topLeftFar.distance(-localPos);
        pos = cam->GetDerivedPosition() + cam->GetDerivedOrientation() * localPos;

        diameter *= (float)1.01; // allow some boundary pixels for filtering etc. TODO - make this a user constant
        pos += dir * 0.5 * (farClip - nearClip); // pull light back so we can see the scene


        //calculate window size
        texCam->SetOrthoWindowWidth(diameter);


        // Round local x/y position based on a world-space texel; this helps to reduce
        // jittering caused by the projection moving with the camera
        float cameraResolution = 1024;
        float worldTexelSize = (texCam->GetOrthoWindowWidth()) / cameraResolution;

        //convert world space camera position into light space
        DiVec3 lightSpacePos = q.Inverse() * pos;

        //snap to nearest texel
        lightSpacePos.x -= fmod(lightSpacePos.x, worldTexelSize);
        lightSpacePos.y -= fmod(lightSpacePos.y, worldTexelSize);

        //convert back to world space
        pos = q * lightSpacePos;

        // Finally set position
        texCam->SetPosition(pos);
        texCam->SetOrientation(q);

        DiMat4 viewmat = texCam->GetViewMatrix();
        DiMat4 proj = texCam->GetProjectionMatrix();
        DiMat4 a = viewmat;
    }

}