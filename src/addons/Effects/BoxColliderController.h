
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

#ifndef BoxColliderController_h__
#define BoxColliderController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"
#include "BaseColliderController.h"

namespace Demi
{
    class DEMI_FX_API DiBoxColliderController : public DiBaseColliderController
    {
    public:
        static const float DEFAULT_WIDTH;
        static const float DEFAULT_HEIGHT;
        static const float DEFAULT_DEPTH;

        DiBoxColliderController(void);

        virtual             ~DiBoxColliderController(void){}

        const float         GetWidth(void) const;

        void                SetWidth(const float width);

        const float         GetHeight(void) const;

        void                SetHeight(const float height);

        const float         GetDepth(void) const;

        void                SetDepth(const float depth);

        bool                IsInnerCollision(void) const;

        void                SetInnerCollision(bool innerCollision);

        void                CalculateDirectionAfterCollision(DiParticle* particle);

        virtual void        PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void        Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void        CopyTo (DiParticleController* affector);

    protected:

        void                CalculateBounds(void);

        bool                IsSmallestValue(float value, const DiVec3& particlePosition);

    protected:
        float               mWidth;

        float               mHeight;

        float               mDepth;

        float               mXmin;

        float               mXmax;

        float               mYmin;

        float               mYmax;

        float               mZmin;

        float               mZmax;

        DiAABB              mBox;

        DiVec3              mPredictedPosition;

        bool                mInnerCollision;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiBoxColliderControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiBoxColliderControllerFactory(void) {}

        virtual     ~DiBoxColliderControllerFactory(void) {}

        DiString    GetControllerType(void) const
        {
            static DiString type = "BoxCollider";
            return type;
        }

        DiParticleController*   CreateController(void)
        {
            return CreateControllerImpl<DiBoxColliderController>();
        }
    };
}
#endif // BoxColliderController_h__

