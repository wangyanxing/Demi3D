


#ifndef PostEffectManager_h__
#define PostEffectManager_h__

#include "Texture.h"

namespace Demi
{
    class DI_GFX_API DiPostEffectManager : public DiBase
    {
    public:

        DiPostEffectManager(DiRenderWindow* renderWindow);

        ~DiPostEffectManager();

        typedef DiVector<DiPostEffect*> PostChains;

        typedef DiStrHash<DiPostEffect*> PostMap;

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

        void                    Release();

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

#endif // PostEffectManager_h__