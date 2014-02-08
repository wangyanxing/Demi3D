
#include "GfxPch.h"
#include "PostEffectManager.h"
#include "Sprite.h"
#include "RenderPipeline.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "RenderBatchGroup.h"
#include "PostEffect.h"
#include "PostEffectPass.h"
#include "ShaderParam.h"
#include "AssetManager.h"
#include "MaterialPredefine.h"
#include "RenderTarget.h"

namespace Demi
{
    DiPostEffectManager::DiPostEffectManager(DiRenderWindow* renderWindow)
        :mRenderWindow(renderWindow)
    {
        Init();
    }

    DiPostEffectManager::~DiPostEffectManager()
    {
        Release();
    }

    void DiPostEffectManager::Init()
    {
        mScreenQuad = CheckScreenQuad(1);

        DiMaterialPtr mat = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>("_post_out");

        mat->LoadShader(DiMaterialDefine::SCREEN_QUAD_VERT_SHADER,
                        DiMaterialDefine::SCREEN_QUAD_PIXEL_SHADER);
        if (Driver->ReversePostpassCullmode())
            mat->SetCullMode(CULL_CCW);

        mScreenQuad->SetBatchGroup(BATCH_POST_FILTER);
        mScreenQuad->SetMaterial(mat);

        CreateDefaultEffects();
    }

    void DiPostEffectManager::Release()
    {
        for (auto it = mPostList.begin(); it != mPostList.end(); ++it)
        {
            DI_DELETE(*it);
        }
        mPostList.clear();
        mPostMap.clear();

        for (auto it = mQuads.begin(); it != mQuads.end(); ++it)
        {
            DI_DELETE(it->second);
        }
        mQuads.clear();
        mScreenQuad = nullptr;
    }

    void DiPostEffectManager::Process(DiRenderPipeline* pipe)
    {
        PreProcess(pipe);

        for (auto it = mPostList.begin(); it != mPostList.end(); ++it)
            (*it)->Process();

        RenderOutput(pipe);
    }

    void DiPostEffectManager::RenderOutput(DiRenderPipeline* pipe)
    {
        mRenderWindow->GetRenderBuffer()->Bind();

        DrawQuad(mScreenQuad->mMaterial.get(),1);
    }

    DiPostEffect* DiPostEffectManager::CreatePostEffect( const DiString& name )
    {
        if (mPostMap.find(name) != mPostMap.end())
        {
            DI_WARNING("The postEffect : %s has already existed",name.c_str());
            return mPostMap[name];
        }

        DiPostEffect* neweffect = DI_NEW DiPostEffect(name,this);
        mPostList.push_back(neweffect);
        mPostMap[name] = neweffect;
        return neweffect;
    }

    void DiPostEffectManager::PreProcess(DiRenderPipeline* pipe)
    {
        pipe->GetShaderEnvironment()->SetIdentity(Driver->NeedTextureFlipping());
    }

    void DiPostEffectManager::DrawQuad( DiMaterial* mat , int quadscale)
    {
        if (mat)
        {
            mat->SetForceWireframe(true);
            DiSprite* quad = CheckScreenQuad(quadscale);
            mat->Bind();
            Driver->RenderGeometry(quad);
        }
    }

    void DiPostEffectManager::CreateDefaultEffects()
    {
        //////////////////////////////////////////////////////////////////////////
        /*
          DiPostEffect* effectGray = CreatePostEffect("Gray");
          effectGray->SetEnable(true);
          DiPostEffectPass* passGray = effectGray->CreatePass("output");
          passGray->BuildTexture(1,D3DFMT_A8R8G8B8);
          passGray->BuildMaterial(DiMaterialDefine::SCREEN_QUAD_VERT_SHADER,"post_gray_p");
          passGray->SetInput("image",INPUT_PREV_EFFECT);
          passGray->SetInput("grayRate",1.0f);
          */

        //////////////////////////////////////////////////////////////////////////
        
        DiPostEffect* effect = CreatePostEffect("Bloom");
        effect->SetEnable(false);
        DiPostEffectPass* bright = effect->CreatePass("bright");
        bright->BuildTexture(1, PF_A8R8G8B8);
        bright->BuildMaterial(DiMaterialDefine::SCREEN_QUAD_VERT_SHADER, "post_bright_p");
        bright->SetInput("image",INPUT_PREV_EFFECT);
        bright->GetShaderParam()->WriteFloat3("vLinearBright",DiVec3(1,-1,1));
        //bright->GetShaderParam()->WriteFloat2("hdrParam", DiVec2(0.5f, 0.08f));
        
        DiPostEffectPass* down4 = effect->CreatePass("down4");
        down4->BuildTexture(4, PF_A8R8G8B8);
        down4->BuildMaterial("post_down4_v","post_down4_p");
        down4->SetInput("image",INPUT_PREV_PASS);

        DiPostEffectPass* vertBlur = effect->CreatePass("vert");
        vertBlur->BuildTexture(4, PF_A8R8G8B8);
        vertBlur->BuildMaterial("post_bloom_vert_v","post_bloom_p");
        vertBlur->SetInput("image",INPUT_PREV_PASS);
        
        DiPostEffectPass* horzBlur = effect->CreatePass("horz");
        horzBlur->BuildTexture(4, PF_A8R8G8B8);
        horzBlur->BuildMaterial("post_bloom_horz_v","post_bloom_p");
        horzBlur->SetInput("image",INPUT_PREV_PASS);
        
        DiPostEffectPass* outscene = effect->CreatePass("output");
        outscene->BuildTexture(1, PF_A8R8G8B8);
        outscene->BuildMaterial(DiMaterialDefine::SCREEN_QUAD_VERT_SHADER,"post_bloom_final_p");
        outscene->SetInput("image",INPUT_PREV_EFFECT);
        outscene->SetInput("imageBloom",INPUT_PREV_PASS);
        
        RefreshEffectChain();
    }

    void DiPostEffectManager::RefreshEffectChain()
    {
        for (auto it = mPostList.begin(); it != mPostList.end(); ++it)
            (*it)->RefereshPasses();

        if (GetLastEnabledEffect())
        {
            mScreenQuad->GetMaterial()->GetShaderParameter()->WriteTexture2D("image",
                GetLastEnabledEffect()->GetLastValidPass()->GetOutTexture());
        }
        else
        {
            mScreenQuad->GetMaterial()->GetShaderParameter()->WriteTexture2D("image",
                mRenderWindow->GetCanvasTexture());
        }
    }

    DiTexturePtr DiPostEffectManager::GetLastOutTexture()
    {
        if (mPostList.empty())
            return mRenderWindow->GetCanvasTexture();

        return mPostList.back()->GetLastOutTexture();
    }

    DiSprite* DiPostEffectManager::CheckScreenQuad( int scale )
    {
        if(mQuads.contains(scale))
            return mQuads[scale];

        DiSprite* sp = DI_NEW DiSprite();
        DiQuadElement quad;
        quad.absCoord = false;
        quad.left = 0;
        quad.right = 1.0f / scale;
        quad.top = 0;
        quad.bottom = 1.0f / scale;

        sp->AddQuad(quad);
        sp->UpdateGeometry(mRenderWindow->GetRenderBuffer());

        mQuads[scale] = sp;
        return sp;
    }

    DiPostEffect* DiPostEffectManager::GetPrevEnabledEffect( DiPostEffect* pe )
    {
        DiPostEffect* lastValid = nullptr;

        for (auto it = mPostList.begin(); it != mPostList.end(); ++it)
        {
            if ((*it) == pe)
                return lastValid;
            else if ((*it)->GetEnable())
                lastValid = *it;
        }

        return lastValid;
    }

    DiPostEffect* DiPostEffectManager::GetLastEnabledEffect()
    {
        if (mPostList.empty())
            return nullptr;

        for (int i = (int)mPostList.size()-1; i >=0; --i)
        {
            if (mPostList[i]->GetEnable())
                return mPostList[i];
        }

        return nullptr;
    }

    DiPostEffect* DiPostEffectManager::GetEffect( const DiString& name )
    {
        auto it = mPostMap.find(name);
        if ( it != mPostMap.end() )
            return it->second;

        return nullptr;
    }
}