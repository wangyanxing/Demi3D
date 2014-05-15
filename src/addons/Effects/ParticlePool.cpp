
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
#include "ParticlePool.h"
#include "VisualParticle.h"

namespace Demi
{
    DiParticlePool::~DiParticlePool (void)
    {
        DestroyAllVisualParticles();
    }
    
    void DiParticlePool::DestroyParticles(const DiParticle::ParticleType particleType)
    {
        switch(particleType)
        {
            case DiParticle::PT_VISUAL:
                DestroyAllVisualParticles();
                break;
        }
    }
    
    void DiParticlePool::DestroyAllVisualParticles(void)
    {
        DiVector<DiVisualParticle*>::iterator it;
        for (it = mVisualParticles.begin(); it != mVisualParticles.end(); ++it)
        {
            DI_DELETE *it;
        }
        mVisualParticles.clear();
        mVisualParticlesPool.Clear();
    }
    
    void DiParticlePool::SetParentElement(DiParticleElement* parentTechnique)
    {
        mParentTechnique = parentTechnique;
    }
    
    bool DiParticlePool::IsEmpty(void)
    {
        return mVisualParticlesPool.IsEmpty();
    }
    
    bool DiParticlePool::IsEmpty(const DiParticle::ParticleType particleType)
    {
        switch(particleType)
        {
            case DiParticle::PT_VISUAL:
                return mVisualParticlesPool.IsEmpty();
                break;
        }

        return true;
    }
    
    size_t DiParticlePool::GetSize(void)
    {
        return mVisualParticlesPool.GetSize();
    }
    
    size_t DiParticlePool::GetSize(const DiParticle::ParticleType particleType)
    {
        switch(particleType)
        {
            case DiParticle::PT_VISUAL:
                return mVisualParticlesPool.GetSize();
                break;
        }

        return 0;
    }
    
    void DiParticlePool::IncreaseVisualParticlePool(size_t size)
    {
        size_t oldSize = mVisualParticles.size();
        if (size < oldSize)
        {
            return;
        }

        DiVisualParticle* particle = 0;
        for (size_t i = oldSize; i < size; i++)
        {
            particle = DI_NEW DiVisualParticle();
            mVisualParticlesPool.AddElement(particle);
            mVisualParticles.push_back(particle);
        }
    }
    
    void DiParticlePool::InitialisePool(void)
    {
    }
    
    void DiParticlePool::IncreasePool (const DiParticle::ParticleType particleType, 
        size_t size, 
        DiParticleElement* element)
    {
        switch(particleType)
        {
            case DiParticle::PT_VISUAL:
                IncreaseVisualParticlePool(size);
                break;
        }
        ResetIterator();
    }
    
    DiParticle* DiParticlePool::ReleaseParticle (const DiParticle::ParticleType particleType, const DiString& name)
    {
        switch(particleType)
        {
            case DiParticle::PT_VISUAL:
                return mVisualParticlesPool.ReleaseElement();
                break;
        }
        
        return 0;
    }
    
    void DiParticlePool::ReleaseAllParticles (void)
    {
        mVisualParticlesPool.ReleaseAllElements();
        
        ResetIterator();
    }
    
    void DiParticlePool::LockLatestParticle (void)
    {
        if (mLatestParticle)
        {
            switch(mLatestParticle->particleType)
            {
                case DiParticle::PT_VISUAL:
                    mVisualParticlesPool.LockLatestElement();
                    break;
            }
        }
    }
    
    void DiParticlePool::LockAllParticles (void)
    {
        mVisualParticlesPool.LockAllElements();
        ResetIterator();
    }
    
    void DiParticlePool::ResetIterator(void)
    {
        mVisualParticlesPool.ResetIterator();
        mLatestParticle = 0;
    }
    
    DiParticle* DiParticlePool::GetFirst(void)
    {
        ResetIterator();
        mLatestParticle = mVisualParticlesPool.GetFirst();
        return mLatestParticle;
    }
    
    DiParticle* DiParticlePool::GetNext(void)
    {
        if (!mLatestParticle)
        {
            return 0;
        }

        switch (mLatestParticle->particleType)
        {
            case DiParticle::PT_VISUAL:
                {
                    mLatestParticle = mVisualParticlesPool.GetNext();
                }
                break;
        }
        return mLatestParticle;
    }
    
    DiParticle* DiParticlePool::GetFirst(const DiParticle::ParticleType particleType)
    {
        ResetIterator();
        switch (particleType)
        {
            case DiParticle::PT_VISUAL:
                mLatestParticle = mVisualParticlesPool.GetFirst();
                break;
        }

        return mLatestParticle;
    }
    
    DiParticle* DiParticlePool::GetNext(const DiParticle::ParticleType particleType)
    {
        mLatestParticle = 0;
        switch (particleType)
        {
            case DiParticle::PT_VISUAL:
                mLatestParticle = mVisualParticlesPool.GetNext();
                break;
        }

        return mLatestParticle;
    }
    
    bool DiParticlePool::End(void)
    {
        return mVisualParticlesPool.End();
    }
    
    bool DiParticlePool::End(const DiParticle::ParticleType particleType)
    {
        switch (particleType)
        {
            case DiParticle::PT_VISUAL:
                return mVisualParticlesPool.End();
                break;
        }

        return End();
    }
}

