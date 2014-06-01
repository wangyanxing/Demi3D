
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

#ifndef ScaleController_h__
#define ScaleController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiScaleController : public DiParticleController
    {
    public:
        static const DiVec2           DEFAULT_SCALE;

        DiScaleController(void);

        virtual                       ~DiScaleController(void);
        
    public:

        virtual void                  CopyTo (DiParticleController* affector);

        virtual void                  Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void                  InitParticleForEmission(DiParticle* particle);

    public:

        bool                          IsSinceStartSystem(void) const {return mSinceStartSystem;}

        void                          SetSinceStartSystem(bool sinceStartSystem){mSinceStartSystem = sinceStartSystem;}

        void                          SetScale(const DiVec2& begin, const DiVec2& med, const DiVec2& end)
        {
            mBeginScale = begin;
            mMedScale = med;
            mEndScale = end;
        }

        void                          SetScale(const DiVec2& scale)
        {
            mBeginScale = scale;
            mMedScale = scale;
            mEndScale = scale;
        }

        void                          SetScale(const DiVec2& begin, const DiVec2& end)
        {
            mBeginScale = begin;
            mMedScale = (begin + end) * 0.5f;
            mEndScale = end;
        }

        void                         SetBeginScale(const DiVec2& scale){ mBeginScale = scale; }

        void                         SetMedianScale(const DiVec2& scale){ mMedScale = scale; }

        void                         SetEndScale(const DiVec2& scale){ mEndScale = scale; }

        DiVec2                       GetBeginScale() { return mBeginScale; }

        DiVec2                       GetMedianScale() { return mMedScale; }

        DiVec2                       GetEndScale() { return mEndScale; }

    protected:

        float                        CalculateScale(DiDynamicAttribute* dynScale, DiParticle* particle);

    protected:

        DiDynamicAttributeFactory    mDynamicAttributeFactory;

        DiDynamicAttributeHelper     mDynamicAttributeHelper;

        bool                         mSinceStartSystem;

        DiVec2                       mBeginScale{ 0, 0 };

        DiVec2                       mMedScale{ 0, 0 };

        DiVec2                       mEndScale{ 0, 0 };
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiScaleControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiScaleControllerFactory(void) {}

        virtual    ~DiScaleControllerFactory(void) {}

        DiString   GetControllerType(void) const
        {
            return "Scale";
        }

        DiParticleController* CreateController(void)
        {
            return CreateControllerImpl<DiScaleController>();
        }
    };
}

#endif // ScaleController_h__
