
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
#include "PlaneColliderController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const DiVec3 DiPlaneColliderController::DEFAULT_NORMAL = DiVec3::ZERO;

    
    DiPlaneColliderController::DiPlaneColliderController(void) : 
        DiBaseColliderController(),
        mPredictedPosition(DiVec3::ZERO),
        mNormal(DEFAULT_NORMAL)
    {
    }
    
    const DiVec3 DiPlaneColliderController::GetNormal(void) const
    {
        return mNormal;
    }
    
    void DiPlaneColliderController::SetNormal(const DiVec3& normal)
    {
        mNormal = normal;
        mPlane.redefine(mNormal, GetDerivedPosition());
    }
    
    void DiPlaneColliderController::NotifyRescaled(const DiVec3& scale)
    {
        DiParticleController::NotifyRescaled(scale);
        mPlane.redefine(mNormal, GetDerivedPosition());
    }
    
    void DiPlaneColliderController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        mPredictedPosition = particle->position + mVelocityScale * particle->direction;
        bool collision = false;

        switch(mIntersectionType)
        {
        case DiBaseColliderController::IT_POINT:
            {
                if (mPlane.getDistance(particle->position) <= 0.0f)
                {
                    particle->position -= mVelocityScale * particle->direction;
                    collision = true;
                }
                else if (mPlane.getDistance(mPredictedPosition) <= 0.0f)
                {
                    collision = true;
                }
            }
            break;

        case DiBaseColliderController::IT_BOX:
            {
                if (particle->particleType != DiParticle::PT_VISUAL)
                    break;

                DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
                DiAABB box;
                PopulateAlignedBox(box,
                    visualParticle->position, 
                    visualParticle->width, 
                    visualParticle->height,
                    visualParticle->depth);
                if (box.Intersects(mPlane))
                {
                    particle->position -= mVelocityScale * particle->direction;
                    collision = true;
                }
                else 
                {
                    PopulateAlignedBox(box,
                        mPredictedPosition, 
                        visualParticle->width, 
                        visualParticle->height,
                        visualParticle->depth);
                    if (box.Intersects(mPlane))
                    {
                        collision = true;
                    }
                }
            }
            break;
        }

        if (collision)
        {
            CalculateDirectionAfterCollision(particle, timeElapsed);
            CalculateRotationSpeedAfterCollision(particle);
            particle->AddEventFlags(DiParticle::PEF_COLLIDED);
        }
    }
    
    void DiPlaneColliderController::CalculateDirectionAfterCollision(DiParticle* particle, float timeElapsed)
    {
        float directionLength = particle->direction.length();
        switch (mCollisionType)
        {
        case DiBaseColliderController::CT_BOUNCE:
            {
                particle->direction.normalise();
                particle->direction = 2 * (-particle->direction.dotProduct(-mNormal)) * -mNormal + particle->direction;

                particle->direction *= directionLength;
                particle->direction *= mBouncyness;
            }
            break;
        case DiBaseColliderController::CT_FLOW:
            {
                particle->position += timeElapsed * directionLength * mNormal;
            }
            break;
        default:
            break;
        }
    }
    
    void DiPlaneColliderController::CopyTo (DiParticleController* affector)
    {
        DiBaseColliderController::CopyTo(affector);

        DiPlaneColliderController* planeCollider = static_cast<DiPlaneColliderController*>(affector);
        planeCollider->SetNormal(mNormal);
    }
}

