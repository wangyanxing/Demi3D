
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

#include "FxPch.h"
#include "BaseForceController.h"
#include "ParticleElement.h"

namespace Demi
{
    const DiVec3 DiBaseForceController::DEFAULT_FORCE_VECTOR = DiVec3::ZERO;
    const DiBaseForceController::ForceApplication DiBaseForceController::DEFAULT_FORCE_APPL = DiBaseForceController::FA_ADD;

    DiBaseForceController::DiBaseForceController(void) : 
    DiParticleController(),
        mForceVector(DEFAULT_FORCE_VECTOR),
        mScaledVector(DiVec3::ZERO),
        mForceApplication(DEFAULT_FORCE_APPL)
    {
    }
    
    void DiBaseForceController::CopyTo (DiParticleController* affector)
    {
        DiParticleController::CopyTo(affector);

        DiBaseForceController* baseForceAffector = static_cast<DiBaseForceController*>(affector);
        baseForceAffector->mForceVector = mForceVector;
        baseForceAffector->mForceApplication = mForceApplication;
    }
    
    const DiVec3& DiBaseForceController::GetForceVector(void) const
    {
        return mForceVector;
    }
    
    void DiBaseForceController::SetForceVector(const DiVec3& forceVector)
    {
        mForceVector = forceVector;
    }
    
    DiBaseForceController::ForceApplication DiBaseForceController::GetForceApplication(void) const
    {
        return mForceApplication;
    }
    
    void DiBaseForceController::SetForceApplication(DiBaseForceController::ForceApplication forceApplication)
    {
        mForceApplication = forceApplication;
    }
}

