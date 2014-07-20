
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
#include "ParticleEmitter.h"
#include "DynamicAttributeFactory.h"
#include "VisualParticle.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"

namespace Demi
{
    const bool      DiParticleEmitter::DEFAULT_ENABLED = true;
    const DiVec3    DiParticleEmitter::DEFAULT_POSITION = DiVec3::ZERO;
    const bool      DiParticleEmitter::DEFAULT_KEEP_LOCAL = false;
    const DiVec3    DiParticleEmitter::DEFAULT_DIRECTION = DiVec3::UNIT_Y;
    const DiQuat    DiParticleEmitter::DEFAULT_ORIENTATION = DiQuat::IDENTITY;
    const DiQuat    DiParticleEmitter::DEFAULT_ORIENTATION_RANGE_START = DiQuat::IDENTITY;
    const DiQuat    DiParticleEmitter::DEFAULT_ORIENTATION_RANGE_END = DiQuat::IDENTITY;
    const DiParticle::ParticleType DiParticleEmitter::DEFAULT_EMITS = DiVisualParticle::PT_VISUAL;
    const uint16    DiParticleEmitter::DEFAULT_START_TEXTURE_COORDS = 0;
    const uint16    DiParticleEmitter::DEFAULT_END_TEXTURE_COORDS = 0;
    const uint16    DiParticleEmitter::DEFAULT_TEXTURE_COORDS = 0;
    const DiColor   DiParticleEmitter::DEFAULT_START_COLOUR_RANGE = DiColor::Black;
    const DiColor   DiParticleEmitter::DEFAULT_END_COLOUR_RANGE = DiColor::White;
    const DiColor   DiParticleEmitter::DEFAULT_COLOUR = DiColor::White;
    const bool      DiParticleEmitter::DEFAULT_AUTO_DIRECTION = false;
    const bool      DiParticleEmitter::DEFAULT_FORCE_EMISSION = false;
    const float     DiParticleEmitter::DEFAULT_EMISSION_RATE = 10.0f;
    const float     DiParticleEmitter::DEFAULT_TIME_TO_LIVE = 3.0f;
    const float     DiParticleEmitter::DEFAULT_MASS = 1.0f;
    const float     DiParticleEmitter::DEFAULT_VELOCITY = 100.0f;
    const float     DiParticleEmitter::DEFAULT_DURATION = 0.0f;
    const float     DiParticleEmitter::DEFAULT_REPEAT_DELAY = 0.0f;
    const float     DiParticleEmitter::DEFAULT_ANGLE = 20.0f;
    const float     DiParticleEmitter::DEFAULT_DIMENSIONS = 0.0f;
    const float     DiParticleEmitter::DEFAULT_WIDTH = 0.0f;
    const float     DiParticleEmitter::DEFAULT_HEIGHT = 0.0f;
    const float     DiParticleEmitter::DEFAULT_DEPTH = 0.0f;

    DiParticleEmitter::DiParticleEmitter(void) :
        mParentElement(0),
        mRemainder(0),
        mDurationRemain(0),
        mRepeatDelayRemain(0),
        mDynDurationSet(false),
        mDynRepeatDelaySet(false),
        mParticleDirection(DEFAULT_DIRECTION),
        mOriginalParticleDirection(DEFAULT_DIRECTION),
        mParticleOrientation(DiQuat::IDENTITY),
        mUpVector(DiVec3::ZERO),
        mDynParticleAllDimensionsSet(false),
        mDynParticleWidthSet(false),
        mDynParticleHeightSet(false),
        mDynParticleDepthSet(false),
        mAutoDirection(DEFAULT_AUTO_DIRECTION),
        mForceEmission(DEFAULT_FORCE_EMISSION),
        mOriginalForceEmission(false),
        mForceEmissionExecuted(false),
        mOriginalForceEmissionExecuted(false),
        mName(DiString::BLANK),
        mEmitterScale(DiVec3::UNIT_SCALE),
        mParticleColour(DEFAULT_COLOUR),
        mKeepLocal(false),
        mParticleTextureCoords(DEFAULT_TEXTURE_COORDS),
        mEmitsType(DEFAULT_EMITS),
        mEmitsName(DiString::BLANK)
    {
        mDynEmissionRate = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynEmissionRate))->SetValue(DEFAULT_EMISSION_RATE);

        mDynTotalTimeToLive = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynTotalTimeToLive))->SetValue(DEFAULT_TIME_TO_LIVE);

        mDynParticleMass = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynParticleMass))->SetValue(DEFAULT_MASS);

        mDynVelocity = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynVelocity))->SetValue(DEFAULT_VELOCITY);

        mDynDuration = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynDuration))->SetValue(DEFAULT_DURATION);

        mDynRepeatDelay = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynRepeatDelay))->SetValue(DEFAULT_REPEAT_DELAY);

        mDynAngle = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynAngle))->SetValue(DEFAULT_ANGLE);

        mDynParticleAllDimensions = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynParticleAllDimensions))->SetValue(DEFAULT_DIMENSIONS);

        mDynParticleWidth = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynParticleWidth))->SetValue(DEFAULT_WIDTH);

        mDynParticleHeight = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynParticleHeight))->SetValue(DEFAULT_HEIGHT);

        mDynParticleDepth = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynParticleDepth))->SetValue(DEFAULT_DEPTH);
    }
    
    DiParticleEmitter::~DiParticleEmitter(void)
    {
        if (mDynEmissionRate)
        {
            DI_DELETE mDynEmissionRate;
        }

        if (mDynTotalTimeToLive)
        {
            DI_DELETE mDynTotalTimeToLive;
        }

        if (mDynParticleMass)
        {
            DI_DELETE mDynParticleMass;
        }

        if (mDynVelocity)
        {
            DI_DELETE mDynVelocity;
        }

        if (mDynDuration)
        {
            DI_DELETE mDynDuration;
        }

        if (mDynRepeatDelay)
        {
            DI_DELETE mDynRepeatDelay;
        }

        if (mDynParticleAllDimensions)
        {
            DI_DELETE mDynParticleAllDimensions;
        }

        if (mDynParticleWidth)
        {
            DI_DELETE mDynParticleWidth;
        }

        if (mDynParticleHeight)
        {
            DI_DELETE mDynParticleHeight;
        }

        if (mDynParticleDepth)
        {
            DI_DELETE mDynParticleDepth;
        }

        if (mDynAngle)
        {
            DI_DELETE mDynAngle;
        }
    }
    
    bool DiParticleEmitter::IsKeepLocal(void) const
    {
        return mKeepLocal;
    }
    
    void DiParticleEmitter::SetKeepLocal(bool keepLocal)
    {
        mKeepLocal = keepLocal;
    }
    
    bool DiParticleEmitter::MakeParticleLocal(DiParticle* particle)
    {
        if (!particle)
            return true;

        if (!mKeepLocal)
            return false;

        DiVec3 diff = GetDerivedPosition() - GetDerivedPosition()/*latestPosition*/;
        particle->position += diff;
        return true;
    }
    
    void DiParticleEmitter::NotifyRescaled(const DiVec3& scale)
    {
        mEmitterScale = scale;
    }
    
    const DiColor& DiParticleEmitter::GetParticleColour(void) const
    {
        return mParticleColour;
    }
    
    void DiParticleEmitter::SetParticleColour(const DiColor& particleColour)
    {
        mParticleColour = particleColour;
    }
    
    const uint16& DiParticleEmitter::GetParticleTextureCoords(void) const
    {
        return mParticleTextureCoords;
    }
    
    void DiParticleEmitter::SetParticleTextureCoords(const uint16& particleTextureCoords)
    {
        mParticleTextureCoords = particleTextureCoords;
    }

    void DiParticleEmitter::CopyTo( DiParticleEmitter* emitter )
    {
        CopyParentTo(emitter);
        emitter->SetEmitterType(mEmitterType);
    }
    
    void DiParticleEmitter::CopyParentTo(DiParticleEmitter* emitter)
    {
        emitter->SetName(mName);
        emitter->mParticleDirection             = mParticleDirection;
        emitter->mOriginalParticleDirection     = mOriginalParticleDirection;
        emitter->mParticleOrientation           = mParticleOrientation;
        emitter->mParentElement                 = mParentElement;
        emitter->mAutoDirection                 = mAutoDirection;
        emitter->mForceEmission                 = mForceEmission;
        emitter->mOriginalForceEmission         = mOriginalForceEmission;
        emitter->mForceEmissionExecuted         = mForceEmissionExecuted;
        emitter->mOriginalForceEmissionExecuted = mOriginalForceEmissionExecuted;
        emitter->mDynDurationSet                = mDynDurationSet;
        emitter->mDynRepeatDelaySet             = mDynRepeatDelaySet;
        emitter->mDynParticleAllDimensionsSet   = mDynParticleAllDimensionsSet;
        emitter->mDynParticleWidthSet           = mDynParticleWidthSet;
        emitter->mDynParticleHeightSet          = mDynParticleHeightSet;
        emitter->mDynParticleDepthSet           = mDynParticleDepthSet;
        emitter->mEmitterScale                  = mEmitterScale;
        emitter->mParticleColour                = mParticleColour;
        emitter->mParticleTextureCoords         = mParticleTextureCoords;
        emitter->mKeepLocal                     = mKeepLocal;

        emitter->SetDynEmissionRate(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynEmissionRate()));
        emitter->SetDynTotalTimeToLive(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynTotalTimeToLive()));
        emitter->SetDynVelocity(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynVelocity()));

        if (mDynDurationSet)
            emitter->SetDynDuration(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynDuration()));

        if (mDynRepeatDelaySet)
            emitter->SetDynRepeatDelay(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynRepeatDelay()));

        emitter->SetDynParticleMass(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynParticleMass()));
        emitter->SetDynAngle(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynAngle()));

        if (mDynParticleAllDimensionsSet)
            emitter->SetDynParticleAllDimensions(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynParticleAllDimensions()));
        
        if (mDynParticleWidthSet)
            emitter->SetDynParticleWidth(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynParticleWidth()));
        
        if (mDynParticleHeightSet)
            emitter->SetDynParticleHeight(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynParticleHeight()));
        
        if (mDynParticleDepthSet)
            emitter->SetDynParticleDepth(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynParticleDepth()));
    }
    
    void DiParticleEmitter::SetParentElement(DiParticleElement* parentTechnique)
    {
        mParentElement = parentTechnique;
    }

    void DiParticleEmitter::SetDynEmissionRate(DiDynamicAttribute* dynEmissionRate)
    {
        if (mDynEmissionRate)
        {
            DI_DELETE mDynEmissionRate;
        }

        mDynEmissionRate = dynEmissionRate;
    }
    
    void DiParticleEmitter::SetDynTotalTimeToLive(DiDynamicAttribute* dynTotalTimeToLive)
    {
        if (mDynTotalTimeToLive)
        {
            DI_DELETE mDynTotalTimeToLive;
        }

        mDynTotalTimeToLive = dynTotalTimeToLive;
    }
    
    void DiParticleEmitter::SetDynParticleMass(DiDynamicAttribute* dynParticleMass)
    {
        if (mDynParticleMass)
        {
            DI_DELETE mDynParticleMass;
        }

        mDynParticleMass = dynParticleMass;
    }
    
    void DiParticleEmitter::SetDynAngle(DiDynamicAttribute* dynAngle)
    {
        if (mDynAngle)
        {
            DI_DELETE mDynAngle;
        }

        mDynAngle = dynAngle;
    }
    
    void DiParticleEmitter::SetDynVelocity(DiDynamicAttribute* dynVelocity)
    {
        if (mDynVelocity)
        {
            DI_DELETE mDynVelocity;
        }

        mDynVelocity = dynVelocity;
    }
    
    void DiParticleEmitter::SetDynDuration(DiDynamicAttribute* dynDuration)
    {
        if (mDynDuration)
        {
            DI_DELETE mDynDuration;
        }

        mDynDuration = dynDuration;
        mDynDurationSet = true;
        InitTimeBased();
    }
    
    void DiParticleEmitter::SetDynDurationSet(bool durationSet)
    {
        mDynDurationSet = durationSet;
    }
    
    void DiParticleEmitter::SetDynRepeatDelay(DiDynamicAttribute* dynRepeatDelay)
    {
        if (mDynRepeatDelay)
        {
            DI_DELETE mDynRepeatDelay;
        }

        mDynRepeatDelay = dynRepeatDelay;
        mDynRepeatDelaySet = true;
        InitTimeBased();
    }
    
    void DiParticleEmitter::SetDynRepeatDelaySet(bool repeatDelaySet)
    {
        mDynRepeatDelaySet = repeatDelaySet;
    }
    
    void DiParticleEmitter::SetDynParticleAllDimensions(DiDynamicAttribute* dynParticleAllDimensions)
    {
        if (mDynParticleAllDimensions)
        {
            DI_DELETE mDynParticleAllDimensions;
        }
        mDynParticleAllDimensions = dynParticleAllDimensions;
        mDynParticleAllDimensionsSet = true;
    }
    
    void DiParticleEmitter::SetDynParticleAllDimensionsSet(bool particleAllDimensionsSet)
    {
        mDynParticleAllDimensionsSet = particleAllDimensionsSet;
    }
    
    void DiParticleEmitter::SetDynParticleWidth(DiDynamicAttribute* dynParticleWidth)
    {
        if (mDynParticleWidth)
        {
            DI_DELETE mDynParticleWidth;
        }

        mDynParticleWidth = dynParticleWidth;
        mDynParticleWidthSet = true;
    }
    
    void DiParticleEmitter::SetDynParticleWidthSet(bool particleWidthSet)
    {
        mDynParticleWidthSet = particleWidthSet;
    }
    
    void DiParticleEmitter::SetDynParticleHeight(DiDynamicAttribute* dynParticleHeight)
    {
        if (mDynParticleHeight)
        {
            DI_DELETE mDynParticleHeight;
        }

        mDynParticleHeight = dynParticleHeight;
        mDynParticleHeightSet = true;
    }
    
    void DiParticleEmitter::SetDynParticleHeightSet(bool particleHeightSet)
    {
        mDynParticleHeightSet = particleHeightSet;
    }
    
    void DiParticleEmitter::SetDynParticleDepth(DiDynamicAttribute* dynParticleDepth)
    {
        if (mDynParticleDepth)
        {
            DI_DELETE mDynParticleDepth;
        }

        mDynParticleDepth = dynParticleDepth;
        mDynParticleDepthSet = true;
    }
    
    void DiParticleEmitter::SetDynParticleDepthSet(bool particleDepthSet)
    {
        mDynParticleDepthSet = particleDepthSet;
    }
    
    void DiParticleEmitter::SetEnabled(bool enabled)
    {
        DiParticle::SetEnabled(enabled);
        InitTimeBased();
    }
    
    const DiVec3& DiParticleEmitter::GetParticleDirection(void) const
    {
        return mParticleDirection;
    }
    
    void DiParticleEmitter::SetParticleDirection(const DiVec3& direction)
    {
        mParticleDirection = direction;
        mOriginalParticleDirection = direction;
        mParticleDirection.normalise();
        mUpVector = mParticleDirection.perpendicular();
        mUpVector.normalise();
    }
    
    const DiVec3& DiParticleEmitter::GetOriginalParticleDirection(void) const
    {
        return mOriginalParticleDirection;
    }
    
    const DiQuat& DiParticleEmitter::GetParticleOrientation(void) const
    {
        return mParticleOrientation;
    }
    
    void DiParticleEmitter::SetParticleOrientation(const DiQuat& orientation)
    {
        mParticleOrientation = orientation;
    }
    
    bool DiParticleEmitter::IsAutoDirection(void) const
    {
        return mAutoDirection;
    }
    
    void DiParticleEmitter::SetAutoDirection(bool autoDirection)
    {
        mAutoDirection = autoDirection;
    }
    
    bool DiParticleEmitter::IsForceEmission(void) const
    {
        return mForceEmission;
    }
    
    void DiParticleEmitter::SetForceEmission(bool forceEmission)
    {
        mForceEmission = forceEmission;
        mOriginalForceEmission = forceEmission;
        mForceEmissionExecuted = false;
        mOriginalForceEmissionExecuted = false;
    }
    
    void DiParticleEmitter::InitTimeBased(void)
    {
        if (mEnabled)
        {
            if (mDynDurationSet && mParentElement)
            {
                mDurationRemain = mDynamicAttributeHelper.Calculate(
                    mDynDuration, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
            }

            if (mDurationRemain > 0)
            {
                mEnabled = true;
                mRepeatDelayRemain = 0;
            }
        }
        else
        {
            if (mDynRepeatDelaySet && mParentElement)
            {
                mRepeatDelayRemain = mDynamicAttributeHelper.Calculate(mDynRepeatDelay, 
                    mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
            }
        }
    }

    DiVec3 DiParticleEmitter::GetDerivedPosition() 
    {
        if (mMarkedForEmission)
        {
            // Use the emitter position, because it is emitted
            // If a particle is emitted, position and derived position are the same
            mDerivedPosition = position;
        }
        else
        {
            // Add the techniques' derived position. Use the emitters' own 'position' as offset.
            mDerivedPosition = mParentElement->GetDerivedPosition() + 
                mParentElement->GetParentSystem()->GetDerivedOrientation() *
                (mEmitterScale * position);
        }
        return mDerivedPosition;
    }

    void DiParticleEmitter::InitParticlePosition(DiParticle* particle)
    {
        particle->position         = GetDerivedPosition();
        particle->originalPosition = particle->position;
        particle->latestPosition   = particle->position;
    }
    
    void DiParticleEmitter::InitParticleForEmission(DiParticle* particle)
    {
        particle->parentEmitter = this;
        InitParticlePosition(particle);
        InitParticleDirection(particle);
        InitParticleVelocity(particle);
        InitParticleOrientation(particle);
        InitParticleMass(particle);
        InitParticleColour(particle);
        InitParticleTextureCoords(particle);
        particle->totalTimeToLive = InitParticleTimeToLive();
        particle->timeToLive = particle->totalTimeToLive;
        
        InitParticleDimensions(particle);
    }
    
    void DiParticleEmitter::InitParticleDirection(DiParticle* particle)
    {
        DiRadian angle;
        GenerateAngle(angle);
        if (angle != DiRadian(0))
            particle->direction = mParticleDirection.randomDeviant(angle, mUpVector);
        else
            particle->direction = mParticleDirection;
        
        particle->originalDirection = particle->direction;
        particle->originalDirectionLength = particle->direction.length();
    }
    
    void DiParticleEmitter::GenerateAngle(DiRadian& angle)
    {
        DiRadian a = DiAngle(mDynamicAttributeHelper.Calculate(mDynAngle, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart()));
        angle = a;
        
        if (mDynAngle->GetType() == DiDynamicAttribute::DAT_FIXED)
            angle = DiMath::UnitRandom() * angle;
    }
    
    void DiParticleEmitter::InitParticleVelocity(DiParticle* particle)
    {
        float scalar = mDynamicAttributeHelper.Calculate(mDynVelocity, mParentElement->GetParentSystem(
            )->GetTimeElapsedSinceStart(), 1.0f);
        particle->direction *= scalar;
        particle->originalVelocity = scalar;
        particle->originalScaledDirectionLength = particle->direction.length();
    }
    
    void DiParticleEmitter::InitParticleOrientation(DiParticle* particle)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        visualParticle->orientation = mParticleOrientation;
        visualParticle->originalOrientation = visualParticle->orientation;
    }
    
    void DiParticleEmitter::InitParticleMass(DiParticle* particle)
    {
        float mass = mDynamicAttributeHelper.Calculate(mDynParticleMass, mParentElement->GetParentSystem(
            )->GetTimeElapsedSinceStart(), DiParticle::DEFAULT_MASS);
        particle->mass = mass;
    }
    
    void DiParticleEmitter::InitParticleColour(DiParticle* particle)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        visualParticle->colour = mParticleColour;

        visualParticle->originalColour = visualParticle->colour;
    }
    
    void DiParticleEmitter::InitParticleTextureCoords(DiParticle* particle)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        visualParticle->textureCoordsCurrent = mParticleTextureCoords;
    }
    
    float DiParticleEmitter::InitParticleTimeToLive(void)
    {
        return mDynamicAttributeHelper.Calculate(mDynTotalTimeToLive, mParentElement->GetParentSystem(
            )->GetTimeElapsedSinceStart(), DiParticle::DEFAULT_TTL);
    }
    
    unsigned short DiParticleEmitter::CalculateRequestedParticles(float timeElapsed)
    {
        unsigned short requestedParticles = 0;
        
        if (mEnabled)
        {
            if (mDynEmissionRate && mParentElement && mParentElement->GetParentSystem())
            {
                float rate = mDynEmissionRate->GetValue(
                                mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
                
                if (mForceEmission)
                {
                    if (mForceEmissionExecuted)
                    {
                        requestedParticles = 0;
                    }
                    else
                    {
                        requestedParticles = (unsigned short)rate;
                        mForceEmissionExecuted = true;
                    }
                }
                else
                {
                    mRemainder += rate * timeElapsed;
                    requestedParticles = (unsigned short)mRemainder;
                }
                
                mRemainder -= requestedParticles;
            }

            if (mDynDurationSet)
            {
                mDurationRemain -= timeElapsed;
                if (mDurationRemain <= 0)
                    SetEnabled(false);
            }
        }
        else if (mDynRepeatDelaySet)
        {
            mRepeatDelayRemain -= timeElapsed;
            if (mRepeatDelayRemain <= 0)
                SetEnabled(true);
        }

        return requestedParticles;
    }
    
    void DiParticleEmitter::InitParticleDimensions(DiParticle* particle)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        if (mDynParticleAllDimensionsSet || mDynParticleWidthSet || mDynParticleHeightSet || mDynParticleDepthSet)
        {
            float extend = 0;
            if (mDynParticleAllDimensionsSet && mDynParticleAllDimensions)
            {
                extend = mDynamicAttributeHelper.Calculate(mDynParticleAllDimensions,
                    mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
                visualParticle->SetOwnDimensions(mEmitterScale.x * extend,
                    mEmitterScale.y * extend, mEmitterScale.z * extend);
                return;
            }

            float width = 0;
            float height = 0;
            float depth = 0;
            if (mDynParticleWidthSet && mDynParticleWidth)
                width = mDynamicAttributeHelper.Calculate(mDynParticleWidth, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
            
            if (mDynParticleHeightSet && mDynParticleHeight)
                height = mDynamicAttributeHelper.Calculate(mDynParticleHeight, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
            
            if (mDynParticleDepthSet && mDynParticleDepth)
                depth = mDynamicAttributeHelper.Calculate(mDynParticleDepth, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());

            if (mDynParticleWidthSet || mDynParticleHeightSet || mDynParticleDepthSet)
                visualParticle->SetOwnDimensions(mEmitterScale.x * width, mEmitterScale.y * height, mEmitterScale.z * depth);
        }
        else
        {
            visualParticle->originWidth  = visualParticle->width  = mEmitterScale.x * mParentElement->GetDefaultWidth();
            visualParticle->originHeight = visualParticle->height = mEmitterScale.y * mParentElement->GetDefaultHeight();
            visualParticle->originDepth  = visualParticle->depth  = mEmitterScale.z * mParentElement->GetDefaultDepth();
            visualParticle->CalculateBoundingSphereRadius();
        }
    }

    void DiParticleEmitter::InitForEmission(void)
    {
        // The emitter itself is emitted.
        DiParticle::InitForEmission();

        // Emitting an emitter is similar as starting one.
        NotifyStart();
    }

    void DiParticleEmitter::InitForExpiration(DiParticleElement* element, float timeElapsed)
    {
        // The emitter itself is expired.
        DiParticle::InitForExpiration(element, timeElapsed);

        // Expiring an emitter is similar as stopping one.
        NotifyStop();
    }

    void DiParticleEmitter::NotifyStart(void)
    {
        latestPosition = GetDerivedPosition(); // V1.3.1
        mForceEmission = mOriginalForceEmission;
        mForceEmissionExecuted = mOriginalForceEmissionExecuted;
        mRemainder = 0;
        mDurationRemain = 0;
        mRepeatDelayRemain = 0;
        SetEnabled(mOriginalEnabled); // Also calls _initTimeBased

        // Generate the event
        _pushEmitterEvent(PU_EVT_EMITTER_STARTED);
    }

    void DiParticleEmitter::NotifyStop(void)
    {
        _pushEmitterEvent(PU_EVT_EMITTER_STOPPED);
    }

    void DiParticleEmitter::_pushEmitterEvent(EventType eventType)
    {
        // Create the event
        DiFxEvent evt;
        evt.eventType = eventType;
        evt.componentType = CT_EMITTER;
        evt.componentName = GetName();
        evt.technique = 0;
        evt.emitter = this;
        PushEvent(evt);
    }

    void DiParticleEmitter::PushEvent(DiFxEvent& DiFxEvent)
    {
        // Forward the event
        if (mParentElement)
            mParentElement->PushEvent(DiFxEvent);
    }
}

