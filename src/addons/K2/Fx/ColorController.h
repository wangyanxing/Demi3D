
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

#ifndef ColorController_h__
#define ColorController_h__

#include "K2Prerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_K2_API DiColorController : public DiParticleController
    {
    public:
        typedef DiMap<float, DiColor> ColorMap;
        typedef DiMap<float, DiColor>::iterator ColourMapIterator;
        enum ColourOperation
        {
            CAO_MULTIPLY,
            CAO_SET
        };

        static const ColourOperation DEFAULT_COLOUR_OPERATION;

        DiColorController(void);

        virtual                     ~DiColorController(void) {}

    public:

        void                        AddColour(float timeFraction, const DiColor& colour);

        ColorMap&                   GetTimeAndColour(void);
        
        void                        SetTimeAndColour(const ColorMap& colors);

        void                        ClearColourMap (void);

        inline ColourMapIterator    FindNearestColourMapIterator(float timeFraction);

        const ColourOperation&      GetColourOperation (void) const;

        void                        SetColourOperation (const ColourOperation& colourOperation);

        virtual void                Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed);

        virtual void                CopyTo(DiParticleController* affector);

    protected:

        ColorMap                    mColourMap;

        ColourOperation             mColourOperation;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiColorControllerFactory : public DiParticleControllerFactory
    {
    public:
        DiColorControllerFactory(void) {}

        virtual                     ~DiColorControllerFactory(void) {}

        DiString                    GetControllerType(void) const
        {
            static DiString type = "Color";
            return type;
        }

        DiParticleController*       CreateController(void)
        {
            return CreateControllerImpl<DiColorController>();
        }
    };
}

#endif // ColorController_h__
