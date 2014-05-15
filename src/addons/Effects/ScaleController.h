
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
        static const float             DEFAULT_X_SCALE;
        static const float             DEFAULT_Y_SCALE;
        static const float             DEFAULT_Z_SCALE;
        static const float             DEFAULT_XYZ_SCALE;

        DiScaleController(void);

        virtual                        ~DiScaleController(void);
        
    public:

        virtual void                 CopyTo (DiParticleController* affector);

        virtual void                 Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        DiDynamicAttribute*            GetDynScaleX(void) const {return mDynScaleX;}

        void                         SetDynScaleX(DiDynamicAttribute* dynScaleX);

        void                         ResetDynScaleX(bool resetToDefault = true);

        DiDynamicAttribute*            GetDynScaleY(void) const {return mDynScaleY;}

        void                         SetDynScaleY(DiDynamicAttribute* dynScaleY);

        void                         ResetDynScaleY(bool resetToDefault = true);

        DiDynamicAttribute*            GetDynScaleZ(void) const {return mDynScaleZ;}

        void                         SetDynScaleZ(DiDynamicAttribute* dynScaleZ);

        void                         ResetDynScaleZ(bool resetToDefault = true);

        DiDynamicAttribute*            GetDynScaleXYZ(void) const {return mDynScaleXYZ;}

        void                         SetDynScaleXYZ(DiDynamicAttribute* dynScaleXYZ);

        void                         ResetDynScaleXYZ(bool resetToDefault = true);

        bool                         IsSinceStartSystem(void) const {return mSinceStartSystem;}

        void                         SetSinceStartSystem(bool sinceStartSystem){mSinceStartSystem = sinceStartSystem;}

    protected:

        float                        CalculateScale(DiDynamicAttribute* dynScale, DiParticle* particle);

    protected:
        DiDynamicAttribute*         mDynScaleX;
        DiDynamicAttribute*         mDynScaleY;
        DiDynamicAttribute*         mDynScaleZ;
        DiDynamicAttribute*         mDynScaleXYZ;
        
        bool                         mDynScaleXSet;
        bool                         mDynScaleYSet;
        bool                         mDynScaleZSet;
        bool                         mDynScaleXYZSet;
        
        DiDynamicAttributeFactory    mDynamicAttributeFactory;
        
        DiDynamicAttributeHelper    mDynamicAttributeHelper;
        
        bool                        mSinceStartSystem;

        float                        mLatestTimeElapsed;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiScaleControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiScaleControllerFactory(void) {}

        virtual                        ~DiScaleControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            return "Scale";
        }

        DiParticleController*        CreateController(void)
        {
            return CreateControllerImpl<DiScaleController>();
        }
    };
}

#endif // ScaleController_h__
