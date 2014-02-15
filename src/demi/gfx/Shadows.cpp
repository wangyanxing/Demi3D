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
#include "Shadows.h"

namespace Demi
{
    DiShadowParams::DiShadowParams() : intensity(0.0f)
        , distance(0.0f)
        , resolution(1.0f)
        , nearFarRatio(0.002f)
        , numSplits(0)
    {
        for (int i = 0; i < MAX_LIGHT_SPLITS; ++i)
        {
            shadowCameras[i] = nullptr;
            shadowCasterBegin[i] = 0;
            shadowCasterEnd[i] = 0;
            shadowNearSplits[i] = 0;
            shadowFarSplits[i] = 0;
        }
    }

    void DiShadowParams::SetBias(float constBias, float slopeBias)
    {
        constantBias = constBias;
        slopeScaledBias = slopeBias;
    }

    void DiShadowParams::SetFocus(bool focus, bool nonUniform, bool autoSize, float quantize, float minView)
    {
        this->focus = focus;
        this->nonUniform = nonUniform;
        this->autoSize = autoSize;
        this->quantize = quantize;
        this->minView = minView;
    }

    void DiShadowParams::SetCascade(float split1, float split2, float split3, float split4, float fadeStart, float biasAutoAdjust /*= 1.0f*/)
    {
        splits[0] = split1;
        splits[1] = split2;
        splits[2] = split3;
        splits[3] = split4;

        this->fadeStart = fadeStart;
        this->biasAutoAdjust = biasAutoAdjust;
    }

    void DiShadowParams::Validate()
    {
        constantBias = DiMath::Clamp(constantBias, -1.0f, 1.0f);
        slopeScaledBias = DiMath::Clamp(slopeScaledBias, -16.0f, 16.0f);

        quantize = DiMath::Max(quantize, SHADOW_MIN_QUANTIZE);
        minView = DiMath::Max(minView, SHADOW_MIN_VIEW);

        for (unsigned i = 0; i < MAX_CASCADE_SPLITS; ++i)
            splits[i] = DiMath::Max(splits[i], 0.0f);
        fadeStart = DiMath::Clamp(fadeStart, std::numeric_limits<float>::epsilon(), 1.0f);
    }

    float DiShadowParams::GetShadowRange(void) const
    {
        float ret = 0.0f;
        for (unsigned i = 0; i < MAX_CASCADE_SPLITS; ++i)
            ret = DiMath::Max(ret, splits[i]);

        return ret;
    }

    CameraPool::CameraPool(DiSceneManager* sm) :sceneManager(sm)
    {
    }

    DiCamera* CameraPool::GetCamera()
    {
        if (currentID == cameras.size())
        {
            DiCamera* camera = DI_NEW DiCamera(sceneManager);
            cameras.push_back(camera);
        }
        DiCamera* ret = cameras.back();
        return ret;
    }

    void CameraPool::Reset()
    {
        currentID = 0;
    }

    CameraPool::~CameraPool()
    {
        for (auto c : cameras)
        {
            DI_DELETE c;
        }
    }

}