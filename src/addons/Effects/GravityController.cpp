
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
#include "GravityController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const float DiGravityController::DEFAULT_GRAVITY = 1.0f;

    
    DiGravityController::DiGravityController(void) : 
        DiParticleController(),
        mGravity(DEFAULT_GRAVITY)
    {
    }
    
    void DiGravityController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiGravityController* gravityAffector = static_cast<DiGravityController*>(affector);
        gravityAffector->mGravity = mGravity;
    }
    
    void DiGravityController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        GetDerivedPosition();
    }
    
    inline void DiGravityController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        DiVec3 distance = mDerivedPosition - particle->position;
        float length = distance.squaredLength();
        
        float scaleVelocity = 1.0f;
        if (mParentElement)
            scaleVelocity = mParentElement->GetParticleSystemScaleVelocity();
        
        if (length > 0 && mParentElement)
        {
            //float force = (mGravity * particle->mass * mass) / length;
            float force = (scaleVelocity * mGravity * particle->mass * mass) / length;
            particle->direction += force * distance * timeElapsed * CalculateAffectSpecialisationFactor(particle);
        }
    }
    
    float DiGravityController::GetGravity(void) const
    {
        return mGravity;
    }
    
    void DiGravityController::SetGravity(float gravity)
    {
        mGravity = gravity;
    }
}