
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

#ifndef PlaneColliderController_h__
#define PlaneColliderController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"
#include "BaseColliderController.h"

namespace Demi
{
    class DEMI_FX_API DiPlaneColliderController : public DiBaseColliderController
    {
    public:
        static const DiVec3 DEFAULT_NORMAL;

        DiPlaneColliderController(void);

        virtual            ~DiPlaneColliderController(void){}

    public:

        const DiVec3    GetNormal(void) const;

        void            SetNormal(const DiVec3& normal);

        virtual void    NotifyRescaled(const DiVec3& scale);

        void            CalculateDirectionAfterCollision(DiParticle* particle, float timeElapsed);

        virtual void     Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void     CopyTo (DiParticleController* affector);

    protected:
        DiVec3            mNormal;
        
        DiVec3            mPredictedPosition;

        DiPlane            mPlane;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiPlaneColliderControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiPlaneColliderControllerFactory(void) {}

        virtual                        ~DiPlaneColliderControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "PlaneCollider";
            return type;
        }

        DiParticleController*        CreateController(void)
        {
            return CreateControllerImpl<DiPlaneColliderController>();
        }
    };
}

#endif // PlaneColliderController_h__
