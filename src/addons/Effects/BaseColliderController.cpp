
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
#include "BaseColliderController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const float DiBaseColliderController::DEFAULT_BOUNCYNESS = 1.0f;
    const float DiBaseColliderController::DEFAULT_FRICTION = 0.0f;
    const DiBaseColliderController::IntersectionType DiBaseColliderController::DEFAULT_INTERSECTION_TYPE = DiBaseColliderController::IT_POINT;
    const DiBaseColliderController::CollisionType DiBaseColliderController::DEFAULT_COLLISION_TYPE = DiBaseColliderController::CT_BOUNCE;

    DiBaseColliderController::DiBaseColliderController(void) :
        DiParticleController(),
        mBouncyness(DEFAULT_BOUNCYNESS),
        mFriction(DEFAULT_FRICTION),
        mIntersectionType(DEFAULT_INTERSECTION_TYPE),
        mCollisionType(DEFAULT_COLLISION_TYPE),
        mVelocityScale(1.0f)
    {
    }
    
    void DiBaseColliderController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        mVelocityScale = 1.0f;
        if (mParentElement)
        {
            mVelocityScale = timeElapsed * mParentElement->GetParticleSystemScaleVelocity();
        }
    }
    
    const DiBaseColliderController::IntersectionType DiBaseColliderController::GetIntersectionType(void) const
    {
        return mIntersectionType;
    }
    
    void DiBaseColliderController::SetIntersectionType(const DiBaseColliderController::IntersectionType& intersectionType)
    {
        mIntersectionType = intersectionType;
    }
    
    const DiBaseColliderController::CollisionType DiBaseColliderController::GetCollisionType(void) const
    {
        return mCollisionType;
    }
    
    void DiBaseColliderController::SetCollisionType(const DiBaseColliderController::CollisionType& collisionType)
    {
        mCollisionType = collisionType;
    }
    
    const float DiBaseColliderController::GetFriction(void) const
    {
        return mFriction;
    }
    
    void DiBaseColliderController::SetFriction(const float friction)
    {
        mFriction = friction;
    }
    
    const float DiBaseColliderController::GetBouncyness(void) const
    {
        return mBouncyness;
    }
    
    void DiBaseColliderController::SetBouncyness(const float bouncyness)
    {
        mBouncyness = bouncyness;
    }
    
    void DiBaseColliderController::PopulateAlignedBox(DiAABB& box,
        const DiVec3& position, 
        const float width,
        const float height,
        const float depth)
    {
        float halfWidth = 0.5f * width;
        float halfHeight = 0.5f * height;
        float halfDepth = 0.5f * depth;
        box.SetMinimum(position.x - halfWidth, 
            position.y - halfHeight, 
            position.z - halfDepth);
        box.SetMaximum(position.x + halfWidth, 
            position.y + halfHeight, 
            position.z + halfDepth);
    }
    
    void DiBaseColliderController::CalculateRotationSpeedAfterCollision(DiParticle* particle)
    {
        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
        float signedFriction = DiMath::UnitRandom() > 0.5 ? -(mFriction - 1) : (mFriction - 1);
        visualParticle->rotationSpeed *= signedFriction;
        visualParticle->zRotationSpeed *= signedFriction;
    }
    
    void DiBaseColliderController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiBaseColliderController* baseCollider = static_cast<DiBaseColliderController*>(affector);
        baseCollider->mBouncyness = mBouncyness;
        baseCollider->mFriction = mFriction;
        baseCollider->mIntersectionType = mIntersectionType;
        baseCollider->mCollisionType = mCollisionType;
    }
}

