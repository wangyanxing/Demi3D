
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

#ifndef EffectListeners_h__
#define EffectListeners_h__

#include "K2Prerequisites.h"

namespace Demi
{
    class DEMI_K2_API DiElementListener
    {
    public:
        DiElementListener(void) {};

        virtual ~DiElementListener(void) {};

        virtual void ParticleEmitted(DiParticleElement* particleTechnique, DiParticle* particle) = 0;

        virtual void ParticleExpired(DiParticleElement* particleTechnique, DiParticle* particle) = 0;
    };

    class DEMI_K2_API DiParticleSystemListener
    {
    public:

        DiParticleSystemListener(void) {};

        virtual ~DiParticleSystemListener(void) {};

        virtual void HandleParticleSystemEvent(DiParticleSystemPtr particleSystem, DiFxEvent& DiFxEvent) = 0;
    };
}
#endif // EffectListeners_h__

