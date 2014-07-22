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

#ifndef ArenaFxProperty_h__
#define ArenaFxProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"
#include "EffectListeners.h"

namespace Demi
{
    struct ArFxProjectileConfig
    {
        void Load(const DiXMLElement& node);

        DiString name;
        float    speed{ 0 };
        float    gravity{ 0 };
        float    modelscale{ 1 };
        DiString model;
        DiString traileffect;
        DiString impacteffect;
    };
    
    /** state property
     */
    class ArFxProperty final : public ArProperty, public DiParticleSystemListener
    {
        DEFINE_PROPERTY_ID(PROPERTY_FX)

    public:

        ArFxProperty();

        ~ArFxProperty();

    public:

        void                Update(float dt) override;
        
        void                PlayProjectile(uint32 entityID, const DiString& bone);
        
        DiParticleSystemPtr PlayParticleSystem(const DiString& templateName);
        
        void                HandleParticleSystemEvent(DiParticleSystemPtr particleSystem, DiFxEvent& DiFxEvent) override;
        
    protected:
        
        void                Init();
        
    protected:
        
        DiVector<DiString>  mProjectiles;
        
        DiStrHash<ArFxProjectileConfig*> mProjectileConfigs;
        
        DiSet<DiParticleSystemPtr> mEffects;
        
        int                 mFxCount{ 0 };
    };
}

#endif
