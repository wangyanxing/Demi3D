
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
#include "LinearForceController.h"
#include "ParticleElement.h"

namespace Demi
{
    void DiLinearForceController::CopyTo (DiParticleController* affector)
    {
        DiBaseForceController::CopyTo(affector);
    }
    
    void DiLinearForceController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        mScaledVector = mForceVector * timeElapsed;
    }
    
    void DiLinearForceController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (mForceApplication == FA_ADD)
        {
            particle->direction += mScaledVector * CalculateAffectSpecialisationFactor(particle);
        }
        else
        {
            particle->direction = (particle->direction + mForceVector) / 2;
        }
    }
}

