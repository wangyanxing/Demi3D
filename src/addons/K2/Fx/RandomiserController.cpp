
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
#include "RandomiserController.h"
#include "ParticleElement.h"

namespace Demi
{
    const DiVec3    DiRandomiserController::DEFAULT_MAX_DEVIATION        = DiVec3::ZERO;
    const float        DiRandomiserController::DEFAULT_TIME_STEP            = 0.0f;
    const bool        DiRandomiserController::DEFAULT_RANDOM_DIRECTION    = true;

    DiRandomiserController::DiRandomiserController(void) : 
        DiParticleController(),
        mMaxDeviationX(DEFAULT_MAX_DEVIATION.x),
        mMaxDeviationY(DEFAULT_MAX_DEVIATION.y),
        mMaxDeviationZ(DEFAULT_MAX_DEVIATION.z),
        mTimeSinceLastUpdate(0.0f),
        mTimeStep(DEFAULT_TIME_STEP),
        mUpdate(true),
        mRandomDirection(DEFAULT_RANDOM_DIRECTION)
    {
    }
    
    float DiRandomiserController::GetMaxDeviationX(void) const
    {
        return mMaxDeviationX;
    }
    
    void DiRandomiserController::SetMaxDeviationX(float maxDeviationX)
    {
        mMaxDeviationX = maxDeviationX;
    }
    
    float DiRandomiserController::GetMaxDeviationY(void) const
    {
        return mMaxDeviationY;
    }
    
    void DiRandomiserController::SetMaxDeviationY(float maxDeviationY)
    {
        mMaxDeviationY = maxDeviationY;
    }
    
    float DiRandomiserController::GetMaxDeviationZ(void) const
    {
        return mMaxDeviationZ;
    }
    
    void DiRandomiserController::SetMaxDeviationZ(float maxDeviationZ)
    {
        mMaxDeviationZ = maxDeviationZ;
    }
    
    float DiRandomiserController::GetTimeStep(void) const
    {
        return mTimeStep;
    }
    
    void DiRandomiserController::SetTimeStep(float timeStep)
    {
        mTimeStep = timeStep;
        mTimeSinceLastUpdate = timeStep;
    }
    
    bool DiRandomiserController::IsRandomDirection(void) const
    {
        return mRandomDirection;
    }
    
    void DiRandomiserController::SetRandomDirection(bool randomDirection)
    {
        mRandomDirection = randomDirection;
    }
    
    void DiRandomiserController::PreProcessParticles(DiParticleElement* technique, float timeElapsed)
    {
        if (technique->GetNumberOfEmittedParticles() > 0)
        {
            mTimeSinceLastUpdate += timeElapsed;
            if (mTimeSinceLastUpdate > mTimeStep)
            {
                mTimeSinceLastUpdate -= mTimeStep;
                mUpdate = true;
            }
        }
    }
    
    void DiRandomiserController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (mUpdate)
        {
            if (mRandomDirection)
            {
                // 方向随机
                particle->direction += DiVec3(DiMath::RangeRandom(-mMaxDeviationX, mMaxDeviationX),
                    DiMath::RangeRandom(-mMaxDeviationY, mMaxDeviationY),
                    DiMath::RangeRandom(-mMaxDeviationZ, mMaxDeviationZ));
            }
            else
            {
                if (particle->IsFreezed())
                {
                    return;
                }

                // 位置随机
                particle->position += mControllerScale * DiVec3(DiMath::RangeRandom(-mMaxDeviationX, mMaxDeviationX),
                    DiMath::RangeRandom(-mMaxDeviationY, mMaxDeviationY),
                    DiMath::RangeRandom(-mMaxDeviationZ, mMaxDeviationZ));
            }
        }
    }
    
    void DiRandomiserController::PostProcessParticles(DiParticleElement* technique, float timeElapsed)
    {
        mUpdate = false;
    }
    
    void DiRandomiserController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiRandomiserController* randomiser = static_cast<DiRandomiserController*>(affector);
        randomiser->mMaxDeviationX = mMaxDeviationX;
        randomiser->mMaxDeviationY = mMaxDeviationY;
        randomiser->mMaxDeviationZ = mMaxDeviationZ;
        randomiser->mRandomDirection = mRandomDirection;
    }
}

