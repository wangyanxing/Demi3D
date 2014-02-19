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
#include "PostEffect.h"
#include "RenderTarget.h"
#include "AssetManager.h"
#include "PostEffectPass.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "PostEffectManager.h"

namespace Demi
{
    DiPostEffect::DiPostEffect( const DiString& name , DiPostEffectManager* mgr)
        :mName(name),
        mEnable(true),
        mMgr(mgr)
    {

    }

    DiPostEffect::~DiPostEffect()
    {
        for (auto it = mPasses.begin(); it != mPasses.end(); ++it)
        {
            DI_DELETE (*it);
        }

        mPasses.clear();
    }

    DiPostEffectPass* DiPostEffect::CreatePass(const DiString& name)
    {
        DiPostEffectPass* pass = DI_NEW DiPostEffectPass(this,name);
        mPasses.push_back(pass);

        return pass;
    }

    void DiPostEffect::Process()
    {
        if (!mEnable)
            return;

        for (auto it = mPasses.begin(); it != mPasses.end(); ++it)
        {
            (*it)->Process();
        }
    }

    DiTexturePtr DiPostEffect::GetLastOutTexture()
    {
        if (mPasses.empty())
        {
            return mMgr->GetRenderWindow()->GetCanvasTexture();
        }

        return mPasses.back()->GetOutTexture();
    }

    int DiPostEffect::GetPassID( const DiString& pass )
    {
        int count = 0;
        for (auto it = mPasses.begin(); it != mPasses.end(); ++it, ++count)
        {
            if ((*it)->GetPassName() == pass)
            {
                return count;
            }
        }
        return -1;
    }

    DiPostEffectPass* DiPostEffect::GetPrevValidPass( const DiString& pass )
    {
        int id = GetPassID(pass);
        if (id == -1)
        {
            return nullptr;
        }
        else if (id == 0)
        {
            DiPostEffect* preEffect = mMgr->GetPrevEnabledEffect(this);
            if (!preEffect)
            {
                return nullptr;
            }
            else
            {
                return preEffect->GetLastValidPass();
            }
        }
        else
        {
            for (size_t i = id; i >=1; --i)
            {
                if (mPasses[i]->GetOutTexture())
                {
                    return mPasses[i-1];
                }
            }
            return nullptr;
        }
    }

    DiPostEffectPass* DiPostEffect::GetLastValidPass()
    {
        if (mPasses.empty())
        {
            return nullptr;
        }
        for (int i = (int)mPasses.size()-1; i >=0; --i)
        {
            if (mPasses[i]->GetOutTexture())
            {
                return mPasses[i];
            }
        }
        return nullptr;
    }

    void DiPostEffect::SetEnable( bool val )
    {
        bool old = mEnable;
        mEnable = val;
    
        if (val != old)
        {
            mMgr->RefreshEffectChain();
        }
    }

    DiPostEffectPass* DiPostEffect::GetPass( const DiString& pass )
    {
        int id = GetPassID(pass);
        if (id >= 0 && id < (int)mPasses.size())
        {
            return mPasses[id];
        }
        return nullptr;
    }

    void DiPostEffect::RefereshPasses()
    {
        if (!mEnable)
            return;
        
        for (auto it = mPasses.begin(); it != mPasses.end(); ++it)
        {
            (*it)->RefreshInput();
        }
    }
}