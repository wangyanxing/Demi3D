
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

#ifndef CircleEmitter_h__
#define CircleEmitter_h__

#include "K2Prerequisites.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterFactory.h"

namespace Demi
{
    class DEMI_K2_API DiCircleEmitter : public DiParticleEmitter
    {

    public:
        static const float     DEFAULT_RADIUS;
        
        static const float     DEFAULT_STEP;
        
        static const float     DEFAULT_ANGLE;
        
        static const bool      DEFAULT_RANDOM;

        static const DiVec3    DEFAULT_NORMAL;

        DiCircleEmitter(void);

        virtual            ~DiCircleEmitter(void) {}

    public:

        const float        GetRadius(void) const;

        void               SetRadius(const float radius);

        const float        GetCircleAngle(void) const;

        void               SetCircleAngle(const float circleAngle);

        const float        GetStep(void) const;

        void               SetStep(const float step);

        const bool         IsRandom(void) const;

        void               SetRandom(const bool random);

        const DiQuat&      GetOrientation(void) const;

        const DiVec3&      GetNormal(void) const;

        void               SetNormal(const DiVec3 &normal); 

        void               NotifyStart(void);

        virtual void       InitParticlePosition(DiParticle* particle);

        virtual void       InitParticleDirection(DiParticle* particle);

        virtual void       CopyTo (DiParticleEmitter* emitter);

    protected:
        float              mRadius;

        float              mCircleAngle;
        
        float              mOriginalCircleAngle;
        
        float              mStep;
        
        float              mX;
        
        float              mZ;
        
        bool               mRandom;
        
        DiQuat             mOrientation;
        
        DiVec3             mNormal;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiCircleEmitterFactory : public DiParticleEmitterFactory
    {
    public:
        DiCircleEmitterFactory(void) {}

        virtual             ~DiCircleEmitterFactory(void) {}

        DiString            GetEmitterType(void) const
        {
            static DiString type = "Circle";
            return type;
        }

        DiParticleEmitter*  CreateEmitter(void)
        {
            return CreateEmitterImpl<DiCircleEmitter>();
        }
    };
}

#endif // CircleEmitter_h__
