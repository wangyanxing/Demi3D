
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

#include "FxPch.h"
#include "VortexController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"
#include "ParticleSystem.h"

namespace Demi
{
    const DiVec3 DiVortexController::DEFAULT_ROTATION_VECTOR = DiVec3::ZERO;
    const float DiVortexController::DEFAULT_ROTATION_SPEED = 1.0f;

    
    DiVortexController::DiVortexController(void) : 
        DiParticleController(),
        mRotation(DiQuat::IDENTITY),
        mRotationVector(DEFAULT_ROTATION_VECTOR)
    {
        mDynRotationSpeed = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynRotationSpeed))->SetValue(DEFAULT_ROTATION_SPEED);
    }
    
    DiVortexController::~DiVortexController(void)
    {
        SAFE_DELETE(mDynRotationSpeed);
    }
    
    const DiVec3& DiVortexController::GetRotationVector(void) const
    {
        return mRotationVector;
    }
    
    void DiVortexController::SetRotationVector(const DiVec3& rotationVector)
    {
        mRotationVector = rotationVector;
    }
    
    DiDynamicAttribute* DiVortexController::GetRotationSpeed(void) const
    {
        return mDynRotationSpeed;
    }
    
    void DiVortexController::SetRotationSpeed(DiDynamicAttribute* dynRotationSpeed)
    {
        SAFE_DELETE(mDynRotationSpeed);

        mDynRotationSpeed = dynRotationSpeed;
    }
    
    void DiVortexController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        DiParticleSystem* sys = mParentElement->GetParentSystem();
        
        if (sys)
            mRotation.FromAngleAxis(DiRadian(CalculateRotationSpeed() * timeElapsed), sys->GetDerivedOrientation() * mRotationVector);
        else
            mRotation.FromAngleAxis(DiRadian(CalculateRotationSpeed() * timeElapsed), mRotationVector);

        GetDerivedPosition();
    }
    
    DiRadian DiVortexController::CalculateRotationSpeed(void)
    {
        return DiRadian(mDynamicAttributeHelper.Calculate(mDynRotationSpeed, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart()));
    }
    
    void DiVortexController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (particle->IsFreezed())
            return;

        DiVec3 local = particle->position - mDerivedPosition;
        particle->position = mDerivedPosition + mRotation * local;
        particle->direction = mRotation * particle->direction;

        if (particle->particleType == DiParticle::PT_VISUAL)
        {
            DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
            visualParticle->orientation = mRotation * visualParticle->orientation;
        }
    }
    
    void DiVortexController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiVortexController* vortexAffector = static_cast<DiVortexController*>(affector);
        vortexAffector->mRotation = mRotation;
        vortexAffector->mRotationVector = mRotationVector;
        vortexAffector->SetRotationSpeed(mDynamicAttributeFactory.CloneDynamicAttribute(GetRotationSpeed()));
    }
}

