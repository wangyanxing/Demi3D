
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

#include "K2Pch.h"
#include "TextureRotatorController.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"
#include "VisualParticle.h"
#include "ParticleEmitter.h"
#include "ParticleRenderer.h"

namespace Demi
{
    // Constants
    const bool DiTextureRotatorController::DEFAULT_USE_OWN_SPEED = false;
    const float DiTextureRotatorController::DEFAULT_ROTATION_SPEED = 10.0f;
    const float DiTextureRotatorController::DEFAULT_ROTATION = 0.0f;

    DiTextureRotatorController::DiTextureRotatorController(void) :
        DiParticleController(),
        mUseOwnRotationSpeed(DEFAULT_USE_OWN_SPEED),
        mScaledRotationSpeed(DiRadian(0))
    {
        mDynRotation = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynRotation))->SetValue(DEFAULT_ROTATION);
        mDynRotationSpeed = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynRotationSpeed))->SetValue(DEFAULT_ROTATION_SPEED);
    }

    void DiTextureRotatorController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        // Only continue if the particle is a visual particle
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        if (mUseOwnRotationSpeed)
        {
            // Use scaled rotation speed and adjust the speed according to the velocity
            mScaledRotationSpeed = visualParticle->zRotationSpeed * timeElapsed;
        }
        else 
        {
            // Scale speed
            mScaledRotationSpeed = CalculateRotationSpeed(particle) * timeElapsed;
        }

        visualParticle->zRotation += mScaledRotationSpeed;
        visualParticle->zRotation = visualParticle->zRotation > DiRadian(DiMath::TWO_PI) ?
            visualParticle->zRotation - DiRadian(DiMath::TWO_PI) : visualParticle->zRotation;
        if (particleTechnique->GetRenderer())
        {
            particleTechnique->GetRenderer()->NotifyParticleZRotated();
        }
    }
    
    void DiTextureRotatorController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiTextureRotatorController* textureRotator = static_cast<DiTextureRotatorController*>(affector);
        textureRotator->SetRotation(mDynamicAttributeFactory.CloneDynamicAttribute(GetRotation()));
        textureRotator->SetRotationSpeed(mDynamicAttributeFactory.CloneDynamicAttribute(GetRotationSpeed()));
        textureRotator->mUseOwnRotationSpeed = mUseOwnRotationSpeed;
    }

    DiTextureRotatorController::~DiTextureRotatorController(void)
    {
        SAFE_DELETE(mDynRotationSpeed);
        SAFE_DELETE(mDynRotation);
    }

    void DiTextureRotatorController::SetRotation(DiDynamicAttribute* dynRotation)
    {
        SAFE_DELETE(mDynRotation);
        mDynRotation = dynRotation;
    }

    void DiTextureRotatorController::SetRotationSpeed(DiDynamicAttribute* dynRotationSpeed)
    {
        SAFE_DELETE(mDynRotationSpeed);
        mDynRotationSpeed = dynRotationSpeed;
    }

    DiRadian DiTextureRotatorController::CalculateRotation(void)
    {
        return DiRadian(DiDegree(mDynamicAttributeHelper.Calculate(mDynRotation,
            mParentElement->GetParentSystem()->GetTimeElapsedSinceStart())));
    }

    DiRadian DiTextureRotatorController::CalculateRotationSpeed(DiParticle* particle)
    {
        return DiRadian(DiDegree(mDynamicAttributeHelper.Calculate(mDynRotationSpeed, particle->timeFraction)));
    }

    void DiTextureRotatorController::InitParticleForEmission(DiParticle* particle)
    {
        // Only continue if the particle is a visual particle
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        // Set initial random zRotation
        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        visualParticle->zRotation = CalculateRotation();

        if (mUseOwnRotationSpeed)
        {
            visualParticle->zRotationSpeed = DiRadian(DiDegree(mDynamicAttributeHelper.Calculate(mDynRotationSpeed, 0)));
        }

        if (particle->parentEmitter->GetParentElement()->GetRenderer())
        {
            // Assume that all parents exist. That must be the case otherwise particles are not emitted.
            particle->parentEmitter->GetParentElement()->GetRenderer()->NotifyParticleZRotated();
        }

        // Set the zRotationSpeed
        visualParticle->zRotationSpeed = CalculateRotationSpeed(particle);
    }
}

