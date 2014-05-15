
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

#ifndef BaseColliderController_h__
#define BaseColliderController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiBaseColliderController : public DiParticleController
    {
    public:
        // 粒子的与控制器的碰撞方式
        enum IntersectionType
        {
            IT_POINT,    //将粒子视为质点参与碰撞运算
            IT_BOX        //将粒子视为有长/宽/高参数的box参与碰撞运算
        };

        // 碰撞发生后的反馈行为类型
        enum CollisionType
        {
            CT_NONE,        //不做任何事
            CT_BOUNCE,        //反弹
            CT_FLOW,        //沿着碰撞器周围轮廓流动
        };

        static const float                DEFAULT_BOUNCYNESS;
        static const float                DEFAULT_FRICTION;
        static const IntersectionType    DEFAULT_INTERSECTION_TYPE;
        static const CollisionType        DEFAULT_COLLISION_TYPE;

        DiBaseColliderController(void);

        virtual                        ~DiBaseColliderController(void){}

    public:

        const IntersectionType        GetIntersectionType(void) const;

        void                        SetIntersectionType(const IntersectionType& intersectionType);

        const CollisionType            GetCollisionType(void) const;

        void                        SetCollisionType(const CollisionType& collisionType);

        // 摩擦力
        const float                    GetFriction(void) const;

        void                        SetFriction(const float friction);

        // 弹性系数
        const float                    GetBouncyness(void) const;

        void                        SetBouncyness(const float bouncyness);

        virtual void                PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        // 生成AABB用于碰撞计算
        void                        PopulateAlignedBox(DiAABB& box,
                                        const DiVec3& position, 
                                        const float width,
                                        const float height,
                                        const float depth);

        // 碰撞后重新计算粒子的旋转速度
        void                        CalculateRotationSpeedAfterCollision(DiParticle* particle);

        virtual void                CopyTo (DiParticleController* affector);

    protected:
        // 摩擦力，影响粒子的旋转速度参数
        float                        mFriction;

        // 弹性系数，影响粒子的速度参数
        float                        mBouncyness;

        IntersectionType            mIntersectionType;
        
        CollisionType                mCollisionType;
        
        float                        mVelocityScale;
    };
}

#endif // BaseColliderController_h__
