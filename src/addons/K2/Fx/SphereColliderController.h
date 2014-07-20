
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

#ifndef SphereColliderController_h__
#define SphereColliderController_h__

#include "K2Prerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"
#include "BaseColliderController.h"

namespace Demi
{
    class DEMI_K2_API DiSphereColliderController : public DiBaseColliderController
    {
    public:
        static const float DEFAULT_RADIUS;

        DiSphereColliderController(void);

        virtual             ~DiSphereColliderController(void){}

    public:

        const float         GetRadius(void) const;

        void                SetRadius(const float radius);

        bool                IsInnerCollision(void) const;

        void                SetInnerCollision(bool innerCollision);

        void                CalculateDirectionAfterCollision(DiParticle* particle, DiVec3 distance, float distanceLength);

        virtual void        PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void        Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void        CopyTo (DiParticleController* affector);

    protected:
        
        float               mRadius;
        
        DiSphere            mSphere;

        DiVec3              mPredictedPosition;

        bool                mInnerCollision;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiSphereColliderControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiSphereColliderControllerFactory(void) {}

        virtual               ~DiSphereColliderControllerFactory(void) {}

        DiString              GetControllerType(void) const
        {
            static DiString type = "SphereCollider";
            return type;
        }

        DiParticleController* CreateController(void)
        {
            return CreateControllerImpl<DiSphereColliderController>();
        }
    };
}

#endif // SphereColliderController_h__
