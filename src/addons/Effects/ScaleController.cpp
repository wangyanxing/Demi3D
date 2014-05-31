
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
#include "ScaleController.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"
#include "VisualParticle.h"

namespace Demi
{
    const DiVec2 DiScaleController::DEFAULT_SCALE = DiVec2::UNIT_SCALE;

    DiScaleController::DiScaleController(void) : 
        DiParticleController(),
        mSinceStartSystem(false)
    {
    }
    
    DiScaleController::~DiScaleController(void)
    {
    }
    
    void DiScaleController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);

        float scale = CalculateScale(&visualParticle->scale, particle);
        float width = visualParticle->originWidth * scale;
        float height = visualParticle->originHeight * scale;
        float depth = visualParticle->originDepth * scale;
        visualParticle->SetScaleDimensions(width, height, depth);
    }
    
    float DiScaleController::CalculateScale(DiDynamicAttribute* dynScale, DiParticle* particle)
    {
        if (!particle)
            return 0.0f;

        if (mSinceStartSystem)
            return mDynamicAttributeHelper.Calculate(dynScale, mParentElement->GetParentSystem()->GetTimeElapsedSinceStart());
        else
            return mDynamicAttributeHelper.Calculate(dynScale, particle->timeFraction);
    }
    
    void DiScaleController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiScaleController* scaleAffector = static_cast<DiScaleController*>(affector);
        scaleAffector->mSinceStartSystem = mSinceStartSystem;
        scaleAffector->mBeginScale       = mBeginScale;
        scaleAffector->mMedScale         = mMedScale;
        scaleAffector->mEndScale         = mEndScale;
    }

    void DiScaleController::InitParticleForEmission(DiParticle* particle)
    {
        // Only continue if the particle is a visual particle
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        // Set initial random zRotation
        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        visualParticle->scale.RemoveAllControlPoints();
        visualParticle->scale.AddControlPoint(0, DiMath::RangeRandom(mBeginScale.x, mBeginScale.y));
        visualParticle->scale.AddControlPoint(0.5f, DiMath::RangeRandom(mMedScale.x, mMedScale.y));
        visualParticle->scale.AddControlPoint(1, DiMath::RangeRandom(mEndScale.x, mEndScale.y));
    }
}

