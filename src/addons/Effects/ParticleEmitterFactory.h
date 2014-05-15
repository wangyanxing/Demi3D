
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

#ifndef ParticleEmitterFactory_h__
#define ParticleEmitterFactory_h__

#include "FxPrerequisites.h"
#include "ParticleEmitter.h"

namespace Demi
{
    class DEMI_FX_API DiParticleEmitterFactory
    {
    public:
        DiParticleEmitterFactory(void) {}

        virtual                        ~DiParticleEmitterFactory(void) {}

        virtual DiString            GetEmitterType(void) const = 0;

        virtual DiParticleEmitter*    CreateEmitter(void) = 0;

        void                        DestroyEmitter (DiParticleEmitter* emitter)
        {
            SAFE_DELETE(emitter);
        }

    protected:

        template <class T>
        DiParticleEmitter*            CreateEmitterImpl(void)
        {
            DiParticleEmitter* particleEmitter = DI_NEW T();
            particleEmitter->SetEmitterType(GetEmitterType());
            return particleEmitter;
        };
    };
}

#endif // ParticleEmitterFactory_h__
