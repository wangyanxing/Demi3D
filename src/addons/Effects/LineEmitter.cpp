
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
#include "LineEmitter.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"

namespace Demi
{
    const DiVec3 DiLineEmitter::DEFAULT_END                = DiVec3::ZERO;
    const float    DiLineEmitter::DEFAULT_MIN_INCREMENT     = 0.0f;
    const float    DiLineEmitter::DEFAULT_MAX_INCREMENT     = 0.0f;
    const float    DiLineEmitter::DEFAULT_MAX_DEVIATION     = 0.0f;

    
    DiLineEmitter::DiLineEmitter(void) : 
        DiParticleEmitter(),
        mEnd(DEFAULT_END),
        mPerpendicular(DiVec3::ZERO),
        mMaxDeviation(DEFAULT_MAX_DEVIATION),
        mMinIncrement(DEFAULT_MIN_INCREMENT),
        mMaxIncrement(DEFAULT_MAX_INCREMENT),
        mIncrement(0.0f),
        mLength(0.0f),
        mIncrementsLeft(true),
        mFirst(true),
        mScaledEnd(DiVec3::ZERO),
        mScaledMaxDeviation(0.0f),
        mScaledMinIncrement(0.0f),
        mScaledMaxIncrement(0.0f),
        mScaledLength(0.0f)
    {
    }
    
    void DiLineEmitter::NotifyRescaled(const DiVec3& scale)
    {
        DiParticleEmitter::NotifyRescaled(scale);
        float scaleLength = scale.length();
        mScaledEnd = mEnd * scale;
        mScaledMaxDeviation = mMaxDeviation * scaleLength;
        mScaledMinIncrement = mMinIncrement * scaleLength;
        mScaledMaxIncrement = (mMaxIncrement - mMinIncrement) * scaleLength;
        mScaledLength = mScaledEnd.length();
    }
    
    unsigned short DiLineEmitter::CalculateRequestedParticles(float timeElapsed)
    {
        unsigned short requested = DiParticleEmitter::CalculateRequestedParticles(timeElapsed);

        if (mScaledMaxIncrement > 0)
        {
            // Do not create particles if there are no increments left
            if (!mIncrementsLeft)
            {
                // Set back to 0 again
                requested = 0;
            }

            // For duration + repeat/delay: Clear the particles and start all over again.
            if (!mEnabled)
            {
                NotifyStart();
                mParentElement->LockAllParticles(); // Needed to set the first generated particle also first in the list with particles
            }
        }

        return requested;
    }
    
    const DiVec3& DiLineEmitter::GetEnd(void) const
    {
        return mEnd;
    }
    
    void DiLineEmitter::SetEnd(const DiVec3& end)
    {
        mEnd = end;
        mScaledEnd = mEnd * mEmitterScale;
        mLength = mEnd.length();
    }
    
    float DiLineEmitter::GetMaxIncrement(void) const
    {
        return mMaxIncrement;
    }
    
    void DiLineEmitter::SetMaxIncrement(float maxIncrement)
    {
        mMaxIncrement = maxIncrement;
        mScaledMaxIncrement = mMaxIncrement * mEmitterScale.length();
    }
    
    float DiLineEmitter::GetMinIncrement(void) const
    {
        return mMinIncrement;
    }
    
    void DiLineEmitter::SetMinIncrement(float minIncrement)
    {
        mMinIncrement = minIncrement;
        mScaledMinIncrement = mMinIncrement * mEmitterScale.length();
    }
    
    float DiLineEmitter::GetMaxDeviation(void) const
    {
        return mMaxDeviation;
    }
    
    void DiLineEmitter::SetMaxDeviation(float maxDeviation)
    {
        mMaxDeviation = maxDeviation;
        mScaledMaxDeviation = mMaxDeviation * mEmitterScale.length();
    }
    
    void DiLineEmitter::InitParticlePosition(DiParticle* particle)
    {
        // Remark: Don't take the orientation of the node into account. The mEnd position is leading.
        if (mAutoDirection || (mScaledMaxDeviation > 0.0f && !mFirst))
        {
            // Generate a random vector perpendicular on the line if this is required
            mPerpendicular = mEnd.crossProduct(DiVec3(DiMath::RangeRandom(-1, 1), 
                DiMath::RangeRandom(-1, 1), 
                DiMath::RangeRandom(-1, 1)));
            mPerpendicular.normalise();
        }

        // If mMaxIncrement has been set, the particle emission follows a trajectory path along the line
        float fraction = 0.0f;
        if (mScaledMaxIncrement > 0.0f)
        {
            if (!mFirst)
            {
                mIncrement += (mScaledMinIncrement + DiMath::UnitRandom() * mScaledMaxIncrement);
                if (mIncrement >= mScaledLength)
                {
                    mIncrementsLeft = false;
                }
                fraction = mIncrement / mScaledLength;
            }
        }
        else
        {
            fraction = DiMath::UnitRandom();
        }

        // If the deviation has been set, generate a position with a certain distance from the line
        DiVec3 dp = GetDerivedPosition();
        if (mScaledMaxDeviation > 0.0f && mIncrementsLeft)
        {
            if (!mFirst)
            {
                DiVec3 basePosition = dp + fraction * mScaledEnd;
                particle->position = basePosition + mScaledMaxDeviation * DiMath::UnitRandom() * mPerpendicular;
                particle->originalPosition = basePosition;    // Position is without deviation from the line,
                // to make affectors a bit faster/easier.
            }
            else
            {
                particle->position = dp;
                particle->originalPosition = particle->position;
            }
        }
        else
        {
            particle->position = dp + fraction * mScaledEnd;
            particle->originalPosition = particle->position;
        }

        mFirst = false;
    }
    
    void DiLineEmitter::InitParticleDirection(DiParticle* particle)
    {
        if (mAutoDirection)
        {
            DiRadian angle;
            GenerateAngle(angle);
            if (angle != DiRadian(0))
            {
                particle->direction = mPerpendicular.randomDeviant(angle, mUpVector);
                particle->originalDirection = particle->direction;
            }
            else
            {
                particle->direction = mPerpendicular;
                particle->originalDirection = particle->direction;
            }
        }
        else
        {
            DiParticleEmitter::InitParticleDirection(particle);
        }
    }
    
    void DiLineEmitter::CopyTo (DiParticleEmitter* emitter)
    {
        DiParticleEmitter::CopyTo(emitter);

        DiLineEmitter* lineEmitter = static_cast<DiLineEmitter*>(emitter);
        lineEmitter->SetEnd(mEnd);
        lineEmitter->mLength = mLength;
        lineEmitter->SetMinIncrement(mMinIncrement);
        lineEmitter->SetMaxIncrement(mMaxIncrement);
        lineEmitter->SetMaxDeviation(mMaxDeviation);
    }

    void DiLineEmitter::NotifyStart()
    {
        mIncrement = 0.0f;
        mIncrementsLeft = true;
        mFirst = true;
    }

}

