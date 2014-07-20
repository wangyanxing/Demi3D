
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef ParticleRendererFactory_h__
#define ParticleRendererFactory_h__

#include "K2Prerequisites.h"
#include "ParticleRenderer.h"

namespace Demi
{
    class DEMI_K2_API DiParticleRendererFactory
    {
    public:
        DiParticleRendererFactory(void){}

        virtual                        ~DiParticleRendererFactory(void){}

        virtual DiString            GetRendererType(void) const = 0;

        virtual DiParticleRenderer* CreateRenderer(void) = 0;

        void                        DestroyRenderer(DiParticleRenderer* renderer)
        {
            SAFE_DELETE(renderer);
        }

    protected:

        template <class T>
        DiParticleRenderer*            CreateRendererImpl(void)
        {
            DiParticleRenderer* particleRenderer = DI_NEW T();
            particleRenderer->SetRendererType(GetRendererType());
            return particleRenderer;
        }
    };
}

#endif // ParticleRendererFactory_h__
