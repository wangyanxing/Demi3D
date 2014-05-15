
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
#include "GeometryRotatorController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const bool DiGeometryRotatorController::DEFAULT_USE_OWN            = false;
    const float DiGeometryRotatorController::DEFAULT_ROTATION_SPEED = 10.0f;
    const DiVec3 DiGeometryRotatorController::DEFAULT_ROTATION_AXIS = DiVec3::ZERO;

    DiGeometryRotatorController::DiGeometryRotatorController(void) : 
    DiParticleController(),
        mScaledRotationSpeed(0.0f),
        mUseOwnRotationSpeed(DEFAULT_USE_OWN),
        mQuat(DiQuat::IDENTITY),
        mRotationAxis(DEFAULT_ROTATION_AXIS),
        mRotationAxisSet(false)
    {
        mDynRotationSpeed = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynRotationSpeed))->SetValue(DEFAULT_ROTATION_SPEED);
    };
    
    DiGeometryRotatorController::~DiGeometryRotatorController(void)
    {
        SAFE_DELETE(mDynRotationSpeed);
    }
    
    const DiVec3& DiGeometryRotatorController::GetRotationAxis(void) const
    {
        return mRotationAxis;
    }
    
    void DiGeometryRotatorController::SetRotationAxis(const DiVec3& rotationAxis)
    {
        mRotationAxis = rotationAxis;
        mRotationAxisSet = true;
    }
    
    void DiGeometryRotatorController::ResetRotationAxis(void)
    {
        mDynRotationSpeed = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynRotationSpeed))->SetValue(DEFAULT_ROTATION_SPEED);
        mRotationAxisSet = false;
    }
    
    DiDynamicAttribute* DiGeometryRotatorController::GetRotationSpeed(void) const
    {
        return mDynRotationSpeed;
    }
    
    void DiGeometryRotatorController::SetRotationSpeed(DiDynamicAttribute* dynRotationSpeed)
    {
        SAFE_DELETE(mDynRotationSpeed);

        mDynRotationSpeed = dynRotationSpeed;
    }
    
    bool DiGeometryRotatorController::UseOwnRotationSpeed (void) const
    {
        return mUseOwnRotationSpeed;
    }
    
    void DiGeometryRotatorController::SetUseOwnRotationSpeed (bool useOwnRotationSpeed)
    {
        mUseOwnRotationSpeed = useOwnRotationSpeed;
    }
    
    float DiGeometryRotatorController::CalculateRotationSpeed(DiParticle* particle)
    {
        return mDynamicAttributeHelper.Calculate(mDynRotationSpeed, particle->timeFraction);
    }
    
    void DiGeometryRotatorController::InitParticleForEmission(DiParticle* particle)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        if (!mRotationAxisSet)
        {
            visualParticle->orientation.x = DiMath::RangeRandom(-1, 1);
            visualParticle->orientation.y = DiMath::RangeRandom(-1, 1);
            visualParticle->orientation.z = DiMath::RangeRandom(-1, 1);
            visualParticle->orientation.w = DiMath::RangeRandom(-1, 1);
            visualParticle->rotationAxis.x = DiMath::UnitRandom();
            visualParticle->rotationAxis.y = DiMath::UnitRandom();
            visualParticle->rotationAxis.z = DiMath::UnitRandom();
        }
        visualParticle->orientation.normalise();

        if (mUseOwnRotationSpeed)
        {
            visualParticle->rotationSpeed = CalculateRotationSpeed(particle);
        }
    }
    
    void DiGeometryRotatorController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
        {
            return;
        }

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);

        if (mUseOwnRotationSpeed)
        {
            mScaledRotationSpeed = visualParticle->rotationSpeed * timeElapsed;
        }
        else
        {
            mScaledRotationSpeed = CalculateRotationSpeed(particle) * timeElapsed;
        }

        mQuat = DiQuat::IDENTITY;
        if (mRotationAxisSet)
        {
            mQuat.FromAngleAxis(DiRadian(mScaledRotationSpeed), mRotationAxis);
        }
        else
        {
            mQuat.FromAngleAxis(DiRadian(mScaledRotationSpeed), visualParticle->rotationAxis);
        }

        visualParticle->orientation = mQuat * visualParticle->orientation;
    }
    
    void DiGeometryRotatorController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiGeometryRotatorController* geometryRotator = static_cast<DiGeometryRotatorController*>(affector);
        geometryRotator->SetRotationSpeed(mDynamicAttributeFactory.CloneDynamicAttribute(GetRotationSpeed()));
        geometryRotator->mUseOwnRotationSpeed = mUseOwnRotationSpeed;
        geometryRotator->mRotationAxis = mRotationAxis;
        geometryRotator->mRotationAxisSet = mRotationAxisSet;
    }
}

