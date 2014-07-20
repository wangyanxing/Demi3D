
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
#include "ParticleController.h"
#include "ParticleEmitter.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"

namespace Demi
{
    const bool DiParticleController::DEFAULT_ENABLED = true;
    const DiVec3 DiParticleController::DEFAULT_POSITION = DiVec3::ZERO;
    const DiParticleController::ControlSpecial DiParticleController::DEFAULT_SPECIALISATION = DiParticleController::AFSP_DEFAULT;

    DiParticleController::DiParticleController(void) :
        DiParticle(),
        mParentElement(0),
        mName(DiString::BLANK),
        mControllerScale(DiVec3::UNIT_SCALE),
        mControllerSpecialisation(DiParticleController::AFSP_DEFAULT)
    {
        particleType = PT_UNUSED;
    };
    
    void DiParticleController::NotifyStart (void)
    {
        SetEnabled(mOriginalEnabled);
    }
    
    void DiParticleController::NotifyRescaled(const DiVec3& scale)
    {
        mControllerScale = scale;
    }
    
    void DiParticleController::InitForEmission(void)
    {
        DiParticle::InitForEmission();
        NotifyStart();
    }
    
    void DiParticleController::InitForExpiration(DiParticleElement* technique, float timeElapsed)
    {
        DiParticle::InitForExpiration(technique, timeElapsed);
        NotifyStop();
    }
    
    void DiParticleController::CopyTo(DiParticleController* affector)
    {
        CopyParentTo(affector);
    }
    
    void DiParticleController::CopyParentTo(DiParticleController* affector)
    {
        DiParticle::CopyTo(affector);

        affector->SetName(mName);
        affector->mParentElement = mParentElement;
        affector->mControllerScale = mControllerScale;
        affector->mControllerSpecialisation = mControllerSpecialisation;
        affector->mExcludedEmitters = DiList<DiString>(mExcludedEmitters); // Use copy constructor
    }
    
    void DiParticleController::SetParentElement(DiParticleElement* parentTechnique)
    {
        mParentElement = parentTechnique;
    }
    
    void DiParticleController::ProcessParticle(DiParticleElement* particleTechnique, 
        DiParticle* particle, float timeElapsed, bool firstParticle)
    {
        if (firstParticle)
        {
            FirstParticle(particleTechnique, particle, timeElapsed);
        }

        if (!mExcludedEmitters.empty() && particle->parentEmitter)
        {
            DiString emitterName = particle->parentEmitter->GetName();
            DiList<DiString>::iterator it;
            it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
            if (it != mExcludedEmitters.end())
            {
                return;
            }
        }

        Control(particleTechnique, particle, timeElapsed);
    }
    
    void DiParticleController::AddEmitterToExclude(const DiString& emitterName)
    {
        DiList<DiString>::iterator it;
        it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
        if (it == mExcludedEmitters.end())
        {
            mExcludedEmitters.push_back(emitterName);
        }
    }
    
    void DiParticleController::RemoveEmitterToExclude(const DiString& emitterName)
    {
        DiList<DiString>::iterator it;
        it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
        if (it != mExcludedEmitters.end())
        {
            mExcludedEmitters.erase(it);
        }
    }
    
    void DiParticleController::RemoveAllEmittersToExclude(void)
    {
        mExcludedEmitters.clear();
    }
    
    const DiList<DiString>& DiParticleController::GetEmittersToExclude(void) const
    {
        return mExcludedEmitters;
    }
    
    bool DiParticleController::HasEmitterToExclude(const DiString& emitterName)
    {
        DiList<DiString>::iterator it;
        it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
        return (it != mExcludedEmitters.end());
    }
    
    const DiVec3& DiParticleController::GetDerivedPosition(void)
    {
        if (mMarkedForEmission)
        {
            mDerivedPosition = position;
        }
        else
        {
            mDerivedPosition = mParentElement->GetDerivedPosition() + 
                (mParentElement->GetParentSystem()->GetDerivedOrientation() * (mControllerScale * position));
        }
        return mDerivedPosition;
    }
    
    float DiParticleController::CalculateAffectSpecialisationFactor (const DiParticle* particle)
    {
        switch (mControllerSpecialisation)
        {
        case AFSP_DEFAULT:
            return 1.0f;
            break;

        case AFSP_TTL_INCREASE:
            {
                if (particle)
                {
                    return particle->timeFraction;
                }
                else
                {
                    return 1.0f;
                }
            }
            break;

        case AFSP_TTL_DECREASE:
            {
                if (particle)
                {
                    return 1.0f - particle->timeFraction;
                }
                else
                {
                    return 1.0f;
                }
            }
            break;

        default:
            return 1.0f;
            break;
        }
    }

    void DiParticleController::SetControllerSpecialisation( const ControlSpecial& affectSpecialisation )
    {
        mControllerSpecialisation = affectSpecialisation;
    }

}

