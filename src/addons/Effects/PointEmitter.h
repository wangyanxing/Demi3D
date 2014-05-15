
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

#ifndef PointEmitter_h__
#define PointEmitter_h__

#include "FxPrerequisites.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterFactory.h"

namespace Demi
{
    class DEMI_FX_API DiPointEmitter : public DiParticleEmitter
    {
    public:
        DiPointEmitter(void) : DiParticleEmitter()
        {
        }

        virtual            ~DiPointEmitter(void) {}

        virtual void    CopyTo (DiPointEmitter* emitter);
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiPointEmitterFactory : public DiParticleEmitterFactory
    {
    public:
        DiPointEmitterFactory(void) {}

        virtual ~DiPointEmitterFactory(void) {}

        DiString            GetEmitterType(void) const
        {
            static DiString type = "Point";
            return type;
        }

        DiParticleEmitter*    CreateEmitter(void)
        {
            return CreateEmitterImpl<DiPointEmitter>();
        }
    };
}

#endif // PointEmitter_h__
