
/********************************************************************
    File:       PostEffectManager.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Texture.h"

namespace Demi
{
    class DI_GFX_API DiPostEffectManager : public DiBase
    {
    public:

        DiPostEffectManager(DiRenderWindow* renderWindow);

        ~DiPostEffectManager();

        typedef DiVector<DiPostEffect*> PostChains;

        typedef DiHashMap<DiString,DiPostEffect*> PostMap;

    public:

        void                    Process(DiRenderPipeline* pipe);

        DiPostEffect*           CreatePostEffect(const DiString& name);

        DiPostEffect*           GetEffect(const DiString& name);

        void                    DrawQuad(DiMaterial* mat, int quadscale = 1);

        DiPostEffect*           GetPrevEnabledEffect(DiPostEffect* pe);

        DiPostEffect*           GetLastEnabledEffect();

        void                    RefreshEffectChain();

        DiRenderWindow*         GetRenderWindow() const { return mRenderWindow; }

    protected:

        void                    Init();

        void                    Shutdown();

    protected:

        void                    RenderOutput(DiRenderPipeline* pipe);

        void                    PreProcess(DiRenderPipeline* pipe);

        void                    CreateDefaultEffects();

        DiTexturePtr            GetLastOutTexture();

        DiSprite*               CheckScreenQuad(int scale);

    protected:

        DiSprite*               mScreenQuad;

        DiMap<int,DiSprite*>    mQuads;

        PostChains              mPostList;
                                
        PostMap                 mPostMap;
                                
        DiRenderWindow*         mRenderWindow;
    };
}