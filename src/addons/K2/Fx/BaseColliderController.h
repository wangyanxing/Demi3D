
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

#include "K2Prerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_K2_API DiBaseColliderController : public DiParticleController
    {
    public:
        /** Determines how a particle collision should be determined. IT_POINT means that the position of
        the particle will be validated against the collider' shape. IT_BOX means that the dimensions
        (width, height and depth) are used to determine whether the particle collides.
        */
        enum IntersectionType
        {
            IT_POINT,
            IT_BOX
        };

        /** Determines how a particle behaves after collision with this collider. The behavior of the
        particle is solved in the collider and only behavior that needs the colliders?data is taken
        into account. The fact that a particle expires for example, can be achieved by using an
        Observer in combination with an EventHandler (DoExpireEventHandler).
        CT_NONE means that the particle doesn't do anything. This value should be set if the behavior
        of the particle is determined outside the collider (for example, expiring the particle).
        CT_BOUNCE means that the particle bounces off the collider.
        CT_FLOW means that the particle flows around the contours of the collider.
        */
        enum CollisionType
        {
            CT_NONE,
            CT_BOUNCE,
            CT_FLOW,
        };

        static const float              DEFAULT_BOUNCYNESS;
        static const float              DEFAULT_FRICTION;
        static const IntersectionType   DEFAULT_INTERSECTION_TYPE;
        static const CollisionType      DEFAULT_COLLISION_TYPE;

        DiBaseColliderController(void);

        virtual                        ~DiBaseColliderController(void){}

    public:

        const IntersectionType         GetIntersectionType(void) const;

        void                           SetIntersectionType(const IntersectionType& intersectionType);

        const CollisionType            GetCollisionType(void) const;

        void                           SetCollisionType(const CollisionType& collisionType);

        const float                    GetFriction(void) const;

        void                           SetFriction(const float friction);

        const float                    GetBouncyness(void) const;

        void                           SetBouncyness(const float bouncyness);

        virtual void                   PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        void                           PopulateAlignedBox(DiAABB& box,
                                           const DiVec3& position, 
                                           const float width,
                                           const float height,
                                           const float depth);

        void                           CalculateRotationSpeedAfterCollision(DiParticle* particle);

        virtual void                   CopyTo (DiParticleController* affector);

    protected:
        float                          mFriction;

        float                          mBouncyness;

        IntersectionType               mIntersectionType;
        
        CollisionType                  mCollisionType;
        
        float                          mVelocityScale;
    };
}

#endif // BaseColliderController_h__
