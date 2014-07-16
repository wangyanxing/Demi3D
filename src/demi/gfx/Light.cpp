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
#include "GpuProgram.h"
#include "SceneManager.h"

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

    void DiLight::InitForShadowCasting(DiSceneManager* sm, ShadowTextureConfig config)
    {
        SetShadowCastEnable(true);
        mShadowConfig.resize(1,config);
        CreateShadowTextures(sm);
    }
    
    void DiLight::CreateShadowTextures(DiSceneManager* sm)
    {
        mShadowTextures.resize(mShadowConfig.size());
        mShadowCameras.resize(mShadowConfig.size());
        
        for (size_t i = 0; i < mShadowConfig.size(); ++i)
        {
            static int st = 0;
            DiString camname;
            camname.Format("_shad_cam_%d", st);
            DiString name;
            name.Format("_shad_tex_%d", st++);
            
            mShadowTextures[i] = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(name);
            mShadowTextures[i]->SetDimensions(mShadowConfig[i].width, mShadowConfig[i].height);
            mShadowTextures[i]->SetFormat(mShadowConfig[i].format);
            mShadowTextures[i]->SetUsage(TU_RENDER_TARGET);
            mShadowTextures[i]->SetAddressing(AM_CLAMP);
            mShadowTextures[i]->CreateTexture();
            auto rt = mShadowTextures[i]->GetRenderTarget();
            
            rt->SetFlippingUV(true);
            rt->SetClearColor(DiColor::White);
            
            DiCamera* cam = sm->CreateCamera(camname);
			cam->SetAspectRatio((float)mShadowConfig[i].width / (float)mShadowConfig[i].height);
            mShadowCameras[i] = cam;
        }
    }
    
    float DiLight::DeriveShadowNearClipDistance(const DiCamera* maincam) const
	{
		if (mShadowNearClipDist > 0)
			return mShadowNearClipDist;
		else
			return maincam->GetNearClipDistance();
	}
    
    float DiLight::DeriveShadowFarClipDistance(const DiCamera* maincam) const
	{
		if (mShadowFarClipDist >= 0)
			return mShadowFarClipDist;
		else
		{
            return 0;
		}
	}
}