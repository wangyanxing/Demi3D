
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
#include "Particle.h"

namespace Demi
{


    DiParticle::~DiParticle()
    {

    }

    void DiParticle::InitForEmission( void )
    {
        mEventFlags = 0;
        timeFraction = 0.0f;
        
        AddEventFlags(DiParticle::PEF_EMITTED);

        mFreezed = false;
    }

    void DiParticle::InitForExpiration( DiParticleElement* technique, float timeElapsed )
    {
    }

    bool DiParticle::IsEnabled( void ) const
    {
        return mEnabled;
    }

    void DiParticle::SetEnabled( bool enabled )
    {
        mEnabled = enabled;
        if (!mOriginalEnabledSet)
        {
            mOriginalEnabled = enabled;
            mOriginalEnabledSet = true;
        }
    }

    void DiParticle::SetOriginalEnabled( bool originalEnabled )
    {
        mOriginalEnabled = originalEnabled;
        mOriginalEnabledSet = true;
    }

    bool DiParticle::GetOriginalEnabled( void ) const
    {
        return mOriginalEnabled;
    }

    bool DiParticle::IsFreezed( void ) const
    {
        return mFreezed;
    }

    void DiParticle::SetFreezed( bool freezed )
    {
        mFreezed = freezed;
    }

    void DiParticle::Process( DiParticleElement* technique, float timeElapsed )
    {
        timeFraction = (totalTimeToLive - timeToLive) / totalTimeToLive;
    }

    float DiParticle::CalculateVelocity( void ) const
    {
        if (originalScaledDirectionLength != 0)
            return originalVelocity * (direction.length() / originalScaledDirectionLength);
        else
            return originalVelocity * direction.length();
    }

    void DiParticle::CopyTo( DiParticle* particle )
    {
        particle->position                      = position;
        particle->originalPosition              = originalPosition;
        particle->mDerivedPosition              = mDerivedPosition;
        particle->direction                     = direction;
        particle->originalDirection             = originalDirection;
        particle->originalDirectionLength       = originalDirectionLength;
        particle->originalScaledDirectionLength = originalScaledDirectionLength;
        particle->originalVelocity              = originalVelocity;
        particle->mass                          = mass;
        particle->timeToLive                    = timeToLive;
        particle->totalTimeToLive               = totalTimeToLive;
        particle->mEventFlags                   = mEventFlags;
        particle->mMarkedForEmission            = mMarkedForEmission;
        particle->mEnabled                      = mEnabled;
        particle->mOriginalEnabled              = mOriginalEnabled;
        particle->mOriginalEnabledSet           = mOriginalEnabledSet;
        particle->mFreezed                      = mFreezed;
        particle->timeFraction                  = timeFraction;
    }

    float DiParticle::DEFAULT_MASS = 1.0f;
    float DiParticle::DEFAULT_TTL = 10.0f;

}

