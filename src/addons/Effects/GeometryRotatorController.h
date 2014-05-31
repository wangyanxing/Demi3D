
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


#ifndef GeometryRotatorController_h__
#define GeometryRotatorController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiGeometryRotatorController : public DiParticleController
    {
    public:
        static const bool            DEFAULT_USE_OWN;
        static const float           DEFAULT_ROTATION_SPEED;
        static const DiVec3          DEFAULT_ROTATION_AXIS;

        DiGeometryRotatorController(void);

        virtual                      ~DiGeometryRotatorController(void);

    public:

        DiDynamicAttribute*          GetRotationSpeed(void) const;

        void                         SetRotationSpeed(DiDynamicAttribute* dynRotationSpeed);

        bool                         UseOwnRotationSpeed (void) const;

        void                         SetUseOwnRotationSpeed (bool useOwnRotationSpeed);

        const DiVec3&                GetRotationAxis(void) const;

        void                         SetRotationAxis(const DiVec3& rotationAxis);

        void                         ResetRotationAxis(void);

        float                        CalculateRotationSpeed (DiParticle* particle);

        virtual void                 CopyTo (DiParticleController* affector);

        virtual void                 InitParticleForEmission(DiParticle* particle);

        virtual void                 Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

    protected:
        float                        mScaledRotationSpeed;

        bool                         mUseOwnRotationSpeed;
        
        DiDynamicAttribute*          mDynRotationSpeed;
        
        DiQuat                       mQuat;
        
        DiVec3                       mRotationAxis;
        
        bool                         mRotationAxisSet;

        DiDynamicAttributeFactory    mDynamicAttributeFactory;
        
        DiDynamicAttributeHelper     mDynamicAttributeHelper;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiGeometryRotatorControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiGeometryRotatorControllerFactory(void) {}

        virtual                     ~DiGeometryRotatorControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "GeometryRotator";
            return type;
        }

        DiParticleController*       CreateController(void)
        {
            return CreateControllerImpl<DiGeometryRotatorController>();
        }
    };
}

#endif // GeometryRotatorController_h__
