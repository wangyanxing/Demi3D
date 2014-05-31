
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

#ifndef BaseForceController_h__
#define BaseForceController_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"
#include "ParticleControllerFactory.h"

namespace Demi
{
    class DEMI_FX_API DiBaseForceController : public DiParticleController
    {
    public:

        enum ForceApplication
        {
            FA_AVERAGE,
            FA_ADD
        };

        static const DiVec3              DEFAULT_FORCE_VECTOR;
        static const ForceApplication    DEFAULT_FORCE_APPL;

        DiBaseForceController(void);

        virtual                ~DiBaseForceController(void) {}

    public:

        virtual void         CopyTo (DiParticleController* affector);

        const DiVec3&        GetForceVector(void) const;

        void                 SetForceVector(const DiVec3& forceVector);

        ForceApplication     GetForceApplication(void) const;

        void                 SetForceApplication(ForceApplication forceApplication);

    protected:
        DiVec3               mForceVector;

        DiVec3               mScaledVector;

        ForceApplication     mForceApplication;
    };
}

#endif // BaseForceController_h__
