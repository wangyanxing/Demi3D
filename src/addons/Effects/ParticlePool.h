
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

#ifndef ParticlePool_h__
#define ParticlePool_h__

#include "FxPrerequisites.h"
#include "Particle.h"
#include "ParticlePoolBase.h"

namespace Demi
{
    class DEMI_FX_API DiParticlePool
    {
    public:

        DiParticlePool(void) : mLatestParticle(0){};

        virtual ~DiParticlePool (void);

        void                             SetParentElement(DiParticleElement* parentTechnique);

        bool                             IsEmpty(void);

        bool                             IsEmpty(const DiParticle::ParticleType particleType);

        size_t                           GetSize(void);

        size_t                           GetSize(const DiParticle::ParticleType particleType);

        void                             InitialisePool(void);

        void                             IncreasePool (const DiParticle::ParticleType particleType, 
                                            size_t size,
                                            DiParticleElement* technique);

        void                             DestroyParticles(const DiParticle::ParticleType particleType);

        void                             DestroyAllVisualParticles(void);

        DiParticle*                      ReleaseParticle (const DiParticle::ParticleType particleType, const DiString& name);

        void                             ReleaseAllParticles (void);

        void                             LockLatestParticle (void);

        void                             LockAllParticles (void);

        void                             ResetIterator(void);

        DiParticle*                      GetFirst(void);

        DiParticle*                      GetNext(void);

        DiParticle*                      GetFirst(const DiParticle::ParticleType particleType);

        DiParticle*                      GetNext(const DiParticle::ParticleType particleType);

        bool                             End(void);

        bool                             End(const DiParticle::ParticleType particleType);


        DiParticlePoolBase<DiVisualParticle>::PoolList& GetVisualParticlesList(void)
        {
            return mVisualParticlesPool.GetActiveElementsList();
        }

    protected:
        DiParticleElement* mParentTechnique;


        DiParticlePoolBase<DiVisualParticle>    mVisualParticlesPool;

        DiVector<DiVisualParticle*>             mVisualParticles;

        void            IncreaseVisualParticlePool(size_t size);

        DiParticle*     mLatestParticle;

    };
}

#endif // ParticlePool_h__
