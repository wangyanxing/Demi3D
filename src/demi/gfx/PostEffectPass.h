


#ifndef DiPostEffectPass_h__
#define DiPostEffectPass_h__


#include "Texture.h"
#include "Material.h"

namespace Demi
{
    enum PassInputType
    {
        INPUT_SCENE_CANVAS,
        INPUT_PREV_PASS,
        INPUT_PREV_EFFECT
    };

    class DI_GFX_API DiPostEffectPass : public DiBase
    {
    public:

        DiPostEffectPass(DiPostEffect* parent, const DiString& name);

        virtual             ~DiPostEffectPass();

    public:

        void                BuildTexture(int scale, DiPixelFormat format);

        void                BuildMaterial(const DiString& vpshader, const DiString& fpshader);

        void                SetInput(const DiString& paramName, PassInputType inputtype);

        void                SetInput(const DiString& paramName, float value);

        void                SetInput(const DiString& paramName, const DiVec3& value);

        void                SetInput(const DiString& paramName, const DiVec4& value);

        void                SetInputGBuffer(const DiString& paramName, int gbufferID);

        void                SetInputTex2D(const DiString& paramName, const DiString& texName);

        void                RefreshInput();

        void                SetBlendMode(DiBlendMode blend);

        DiShaderParameter*  GetShaderParam();

        DiTexturePtr        GetOutTexture() { return mTexture; }

        void                Process();

        bool                GetAutoClear() const;

        void                SetAutoClear(bool val);

        int                 GetQuadScale() const { return mQuadScale; }

        void                SetQuadScale(int val) { mQuadScale = val; }

        DiString            GetPassName() const { return mPassName; }

        void                SetInput(const DiString& paramName, const DiString& texturename);

        void                SetInput(const DiString& paramName, DiTexturePtr tex);

    protected:

        DiString            mPassName;
        
        DiTexturePtr        mTexture;

        DiMaterialPtr       mMaterial;
                            
        DiPostEffect*       mParent;

        DiRenderTarget*     mRenderTarget;

        int                 mQuadScale;
        
        bool                mAutoClear;

        typedef DiPair<DiString,PassInputType> InputParam;
        InputParam          mInput;
    };
}

#endif
