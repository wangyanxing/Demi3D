
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

#ifndef GravityController_h__
#define GravityController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiGravityController : public DiParticleController
    {
    public:
        static const float DEFAULT_GRAVITY;

        DiGravityController(void);

        virtual            ~DiGravityController(void) {}

    public:

        virtual void     CopyTo (DiParticleController* affector);

        virtual void     PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void     Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        float            GetGravity(void) const;

        void            SetGravity(float gravity);

    protected:
        float            mGravity;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiGravityControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiGravityControllerFactory(void) {}

        virtual                        ~DiGravityControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "Gravity";
            return type;
        }

        DiParticleController*        CreateController(void)
        {
            return CreateControllerImpl<DiGravityController>();
        }
    };
}

#endif // GravityController_h__
