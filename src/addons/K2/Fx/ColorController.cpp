
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

#include "K2Pch.h"
#include "ColorController.h"
#include "ParticleElement.h"
#include "VisualParticle.h"
#include "ParticleSystem.h"

namespace Demi
{
    const DiColorController::ColourOperation DiColorController::DEFAULT_COLOUR_OPERATION = DiColorController::CAO_SET;

    DiColorController::DiColorController(void) : 
        DiParticleController(),
        mColourOperation(DEFAULT_COLOUR_OPERATION)
    {
    }
    
    void DiColorController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiColorController* colourAffector = static_cast<DiColorController*>(affector);
        colourAffector->mColourMap        = mColourMap;
        colourAffector->mColourOperation  = mColourOperation;
    }
    
    const DiColorController::ColourOperation& DiColorController::GetColourOperation (void) const
    {
        return mColourOperation;
    }
    
    void DiColorController::SetColourOperation (const DiColorController::ColourOperation& colourOperation)
    {
        mColourOperation = colourOperation;
    }
    
    void DiColorController::AddColour (float timeFraction, const DiColor& colour)
    {
        mColourMap[timeFraction] = colour;
    }
    
    DiColorController::ColorMap& DiColorController::GetTimeAndColour(void)
    {
        return mColourMap;
    }
    
    void DiColorController::SetTimeAndColour(const ColorMap& colors)
    {
        mColourMap = colors;
    }
    
    void DiColorController::ClearColourMap (void)
    {
        mColourMap.clear();
    }
    
    void DiColorController::Control(DiParticleElement* particleTechnique, DiParticle* particle, float timeElapsed)
    {
        if (mColourMap.empty())
            return;

        if (particle->particleType != DiParticle::PT_VISUAL)
            return;

        DiVisualParticle* visualParticle = static_cast<DiVisualParticle*>(particle);

        DiColor colour = DiColor::White;
        float timeFraction = (visualParticle->totalTimeToLive - visualParticle->timeToLive) / visualParticle->totalTimeToLive;

        timeFraction = particle->timeFraction;

        ColourMapIterator it1 = FindNearestColourMapIterator(timeFraction);
        ColourMapIterator it2 = it1;
        it2++;
        
        if (it2 != mColourMap.end())
            colour = it1->second + ((it2->second - it1->second) * ((timeFraction - it1->first)/(it2->first - it1->first)));
        else
            colour = it1->second;

        if (mColourOperation == CAO_SET)
            visualParticle->colour = colour;
        else
            visualParticle->colour = colour * visualParticle->originalColour;
    }
    
    DiColorController::ColourMapIterator DiColorController::FindNearestColourMapIterator(float timeFraction)
    {
        ColourMapIterator it;
        for (it = mColourMap.begin(); it != mColourMap.end(); ++it)
        {
            if (timeFraction < it->first)
            {
                if (it == mColourMap.begin())
                    return it;
                else
                    return --it;
            }
        }

        return --it;
    }
}

