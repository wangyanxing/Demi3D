
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

#ifndef JetController_h__
#define JetController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiJetController : public DiParticleController
    {
    public:
        static const float            DEFAULT_ACCELERATION;

        DiJetController(void);

        virtual                        ~DiJetController(void);

    public:

        DiDynamicAttribute*         GetDynAcceleration(void) const {return mDynAcceleration;}

        void                        SetDynAcceleration(DiDynamicAttribute* dynAcceleration);

        virtual void                Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void                CopyTo (DiParticleController* affector);

    protected:

        float                        mScaled;

        DiDynamicAttribute*            mDynAcceleration;

        DiDynamicAttributeFactory    mDynamicAttributeFactory;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiJetControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiJetControllerFactory(void) {}

        virtual                        ~DiJetControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "Jet";
            return type;
        }

        DiParticleController*        CreateController(void)
        {
            return CreateControllerImpl<DiJetController>();
        }
    };
}

#endif // JetController_h__
