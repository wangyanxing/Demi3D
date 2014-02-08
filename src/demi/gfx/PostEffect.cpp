
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
        PassChain::iterator it;
        PassChain::iterator itend = mPasses.end();

        for (it = mPasses.begin(); it != itend; ++it)
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
        PassChain::iterator it;
        PassChain::iterator itend = mPasses.end();
        int count = 0;

        for (it = mPasses.begin(); it != itend; ++it, ++count)
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
            return NULL;
        }
        else if (id == 0)
        {
            DiPostEffect* preEffect = mMgr->GetPrevEnabledEffect(this);
            if (!preEffect)
            {
                return NULL;
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
            return NULL;
        }
    }

    DiPostEffectPass* DiPostEffect::GetLastValidPass()
    {
        if (mPasses.empty())
        {
            return NULL;
        }
        for (int i = (int)mPasses.size()-1; i >=0; --i)
        {
            if (mPasses[i]->GetOutTexture())
            {
                return mPasses[i];
            }
        }
        return NULL;
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
        return NULL;
    }

    void DiPostEffect::RefereshPasses()
    {
        if (!mEnable)
        {
            return;
        }

        PassChain::iterator it;
        PassChain::iterator itend = mPasses.end();

        for (it = mPasses.begin(); it != itend; ++it)
        {
            (*it)->RefreshInput();
        }
    }
}