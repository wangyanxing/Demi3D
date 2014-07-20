
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

#ifndef LinearForceController_h__
#define LinearForceController_h__

#include "K2Prerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"
#include "BaseForceController.h"

namespace Demi
{
    class DEMI_K2_API DiLinearForceController : public DiBaseForceController
    {
    public:
        DiLinearForceController(void) : DiBaseForceController(){}

        virtual ~DiLinearForceController(void) {}

        virtual void CopyTo (DiParticleController* affector);

        virtual void PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiLinearForceControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiLinearForceControllerFactory(void) {}

        virtual                     ~DiLinearForceControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "LinearForce";
            return type;
        }

        DiParticleController*       CreateController(void)
        {
            return CreateControllerImpl<DiLinearForceController>();
        }
    };
}

#endif // LinearForceController_h__
