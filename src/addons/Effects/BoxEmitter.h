
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

#ifndef BoxEmitter_h__
#define BoxEmitter_h__

#include "FxPrerequisites.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterFactory.h"

namespace Demi
{
    class DEMI_FX_API DiBoxEmitter : public DiParticleEmitter
    {
    public:
        static const float DEFAULT_WIDTH;
        static const float DEFAULT_HEIGHT;
        static const float DEFAULT_DEPTH;

        DiBoxEmitter(void);

        virtual           ~DiBoxEmitter(void) {}

        float             GetHeight(void) const {return mHeight;}

        void              SetHeight(const float height);

        float             GetWidth(void) const {return mWidth;}

        void              SetWidth(const float width);

        float             GetDepth(void) const {return mDepth;}

        void              SetDepth(const float depth);

        virtual void      InitParticlePosition(DiParticle* particle);

        virtual void      CopyTo (DiParticleEmitter* emitter);

    protected:
        float             mHeight;
        
        float             mWidth;
        
        float             mDepth;

        float             mXRange;

        float             mYRange;
        
        float             mZRange;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiBoxEmitterFactory : public DiParticleEmitterFactory
    {
    public:
        DiBoxEmitterFactory(void) {}

        virtual             ~DiBoxEmitterFactory(void) {}

        DiString            GetEmitterType(void) const
        {
            static DiString type = "Box";
            return type;
        }

        DiParticleEmitter*  CreateEmitter(void)
        {
            return CreateEmitterImpl<DiBoxEmitter>();
        }
    };
}

#endif // BoxEmitter_h__
