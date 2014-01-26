
#include "GBuffer.h"
#include "RenderTarget.h"
#include "AssetManager.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "DepthBuffer.h"

namespace Demi 
{
    DiGBuffer::DiGBuffer(DiRenderWindow* renderWnd)
        :mParentWnd(renderWnd)
    {
        Create();
    }

    DiGBuffer::~DiGBuffer(void)
    {
    }

    void DiGBuffer::Create()
    {
        // create RTs
        DiString gbName;
        gbName = "_gb_0";

        DiTexturePtr tex;
        tex = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(gbName);
        tex->SetDimensions(0,0);
        tex->SetAdaptedRT(mParentWnd->GetRenderBuffer());
        tex->SetViewportScale(DiVec2(1,1));
        tex->SetFormat(PF_A32B32G32R32F);
        tex->SetResourceUsage(RU_NONE);
        tex->SetUsage(TU_RENDER_TARGET);
        tex->CreateTexture();
        tex->SetAddressing(AM_BORDER);

        DiRenderTarget* rt = tex->GetRenderTarget();
        if (rt)
            rt->SetShouldUpdate(false);
        //rt->SetDepthBufferPool(DiDepthBuffer::POOL_DEFAULT);

        //rt->AttachDepthBuffer(mParentWnd->GetRenderBuffer()->GetDepthBuffer());
    
        mTexture.push_back(tex);
        mRenderTargets.push_back(rt);

        // create materials
        mMaterial = DiMaterial::QuickCreate("gbuffer_v","gbuffer_p");
        mMaterial->SetDepthWrite(TRUE);
        mMaterial->SetDepthCheck(TRUE);
    }

    void DiGBuffer::Bind()
    {
        for(size_t i = 0; i < mRenderTargets.size(); i++)
        {
            DiRenderTarget* rt = mRenderTargets[i];
            if (rt) 
                rt->Bind((uint8)i);
        }
    }

}