
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
#include "CircleEmitter.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"

namespace Demi
{
    const float     DiCircleEmitter::DEFAULT_RADIUS = 100.0f;
    const float     DiCircleEmitter::DEFAULT_STEP = 0.1f;
    const float     DiCircleEmitter::DEFAULT_ANGLE = 0.0f;
    const bool        DiCircleEmitter::DEFAULT_RANDOM = true;
    const DiVec3    DiCircleEmitter::DEFAULT_NORMAL = DiVec3::ZERO;

    
    DiCircleEmitter::DiCircleEmitter(void) : 
        DiParticleEmitter(),
        mRadius(DEFAULT_RADIUS),
        mCircleAngle(DEFAULT_ANGLE),
        mOriginalCircleAngle(DEFAULT_ANGLE),
        mStep(DEFAULT_STEP),
        mRandom(DEFAULT_RANDOM),
        mOrientation(DiQuat::IDENTITY),
        mNormal(DEFAULT_NORMAL),
        mX(0.0f),
        mZ(0.0f)
    {
    }
    
    const float DiCircleEmitter::GetRadius(void) const
    {
        return mRadius;
    }
    
    void DiCircleEmitter::SetRadius(const float radius)
    {
        mRadius = radius;
    }
    
    const float DiCircleEmitter::GetCircleAngle(void) const
    {
        return mOriginalCircleAngle;
    }
    
    void DiCircleEmitter::SetCircleAngle(const float circleAngle)
    {
        mOriginalCircleAngle = circleAngle;
        mCircleAngle = circleAngle;
    }
    
    const float DiCircleEmitter::GetStep(void) const
    {
        return mStep;
    }
    
    void DiCircleEmitter::SetStep(const float step)
    {
        mStep = step;
    }
    
    const bool DiCircleEmitter::IsRandom(void) const
    {
        return mRandom;
    }
    
    void DiCircleEmitter::SetRandom(const bool random)
    {
        mRandom = random;
    }
    
    const DiQuat& DiCircleEmitter::GetOrientation(void) const
    {
        return mOrientation;
    }
     
    const DiVec3& DiCircleEmitter::GetNormal(void) const
    { 
        return mNormal;
    } 
     
    void DiCircleEmitter::SetNormal(const DiVec3& normal) 
    { 
        mOrientation = DiVec3::UNIT_Y.getRotationTo(normal, DiVec3::UNIT_X);
        mNormal = normal;
    }
    
    void DiCircleEmitter::NotifyStart (void)
    {
        mCircleAngle = mOriginalCircleAngle;
    }
     
    void DiCircleEmitter::InitParticlePosition(DiParticle* particle)
    {
        float angle = 0;
        if (mRandom)
        {
            angle = DiMath::RangeRandom(0, DiMath::TWO_PI);
        }
        else
        {
            mCircleAngle += mStep;
            mCircleAngle = mCircleAngle > DiMath::TWO_PI ? mCircleAngle - DiMath::TWO_PI : mCircleAngle;
            angle = mCircleAngle;
        }

        mX = DiMath::Cos(angle);
        mZ = DiMath::Sin(angle);
        DiParticleSystem* sys = mParentElement->GetParentSystem();
        if (sys)
        {
            particle->position = GetDerivedPosition() + 
                sys->GetDerivedOrientation() * mOrientation * (mEmitterScale * DiVec3(mX * mRadius, 0, mZ * mRadius));
        }
        else
        {
            particle->position = GetDerivedPosition() + mEmitterScale * ( mOrientation * DiVec3(mX * mRadius, 0, mZ * mRadius) );
        }
        particle->originalPosition = particle->position;
    }
    
    void DiCircleEmitter::InitParticleDirection(DiParticle* particle)
    {
        if (mAutoDirection)
        {
            DiRadian angle;
            GenerateAngle(angle);
            if (angle != DiRadian(0))
            {
                particle->direction = (mOrientation * DiVec3(mX, 0, mZ) ).randomDeviant(angle, mUpVector);
                particle->originalDirection = particle->direction;
            }
            else
            {
                particle->direction = DiVec3(mX, 0, mZ);
                particle->direction = mOrientation * DiVec3(mX, 0, mZ);
            }
        }
        else
        {
            DiParticleEmitter::InitParticleDirection(particle);
        }
    }
    
    void DiCircleEmitter::CopyTo (DiParticleEmitter* emitter)
    {
        DiParticleEmitter::CopyTo(emitter);

        DiCircleEmitter* circleEmitter = static_cast<DiCircleEmitter*>(emitter);
        circleEmitter->mRadius = mRadius;
        circleEmitter->mCircleAngle = mCircleAngle;
        circleEmitter->mOriginalCircleAngle = mOriginalCircleAngle;
        circleEmitter->mStep = mStep;
        circleEmitter->mRandom = mRandom;
        circleEmitter->mNormal = mNormal;
        circleEmitter->mOrientation = mOrientation; 
    }
}

