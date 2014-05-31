
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

#ifndef SineForceController_h__
#define SineForceController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"
#include "BaseForceController.h"

namespace Demi
{
    class DEMI_FX_API DiSineForceController : public DiBaseForceController
    {
    public:
        static const float DEFAULT_FREQ_MIN;
        static const float DEFAULT_FREQ_MAX;

        DiSineForceController(void);

        virtual          ~DiSineForceController(void) {}

    public:

        virtual void     CopyTo (DiParticleController* affector);

        virtual void     PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void     Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        const float      GetFrequencyMin(void) const;

        void             SetFrequencyMin(const float frequencyMin);

        const float      GetFrequencyMax(void) const;

        void             SetFrequencyMax(const float frequencyMax);

    protected:
        float            mAngle;

        float            mFrequencyMin;

        float            mFrequencyMax;

        float            mFrequency;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiSineForceControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiSineForceControllerFactory(void) {}

        virtual                 ~DiSineForceControllerFactory(void) {}

        DiString                GetControllerType(void) const
        {
            static DiString type = "SineForce";
            return type;
        }

        DiParticleController*   CreateController(void)
        {
            return CreateControllerImpl<DiSineForceController>();
        }
    };
}

#endif // SineForceController_h__
