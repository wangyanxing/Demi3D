
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
#include "JetController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const float DiJetController::DEFAULT_ACCELERATION = 1.0f;

    DiJetController::DiJetController (void) : 
    DiParticleController(),
        mScaled(0.0f)
    {
        mDynAcceleration = DI_NEW DiAttributeFixed();
        (static_cast<DiAttributeFixed*>(mDynAcceleration))->SetValue(DEFAULT_ACCELERATION);
    }
    
    DiJetController::~DiJetController (void)
    {
        SAFE_DELETE(mDynAcceleration);
    }
    
    void DiJetController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiJetController* jetAffector = static_cast<DiJetController*>(affector);
        jetAffector->SetDynAcceleration(mDynamicAttributeFactory.CloneDynamicAttribute(GetDynAcceleration()));
    }
    
    void DiJetController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        mScaled = timeElapsed * (mDynAcceleration->GetValue(particle->timeFraction));
        if (particle->direction == DiVec3::ZERO)
        {
            particle->direction += (particle->originalDirection * mScaled);
        }
        else
        {
            particle->direction += (particle->direction * mScaled);
        }
    }
    
    void DiJetController::SetDynAcceleration(DiDynamicAttribute* dynAcceleration)
    {
        SAFE_DELETE(mDynAcceleration);
        mDynAcceleration = dynAcceleration;
    }
}

