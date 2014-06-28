
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
#include "BoxColliderController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"

namespace Demi
{
    const float DiBoxColliderController::DEFAULT_WIDTH = 100.0f;
    const float DiBoxColliderController::DEFAULT_HEIGHT = 100.0f;
    const float DiBoxColliderController::DEFAULT_DEPTH = 100.0f;
    
    DiBoxColliderController::DiBoxColliderController(void) : 
        DiBaseColliderController(),
        mWidth(DEFAULT_WIDTH),
        mHeight(DEFAULT_HEIGHT),
        mDepth(DEFAULT_DEPTH),
        mXmin(0.0f),
        mXmax(0.0f),
        mYmin(0.0f),
        mYmax(0.0f),
        mZmin(0.0f),
        mZmax(0.0f),
        mPredictedPosition(DiVec3::ZERO),
        mInnerCollision(false)
    {
    }
    
    float DiBoxColliderController::GetWidth(void) const
    {
        return mWidth;
    }
    
    void DiBoxColliderController::SetWidth(const float width)
    {
        mWidth = width;
    }
    
    float DiBoxColliderController::GetHeight(void) const
    {
        return mHeight;
    }
    
    void DiBoxColliderController::SetHeight(const float height)
    {
        mHeight = height;
    }
    
    float DiBoxColliderController::GetDepth(void) const
    {
        return mDepth;
    }
    
    void DiBoxColliderController::SetDepth(const float depth)
    {
        mDepth = depth;
    }
    
    bool DiBoxColliderController::IsInnerCollision(void) const
    {
        return mInnerCollision;
    }
    
    void DiBoxColliderController::SetInnerCollision(bool innerCollision)
    {
        mInnerCollision = innerCollision;
    }
    
    void DiBoxColliderController::PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed)
    {
        DiBaseColliderController::PreProcessParticles(particleTechnique, timeElapsed);

        PopulateAlignedBox(mBox, GetDerivedPosition(), mControllerScale.x * mWidth,
            mControllerScale.y * mHeight, mControllerScale.z * mDepth);
        CalculateBounds();
    }
    
    void DiBoxColliderController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        // 预计位置，即按照方向和当前位置计算可能的下一处位置
        mPredictedPosition = particle->position + mVelocityScale * particle->direction;

        bool collision = false;

        // 首先，判断粒子目前是否处于碰撞状态，如果是则先重新计算位置
        // 不过因为有时间差的原因，相关数值并不与粒子碰撞之前一样，所以我们首先
        // 计算了一个mPredictedPosition变量来“预测”可能的下一处位置来继续参与碰撞判断
        switch(mIntersectionType)
        {
        case DiBaseColliderController::IT_POINT:
            {
                // 先检测粒子点是否在box内部
                if (mInnerCollision != mBox.Intersects(particle->position))
                {
                    // 有碰撞，重新计算位置
                    particle->position -= mVelocityScale * particle->direction;
                    collision = true;
                }
                else if (mInnerCollision != mBox.Intersects(mPredictedPosition))
                {
                    // 碰撞已检测
                    collision = true;
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

                if (mInnerCollision != box.Intersects(mBox))
                {
                    // 有碰撞，重新计算位置
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
                    if (mInnerCollision != box.Intersects(mBox))
                    {
                        // 碰撞已检测
                        collision = true;
                    }
                }
            }
            break;
        }

        if (collision)
        {
            CalculateDirectionAfterCollision(particle);
            CalculateRotationSpeedAfterCollision(particle);
            particle->AddEventFlags(DiParticle::PEF_COLLIDED);
        }
    }
    
    void DiBoxColliderController::CalculateDirectionAfterCollision(DiParticle* particle)
    {
        switch (mCollisionType)
        {
        case DiBaseColliderController::CT_BOUNCE:
            {
                if (IsSmallestValue (particle->position.x - mXmin, particle->position))
                {        
                    particle->direction.x *= -1;
                }
                else if (IsSmallestValue (mXmax - particle->position.x, particle->position))
                {
                    particle->direction.x *= -1;
                }
                else if (IsSmallestValue (particle->position.y - mYmin, particle->position))
                {
                    particle->direction.y *= -1;
                }
                else if (IsSmallestValue (mYmax - particle->position.y, particle->position))
                {
                    particle->direction.y *= -1;
                }
                else if (IsSmallestValue (particle->position.z - mZmin, particle->position))
                {
                    particle->direction.z *= -1;
                }
                else if (IsSmallestValue (mZmax - particle->position.z, particle->position))
                {
                    particle->direction.z *= -1;
                }
                particle->direction *= mBouncyness;
            }
            break;
        case DiBaseColliderController::CT_FLOW:
            {
                if (IsSmallestValue (particle->position.x - mXmin, particle->position))
                {        
                    particle->direction.x = 0;
                }
                else if (IsSmallestValue (mXmax - particle->position.x, particle->position))
                {
                    particle->direction.x = 0;
                }
                else if (IsSmallestValue (particle->position.y - mYmin, particle->position))
                {
                    particle->direction.y = 0;
                }
                else if (IsSmallestValue (mYmax - particle->position.y, particle->position))
                {
                    particle->direction.y = 0;
                }
                else if (IsSmallestValue (particle->position.z - mZmin, particle->position))
                {
                    particle->direction.z = 0;
                }
                else if (IsSmallestValue (mZmax - particle->position.z, particle->position))
                {
                    particle->direction.z = 0;
                }
                particle->direction *= -mFriction;
            }
            break;
        default:
            break;
        }
    }
    
    void DiBoxColliderController::CalculateBounds (void)
    {
        float scaledWidth = mControllerScale.x * mWidth;
        float scaledHeight = mControllerScale.y * mHeight;
        float scaledDepth = mControllerScale.z * mDepth;

        mXmin = mDerivedPosition.x - 0.5f * scaledWidth;
        mXmax = mDerivedPosition.x + 0.5f * scaledWidth;
        mYmin = mDerivedPosition.y - 0.5f * scaledHeight;
        mYmax = mDerivedPosition.y + 0.5f * scaledHeight;
        mZmin = mDerivedPosition.z - 0.5f * scaledDepth;
        mZmax = mDerivedPosition.z + 0.5f * scaledDepth;
    }
    
    bool DiBoxColliderController::IsSmallestValue(float value, const DiVec3& particlePosition)
    {
        float value1 = particlePosition.x - mXmin;
        float value2 = mXmax - particlePosition.x;
        float value3 = particlePosition.y - mYmin;
        float value4 = mYmax - particlePosition.y;
        float value5 = particlePosition.z - mZmin;
        float value6 = mZmax - particlePosition.z;

        return (
            value <= value1 && 
            value <= value2 &&
            value <= value3 && 
            value <= value4 &&
            value <= value5 &&
            value <= value6);
    }
    
    void DiBoxColliderController::CopyTo (DiParticleController* affector)
    {
        DiBaseColliderController::CopyTo(affector);

        DiBoxColliderController* boxCollider = static_cast<DiBoxColliderController*>(affector);
        boxCollider->mWidth = mWidth;
        boxCollider->mHeight = mHeight;
        boxCollider->mDepth = mDepth;
        boxCollider->mInnerCollision = mInnerCollision;
    }
}