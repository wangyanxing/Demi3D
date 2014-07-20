
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

#ifndef VortexController_h__
#define VortexController_h__

#include "K2Prerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_K2_API DiVortexController : public DiParticleController
    {
    public:
        static const DiVec3         DEFAULT_ROTATION_VECTOR;

        static const float          DEFAULT_ROTATION_SPEED;

    public:

        DiVortexController(void);

        virtual                     ~DiVortexController(void);

    public:

        const DiVec3&               GetRotationVector(void) const;

        void                        SetRotationVector(const DiVec3& rotationVector);

        DiDynamicAttribute*         GetRotationSpeed(void) const;

        void                        SetRotationSpeed(DiDynamicAttribute* dynRotationSpeed);

        virtual void                CopyTo (DiParticleController* affector);

        virtual void                PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void                Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

    protected:

        DiRadian                    CalculateRotationSpeed(void);

    protected:

        DiVec3                      mRotationVector;

        DiQuat                      mRotation;
        
        DiDynamicAttribute*         mDynRotationSpeed;
        
        DiDynamicAttributeHelper    mDynamicAttributeHelper;
        
        DiDynamicAttributeFactory   mDynamicAttributeFactory;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiVortexControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiVortexControllerFactory(void) {}

        virtual                     ~DiVortexControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "Vortex";
            return type;
        }

        DiParticleController*       CreateController(void)
        {
            return CreateControllerImpl<DiVortexController>();
        }
    };
}

#endif // VortexController_h__
