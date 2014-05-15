
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

#ifndef ParticleControllerFactory_h__
#define ParticleControllerFactory_h__

#include "FxPrerequisites.h"
#include "ParticleController.h"

namespace Demi
{
    class DEMI_FX_API DiParticleControllerFactory
    {
    public:
        DiParticleControllerFactory(void) {}

        virtual                         ~DiParticleControllerFactory(void) {}

        virtual DiString                GetControllerType(void) const = 0;

        virtual DiParticleController*   CreateController(void) = 0;

        void                            DestroyController (DiParticleController* ct)
        {
            SAFE_DELETE(ct);
        }

    protected:

        template <class T>
        DiParticleController*           CreateControllerImpl(void)
        {
            DiParticleController* ct = DI_NEW T();
            ct->SetControllerType(GetControllerType());
            return ct;
        };
    };
}

#endif // ParticleControllerFactory_h__
