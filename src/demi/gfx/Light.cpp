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

namespace Demi 
{
    DiLight::DiLight(LightType type)
        :mType(type),
        mColor(DiColor::White)
    {
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

}