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

#ifndef DiPostEffect_h__
#define DiPostEffect_h__


#include "Texture.h"
#include "Material.h"

namespace Demi
{
    class DI_GFX_API DiPostEffect : public DiBase
    {
    public:

        DiPostEffect(const DiString& name, DiPostEffectManager* mgr);

        virtual                 ~DiPostEffect();

        typedef DiVector<DiPostEffectPass*>    PassChain;

    public:

        DiPostEffectPass*       CreatePass(const DiString& name);

        void                    Process();

        DiTexturePtr            GetLastOutTexture();

        int                     GetPassID(const DiString& pass);

        DiPostEffectPass*       GetPass(const DiString& pass);

        DiPostEffectPass*       GetPrevValidPass(const DiString& pass);

        DiPostEffectPass*       GetLastValidPass();

        bool                    GetEnable() const { return mEnable; }

        void                    SetEnable(bool val);

        DiPostEffectManager*    GetPostEffectManager(){return mMgr;}

        void                    RefereshPasses();

    protected:

        DiString                mName;

        PassChain               mPasses;

        bool                    mEnable;

        DiPostEffectManager*    mMgr;
    };
}

#endif
