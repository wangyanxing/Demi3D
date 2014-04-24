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

        void                    SetOutputToBackBuffer(bool val) { mOutput = val; }

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

        bool                    mOutput;
    };
}

#endif // PostEffectManager_h__