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
#include "Light.h"
#include "AssetManager.h"
#include "RenderTarget.h"

namespace Demi 
{
    DiLight::DiLight(LightType type)
        :mType(type),
        mColor(DiColor::White),
        mShadowCameraDirty(true),
        mShadowCamera(nullptr),
        mSplitPadding(1.0f)
    {
        for (int i = 0; i < MAX_CASCADE_SPLITS; ++i)
            mShadowCameras[i] = nullptr;

        //test
        CreateShadowTextures();

        CalculateSplitPoints(MAX_CASCADE_SPLITS, 50.0f, 5000.0f, 0.93f);
    }

    DiLight::~DiLight(void)
    {
    }
    
    DiString& DiLight::GetType()
    {
        static DiString type = "Light";
        return type;
    }

    void DiLight::UpdateSplitDist(DiCamera* camera)
    {
        float fd = camera->GetFarClipDistance();
        float nd = camera->GetNearClipDistance();
        
        // temp
        float split_weight = 0.75f;
        int cur_num_splits = 4;

        float lambda = split_weight;

        float ratio = fd / nd;
        mShadowFrustums[0].neard = nd;

        for (int i = 1; i < cur_num_splits; i++)
        {
            float si = i / (float)cur_num_splits;

            mShadowFrustums[i].neard = lambda*(nd*powf(ratio, si)) + (1 - lambda)*(nd + (fd - nd)*si);
            mShadowFrustums[i - 1].fard = mShadowFrustums[i].neard * 1.005f;
        }
        mShadowFrustums[cur_num_splits - 1].fard = fd;
    }

    void DiLight::UpdateFrustumPoints(ShadowFrustum &f, const DiVec3 &center, const DiVec3 &view_dir)
    {
        DiVec3 up(0.0f, 1.0f, 0.0f);
        DiVec3 right = view_dir.crossProduct(up);

        DiVec3 fc = center + view_dir*f.fard;
        DiVec3 nc = center + view_dir*f.neard;

        right = right.normalise();
        up = right.crossProduct(view_dir);
        up.normalise();

        // these heights and widths are half the heights and widths of
        // the near and far plane rectangles
        float near_height = tan(f.fov / 2.0f) * f.neard;
        float near_width = near_height * f.ratio;
        float far_height = tan(f.fov / 2.0f) * f.fard;
        float far_width = far_height * f.ratio;

        f.point[0] = nc - up*near_height - right*near_width;
        f.point[1] = nc + up*near_height - right*near_width;
        f.point[2] = nc + up*near_height + right*near_width;
        f.point[3] = nc - up*near_height + right*near_width;

        f.point[4] = fc - up*far_height - right*far_width;
        f.point[5] = fc + up*far_height - right*far_width;
        f.point[6] = fc + up*far_height + right*far_width;
        f.point[7] = fc - up*far_height + right*far_width;
    }

    void DiLight::ApplyCropMatrix(DiCamera* shadowCamera, ShadowFrustum& f)
    {
        shadowCamera->SetProjectionType(PT_ORTHOGRAPHIC);
        shadowCamera->SetAspectRatio(1.0);
    }

    void DiLight::CreateShadowTextures()
    {
        for (int i = 0; i < MAX_CASCADE_SPLITS; ++i)
        {
            DiString name;
            static int st = 0;
            name.Format("_shad_tex_%d", st++);
            mShadowTextures[i] = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(name);
            mShadowTextures[i]->SetDimensions(1024, 1024);
            mShadowTextures[i]->SetFormat(PF_R32F);
            mShadowTextures[i]->SetUsage(TU_RENDER_TARGET);
            mShadowTextures[i]->CreateTexture();
            auto rt = mShadowTextures[i]->GetRenderTarget();
            rt->SetFlippingUV(true);
            rt->SetClearColor(DiColor::White);
        }
    }

    void DiLight::CalculateSplitPoints(uint16 cascadeCount, float firstSplitDist, float farDist, float lambda)
    {
        if (cascadeCount < 2)
        {
            DI_WARNING("The cascade count should be greator than 2");
            return;
        }

        mSplitPoints.resize(cascadeCount + 1);
        mCascadeCount = cascadeCount;

        mSplitPoints[0] = 0;
        firstSplitDist = DiMath::Max(0.001f, firstSplitDist);

        for (uint16 i = 1; i <= mCascadeCount; i++)
        {
            float fraction = (float)(i - 1) / (mCascadeCount - 1);
            float logDist = firstSplitDist * DiMath::Pow(farDist / firstSplitDist, fraction);
            float linDist = firstSplitDist + fraction * (farDist - firstSplitDist);
            float splitPoint = linDist + lambda * (logDist - linDist);
            mSplitPoints[i] = splitPoint;
        }
    }

    void DiLight::SetSplitPoints(const DiVector<float>& newSplitPoints)
    {
        if (newSplitPoints.size() < 3) // 3, not 2 since splits + 1 points
        {
            DI_WARNING("Cannot specify less than 2 splits");
            return;
        }
        mCascadeCount = newSplitPoints.size() - 1;
        mSplitPoints = newSplitPoints;
    }

    void DiLight::GetShadowCamera(const DiCamera *cam, DiCamera *texCam, uint16 iteration)
    {
        // apply the right clip distance.
        float nearDist = mSplitPoints[iteration];
        float farDist = mSplitPoints[iteration + 1];

        // Add a padding factor to internal distances so that the connecting split point will not have bad artifacts.
        if (iteration > 0)
            nearDist -= mSplitPadding;

        if (iteration < mCascadeCount - 1)
            farDist += mSplitPadding;

        mCurrentIteration = iteration;

        GetShadowCameraForCascade(cam, texCam, iteration, nearDist, farDist);
    }
}