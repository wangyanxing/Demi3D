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
