
#include "PostEffectPass.h"
#include "AssetManager.h"
#include "PostEffectManager.h"
#include "GfxDriver.h"
#include "RenderTarget.h"
#include "ShaderParam.h"
#include "PostEffect.h"
#include "RenderWindow.h"
#include "GBuffer.h"

namespace Demi
{

    DiPostEffectPass::DiPostEffectPass( DiPostEffect* parent, const DiString& name )
        :mParent(parent)
        ,mPassName(name)
        ,mRenderTarget(NULL)
        ,mAutoClear(true)
    {
        mInput.first = "image";
        mInput.second = INPUT_PREV_PASS;
    }

    DiPostEffectPass::~DiPostEffectPass()
    {
        DiString texname = mPassName + "_t";
        DiString matname = mPassName + "_m";
        DiAssetManager::GetInstance().DestroyAsset(texname);
        DiAssetManager::GetInstance().DestroyAsset(matname);
    }

    void DiPostEffectPass::BuildTexture( int scale, DiPixelFormat format )
    {
        DiString texname = mPassName + "_t";

        mQuadScale = scale;

        mTexture = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(texname);
        mTexture->SetDimensions(0,0);
        mTexture->SetAdaptedRT(mParent->GetPostEffectManager(
            )->GetRenderWindow()->GetRenderBuffer());
        mTexture->SetViewportScale(DiVec2(1,1));
        mTexture->SetFormat(format);
        mTexture->SetResourceUsage(RU_NONE);
        mTexture->SetUsage(TU_RENDER_TARGET);
        mTexture->CreateTexture();
        mTexture->SetAddressing(AM_BORDER);
        mRenderTarget = mTexture->GetRenderTarget();
        if (mRenderTarget)
        {
            mRenderTarget->SetShouldUpdate(false);
            mRenderTarget->SetDepthBufferPool(0);
        }
    }

    void DiPostEffectPass::BuildMaterial( const DiString& vpshader, const DiString& fpshader )
    {
        DiString matname = mPassName + "_m";
        mMaterial = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>(matname);

        mMaterial->LoadVertexShader(vpshader);
        mMaterial->LoadPixelShader(fpshader);
    }

    void DiPostEffectPass::Process()
    {
        if (!mMaterial)
            return;

        if (mRenderTarget) 
        {
            mRenderTarget->SetClearPerFrame(mAutoClear);
            mRenderTarget->Bind();
        }

        mParent->GetPostEffectManager()->DrawQuad(mMaterial.get(),mQuadScale);
    }

    void DiPostEffectPass::SetInput( const DiString& paramName, DiTexturePtr tex )
    {
        if (!mMaterial)
            return;

        mMaterial->GetShaderParameter()->WriteTexture2D(paramName,tex);
    }

    void DiPostEffectPass::SetInput( const DiString& paramName, const DiString& texturename )
    {
        if (!mMaterial)
            return;

        mMaterial->GetShaderParameter()->WriteTexture2D(paramName,texturename);
    }

    void DiPostEffectPass::SetInput( const DiString& paramName, PassInputType inputtype )
    {
        if (!mMaterial)
            return;

        mInput.first = paramName;
        mInput.second = inputtype;

        DiTexturePtr canvas = mParent->GetPostEffectManager(
            )->GetRenderWindow()->GetCanvasTexture();

        if (inputtype == INPUT_SCENE_CANVAS)
        {
            mMaterial->GetShaderParameter()->WriteTexture2D(paramName,canvas);
        }
        else if (inputtype == INPUT_PREV_PASS)
        {
            DiPostEffectPass* prePass = mParent->GetPrevValidPass(GetPassName());
            if (prePass)
                mMaterial->GetShaderParameter()->WriteTexture2D(paramName,prePass->GetOutTexture());
            else
                mMaterial->GetShaderParameter()->WriteTexture2D(paramName,canvas);
        }
        else if (inputtype == INPUT_PREV_EFFECT)
        {
            DiPostEffect* preEffect = mParent->GetPostEffectManager()->GetPrevEnabledEffect(mParent);
            if (!preEffect)
                mMaterial->GetShaderParameter()->WriteTexture2D(paramName,canvas);
            else
            {
                DiPostEffectPass* lastvalid = preEffect->GetLastValidPass();
                if (lastvalid)
                    mMaterial->GetShaderParameter()->WriteTexture2D(paramName,lastvalid->GetOutTexture());
                else
                    mMaterial->GetShaderParameter()->WriteTexture2D(paramName,canvas);
            }
        }
    }

    void DiPostEffectPass::SetInput( const DiString& paramName, float value )
    {
        mMaterial->GetShaderParameter()->WriteFloat(paramName,value);
    }

    void DiPostEffectPass::SetInput( const DiString& paramName, const DiVec3& value )
    {
        mMaterial->GetShaderParameter()->WriteFloat3(paramName,value);
    }

    void DiPostEffectPass::SetInput( const DiString& paramName, const DiVec4& value )
    {
        mMaterial->GetShaderParameter()->WriteFloat4(paramName,value);
    }

    void DiPostEffectPass::SetInputGBuffer(const DiString& paramName, int gbufferID)
    {
        DiTexturePtr tex = mParent->GetPostEffectManager()->GetRenderWindow()->GetGBuffer()->GetRTTexture(gbufferID);
        mMaterial->GetShaderParameter()->WriteTexture2D(paramName,tex);
    }

    void DiPostEffectPass::SetInputTex2D(const DiString& paramName, const DiString& texName)
    {
        mMaterial->GetShaderParameter()->WriteTexture2D(paramName,texName);
    }

    DiShaderParameter* DiPostEffectPass::GetShaderParam()
    {
        if (!mMaterial)
            return NULL;
        else
            return mMaterial->GetShaderParameter();
    }

    void DiPostEffectPass::SetBlendMode( DiBlendMode blend )
    {
        mMaterial->SetBlendMode(blend);
    }

    void DiPostEffectPass::RefreshInput()
    {
        SetInput(mInput.first,mInput.second);
    }

    void DiPostEffectPass::SetAutoClear(bool val) 
    {
        mRenderTarget->SetClearPerFrame(val); 
    }

    bool DiPostEffectPass::GetAutoClear() const 
    { 
        return mRenderTarget->GetClearPerFrame();
    }
}