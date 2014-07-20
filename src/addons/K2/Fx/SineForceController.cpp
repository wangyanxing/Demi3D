
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
#include "SineForceController.h"
#include "ParticleElement.h"

namespace Demi
{
    const float DiSineForceController::DEFAULT_FREQ_MIN = 1.0f;
    const float DiSineForceController::DEFAULT_FREQ_MAX = 1.0f;

    
    DiSineForceController::DiSineForceController(void) : 
        DiBaseForceController(),
        mAngle(361),
        mFrequencyMin(DEFAULT_FREQ_MIN),
        mFrequencyMax(DEFAULT_FREQ_MAX),
        mFrequency(1.0f)
    {
    }
    
    void DiSineForceController::CopyTo (DiParticleController* affector)
    {
        DiBaseForceController::CopyTo(affector);

        DiSineForceController* sineForceAffector = static_cast<DiSineForceController*>(affector);
        sineForceAffector->mFrequencyMin = mFrequencyMin;
        sineForceAffector->mFrequencyMax = mFrequencyMax;
        sineForceAffector->mFrequency = mFrequency;
        sineForceAffector->mAngle = mAngle;
    }
    
    void DiSineForceController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        mAngle += mFrequency * timeElapsed;
        float sineValue = DiMath::Sin(DiRadian(mAngle));
        mScaledVector = mForceVector * timeElapsed * sineValue;

        if (mAngle > DiMath::TWO_PI)
        {
            mAngle = 0.0f;

            if (mFrequencyMin != mFrequencyMax)
            {
                mFrequency = DiMath::RangeRandom(mFrequencyMin, mFrequencyMax);
            }
        }
    }
    
    float DiSineForceController::GetFrequencyMin(void) const
    {
        return mFrequencyMin;
    }
    
    void DiSineForceController::SetFrequencyMin(const float frequencyMin)
    {
        mFrequencyMin = frequencyMin;
        if (frequencyMin > mFrequencyMax)
        {
            mFrequency = frequencyMin;
        }
    }
    
    float DiSineForceController::GetFrequencyMax(void) const
    {
        return mFrequencyMax;
    }
    
    void DiSineForceController::SetFrequencyMax(const float frequencyMax)
    {
        mFrequencyMax = frequencyMax;
        mFrequency = frequencyMax;
    }
    
    void DiSineForceController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (mForceApplication == FA_ADD)
        {
            particle->direction += mScaledVector;
        }
        else
        {
            particle->direction = (particle->direction + mForceVector) / 2;
        }
    }
}

