
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
#include "BoxEmitter.h"
#include "ParticleElement.h"
#include "ParticleSystem.h"

namespace Demi
{
    const float DiBoxEmitter::DEFAULT_HEIGHT = 100.0f;
    const float DiBoxEmitter::DEFAULT_DEPTH = 100.0f;
    const float DiBoxEmitter::DEFAULT_WIDTH = 100.0f;

    DiBoxEmitter::DiBoxEmitter(void) : 
        DiParticleEmitter(),
        mWidth(DEFAULT_WIDTH),
        mHeight(DEFAULT_HEIGHT),
        mDepth(DEFAULT_DEPTH),
        mXRange(0.5f * DEFAULT_WIDTH),
        mYRange(0.5f * DEFAULT_HEIGHT),
        mZRange(0.5f * DEFAULT_DEPTH)
    {
    }
    
    void DiBoxEmitter::SetHeight(const float height)
    {
        mHeight = height;
        mYRange = 0.5f * height;
    }
    
    void DiBoxEmitter::SetWidth(const float width)
    {
        mWidth = width;
        mXRange = 0.5f * width;
    }
    
    void DiBoxEmitter::SetDepth(const float depth)
    {
        mDepth = depth;
        mZRange = 0.5f * depth;
    }
    
    void DiBoxEmitter::InitParticlePosition(DiParticle* particle)
    {
        DiParticleSystem* sys = mParentElement->GetParentSystem();
        if (sys)
        {
            particle->position = GetDerivedPosition() + 
                sys->GetDerivedOrientation() *
                (mEmitterScale *
                DiVec3(DiMath::SymmetricRandom() * mXRange,
                DiMath::SymmetricRandom() * mYRange,
                DiMath::SymmetricRandom() * mZRange));
        }
        else
        {
            particle->position = GetDerivedPosition() + 
                mEmitterScale *
                DiVec3(DiMath::SymmetricRandom() * mXRange,
                DiMath::SymmetricRandom() * mYRange,
                DiMath::SymmetricRandom() * mZRange);
        }

        particle->originalPosition = particle->position;
    }
    
    void DiBoxEmitter::CopyTo (DiParticleEmitter* emitter)
    {
        DiParticleEmitter::CopyTo(emitter);

        DiBoxEmitter* boxEmitter = static_cast<DiBoxEmitter*>(emitter);
        boxEmitter->mHeight = mHeight;
        boxEmitter->mWidth = mWidth;
        boxEmitter->mDepth = mDepth;
        boxEmitter->mXRange = mXRange;
        boxEmitter->mYRange = mYRange;
        boxEmitter->mZRange = mZRange;
    }
}

