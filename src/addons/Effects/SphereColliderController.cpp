
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
#include "SphereColliderController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const float DiSphereColliderController::DEFAULT_RADIUS = 100.0f;
    
    DiSphereColliderController::DiSphereColliderController(void) : 
        DiBaseColliderController(),
        mPredictedPosition(DiVec3::ZERO),
        mRadius(DEFAULT_RADIUS),
        mInnerCollision(false)
    {
    }
    
    const float DiSphereColliderController::GetRadius(void) const
    {
        return mRadius;
    }
    
    void DiSphereColliderController::SetRadius(const float radius)
    {
        mRadius = radius;
        mSphere.setRadius(mRadius);
    }
    
    bool DiSphereColliderController::IsInnerCollision(void) const
    {
        return mInnerCollision;
    }
    
    void DiSphereColliderController::SetInnerCollision(bool innerCollision)
    {
        mInnerCollision = innerCollision;
    }
    
    void DiSphereColliderController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        DiBaseColliderController::PreProcessParticles(particleTechnique, timeElapsed);
        mSphere.setCenter(GetDerivedPosition());
    }
    
    void DiSphereColliderController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        mPredictedPosition = particle->position + mVelocityScale * particle->direction;
        bool collision = false;
        DiVec3 distance = particle->position - mDerivedPosition;
        float distanceLength = distance.length();
        float scaledRadius = 0.3333f * (mControllerScale.x + mControllerScale.y + mControllerScale.z) * mRadius; // Scaling changed in V 1.3.1

        switch(mIntersectionType)
        {
            case DiBaseColliderController::IT_POINT:
            {
                if (mInnerCollision == (distanceLength > scaledRadius))
                {
                    particle->position -= mVelocityScale * particle->direction;
                    collision = true;
                }
                else
                {
                    distance = mPredictedPosition - mDerivedPosition;
                    distanceLength = distance.length();
                    if (mInnerCollision == (distanceLength > scaledRadius))
                    {
                        collision = true;
                    }
                }
            }
            break;

            case DiBaseColliderController::IT_BOX:
            {
                if (particle->particleType != DiParticle::PT_VISUAL)
                {
                    break;
                }

                DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);
                DiAABB box;
                PopulateAlignedBox(box,
                    visualParticle->position, 
                    visualParticle->width, 
                    visualParticle->height,
                    visualParticle->depth);

                    if (mInnerCollision != box.Intersects(mSphere))
                    {
                        particle->position -= mVelocityScale * particle->direction;
                        collision = true;
                    }
                    else
                    {
                        DiAABB box;
                        PopulateAlignedBox(box,
                            mPredictedPosition, 
                            visualParticle->width, 
                            visualParticle->height,
                            visualParticle->depth);
                        if (mInnerCollision != box.Intersects(mSphere))
                        {
                            collision = true;
                        }
                    }
            }
            break;
        }

        if (collision)
        {
            CalculateDirectionAfterCollision(particle, distance, distanceLength);
            CalculateRotationSpeedAfterCollision(particle);
            particle->AddEventFlags(DiParticle::PEF_COLLIDED);
        }
    }
    
    void DiSphereColliderController::CalculateDirectionAfterCollision(DiParticle* particle, DiVec3 distance, float distanceLength)
    {
        switch (mCollisionType)
        {
            case DiBaseColliderController::CT_BOUNCE:
            {
                /** 如果粒子在球面表面（或刚好穿插进球体里面）则按照如下公式计算
                    公式：R = 2 * (-I dot N) * N + I, 其中
                    R = 新的方向向量
                    I = 碰撞前的方向向量（单位向量）
                    N = 碰撞点发现
                */
                float directionLength = particle->direction.length();
                particle->direction.normalise();
                distance.normalise();
                particle->direction = 2 * (-particle->direction.dotProduct(distance)) * distance + particle->direction;

                particle->direction *= directionLength;

                // 弹性系数叠加
                particle->direction *= mBouncyness;
            }
            break;
            case DiBaseColliderController::CT_FLOW:
            {
                //particle->position = mDerivedPosition + distance * (mRadius / distanceLength);
                float scaledRadius = 0.3333f * (mControllerScale.x + mControllerScale.y + mControllerScale.z) * mRadius;
                particle->position = mDerivedPosition + distance * (scaledRadius / distanceLength);
            }
            break;
        }
    }
    
    void DiSphereColliderController::CopyTo (DiParticleController* affector)
    {
        DiBaseColliderController::CopyTo(affector);

        DiSphereColliderController* sphereCollider = static_cast<DiSphereColliderController*>(affector);
        sphereCollider->mRadius = mRadius;
        sphereCollider->mSphere = mSphere;
        sphereCollider->mInnerCollision = mInnerCollision;
    }
}

