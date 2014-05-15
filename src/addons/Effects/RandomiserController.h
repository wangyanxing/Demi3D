
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

#ifndef RandomiserController_h__
#define RandomiserController_h__


#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiRandomiserController : public DiParticleController
    {
    public:
        static const DiVec3 DEFAULT_MAX_DEVIATION;
        static const float    DEFAULT_TIME_STEP;
        static const bool    DEFAULT_RANDOM_DIRECTION;

        DiRandomiserController(void);

        virtual            ~DiRandomiserController(void) {}

    public:

        float            GetMaxDeviationX(void) const;

        void            SetMaxDeviationX(const float maxDeviationX);

        float            GetMaxDeviationY(void) const;
        
        void            SetMaxDeviationY(const float maxDeviationZ);

        float            GetMaxDeviationZ(void) const;
        
        void            SetMaxDeviationZ(const float maxDeviationZ);

        float            GetTimeStep(void) const;
        
        void            SetTimeStep(const float timeStep);

        bool            IsRandomDirection(void) const;
        
        void            SetRandomDirection(bool randomDirection);

        virtual void    PreProcessParticles(DiParticleElement* particleTechnique, float timeElapsed);

        virtual void    Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void    PostProcessParticles(DiParticleElement* technique, float timeElapsed);

        virtual void    CopyTo (DiParticleController* affector);

    protected:
        float             mMaxDeviationX;

        float             mMaxDeviationY;

        float             mMaxDeviationZ;

        float             mTimeSinceLastUpdate;

        float             mTimeStep;

        bool             mRandomDirection;

        bool             mUpdate;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiRandomiserControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiRandomiserControllerFactory(void) {}

        virtual                        ~DiRandomiserControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "Randomiser";
            return type;
        }

        DiParticleController*        CreateController(void)
        {
            return CreateControllerImpl<DiRandomiserController>();
        }
    };
}

#endif // RandomiserController_h__
