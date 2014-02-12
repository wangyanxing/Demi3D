


#ifndef DiGBuffer_h__
#define DiGBuffer_h__


#include "Texture.h"
#include "Material.h"

namespace Demi 
{

    class DiGBuffer : public DiBase
    {
    public:
        DiGBuffer(DiRenderWindow* renderWnd);

        ~DiGBuffer(void);

    public:

        void            Create();

        void            Bind();

        DiTexturePtr    GetRTTexture(int id){ return mTexture[id]; }

        DiMaterialPtr   GetMaterial(){ return mMaterial; }

    protected:
        
        typedef DiVector<DiTexturePtr>    TexBuffer;
        typedef DiVector<DiRenderTarget*> RTBuffer;

        TexBuffer        mTexture;

        RTBuffer         mRenderTargets;

        DiMaterialPtr    mMaterial;

        DiRenderWindow*  mParentWnd;
    };

}

#endif
