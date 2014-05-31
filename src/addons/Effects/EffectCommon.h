
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

#ifndef FxEffectCommon_h__
#define FxEffectCommon_h__

#include "FxPrerequisites.h"
#include "Str.h"

namespace Demi
{
    enum EventType
    {
        PU_EVT_SYSTEM_ATTACHING,
        PU_EVT_SYSTEM_ATTACHED,    
        PU_EVT_SYSTEM_PREPARING,
        PU_EVT_SYSTEM_PREPARED,    
        PU_EVT_SYSTEM_STARTING,    
        PU_EVT_SYSTEM_STARTED,    
        PU_EVT_SYSTEM_STOPPING,    
        PU_EVT_SYSTEM_STOPPED,    
        PU_EVT_SYSTEM_PAUSING,    
        PU_EVT_SYSTEM_PAUSED,    
        PU_EVT_SYSTEM_RESUMING,    
        PU_EVT_SYSTEM_RESUMED,    
        PU_EVT_SYSTEM_DELETING,    
        PU_EVT_LOD_TRANSITION,    
        PU_EVT_EMITTER_STARTED,    
        PU_EVT_EMITTER_STOPPED,    
        PU_EVT_NO_PARTICLES_LEFT
    };

    enum ComponentType
    {
        CT_VISUAL_PARTICLE,
        CT_SYSTEM,
        CT_TECHNIQUE,
        CT_EMITTER,
        CT_AFFECTOR,
        CT_OBSERVER
    };

    struct ParticleUniverseEvent
    {
        EventType             eventType;
        
        ComponentType         componentType;
        
        DiString              componentName;

        DiParticleElement*    technique;
        
        DiParticleEmitter*    emitter;
    };
}

#endif // EffectCommon_h__
