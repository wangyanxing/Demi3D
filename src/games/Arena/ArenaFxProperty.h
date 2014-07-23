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
    /** state property
     */
    class ArFxProperty final : public ArProperty, public DiParticleSystemListener
    {
        DEFINE_PROPERTY_ID(PROPERTY_FX)

    public:

        ArFxProperty();

        ~ArFxProperty();
        
        struct Projectile
        {
            uint32 targetID{ 0 };
            DiParticleSystemPtr impactFx;
            DiParticleSystemPtr trailFx;
            DiCullNode* node{ nullptr };
            float speed{ 0 };
            DiVec3 curPos{ DiVec3::ZERO };
        };

    public:

        void                Update(float dt) override;
        
        void                PlayProjectile(uint32 entityID);
        
        DiParticleSystemPtr PlayParticleSystem(const DiString& templateName);
        
        void                HandleParticleSystemEvent(DiParticleSystemPtr particleSystem, DiFxEvent& DiFxEvent) override;
        
    protected:
        
        void                Init();
        
    protected:
        
        DiSet<DiParticleSystemPtr> mEffects;
        
        DiVector<Projectile*> mProjectiles;
        
        int                 mFxCount{ 0 };
    };
}

#endif
