
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

#ifndef TextureRotatorController_h__
#define TextureRotatorController_h__

#include "K2Prerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"
#include "DynamicAttributeFactory.h"

namespace Demi
{
    class DEMI_K2_API DiTextureRotatorController : public DiParticleController
    {
    public:
        // Constants
        static const bool  DEFAULT_USE_OWN_SPEED;
        static const float DEFAULT_ROTATION_SPEED;
        static const float DEFAULT_ROTATION;

        DiTextureRotatorController(void);

        virtual             ~DiTextureRotatorController(void);

    public:
        DiRadian            CalculateRotation(void);

        /** Returns a rotation speed value, depending on the type of dynamic attribute.
        */
        DiRadian            CalculateRotationSpeed(DiParticle* particle);

        bool                UseOwnRotationSpeed(void) const { return mUseOwnRotationSpeed; }

        /** Set the indication whether the 2D rotation speed of the particle itself is used.
        */
        void                SetUseOwnRotationSpeed(bool useOwnRotationSpeed) { mUseOwnRotationSpeed = useOwnRotationSpeed; }

        /** Returns the rotation speed. This is the speed controlled by the affector.
        */
        DiDynamicAttribute* GetRotationSpeed(void) const  { return mDynRotationSpeed; }

        /**
        */
        void                SetRotationSpeed(DiDynamicAttribute* dynRotationSpeed);

        /** Returns the rotation defined in the the affector.
        */
        DiDynamicAttribute* GetRotation(void) const { return mDynRotation; }

        /**
        */
        void                SetRotation(DiDynamicAttribute* dynRotation);

        virtual void        Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void        CopyTo(DiParticleController* affector);

        virtual void        InitParticleForEmission(DiParticle* particle);

    protected:
     
        bool                        mUseOwnRotationSpeed;
        
        DiRadian                    mScaledRotationSpeed;

        DiDynamicAttribute*         mDynRotation;
        
        DiDynamicAttribute*         mDynRotationSpeed;

        DiDynamicAttributeFactory   mDynamicAttributeFactory;

        DiDynamicAttributeHelper    mDynamicAttributeHelper;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiTextureRotatorControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiTextureRotatorControllerFactory(void) {}

        virtual     ~DiTextureRotatorControllerFactory(void) {}

        DiString    GetControllerType(void) const
        {
            static DiString type = "TextureRotator";
            return type;
        }

        DiParticleController*   CreateController(void)
        {
            return CreateControllerImpl<DiTextureRotatorController>();
        }
    };
}

#endif // RandomiserController_h__
