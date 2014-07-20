
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

#ifndef Particle_h__
#define Particle_h__

#include "K2Prerequisites.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   pragma warning(disable:4800)
#endif

namespace Demi
{
    class DEMI_K2_API DiParticle
    {
    public:

        static float DEFAULT_TTL;
        static float DEFAULT_MASS;
        enum ParticleType
        {
            PT_VISUAL,

            PT_UNUSED
        };

        enum ReservedParticleEventFlags
        {
            PEF_EXPIRED = 1<<0,
            PEF_EMITTED = 1<<1,
            PEF_COLLIDED = 1<<2
        };

    public:

        DiParticle():
          mMarkedForEmission(false),
              position(DiVec3::ZERO),
              mDerivedPosition(DiVec3::ZERO),
              direction(DiVec3::ZERO),
              timeToLive(DEFAULT_TTL),
              totalTimeToLive(DEFAULT_TTL),
              timeFraction(0.0f),
              particleType(PT_VISUAL),
              mass(DEFAULT_MASS),
              mEventFlags(0),
              mEnabled(true),
              mFreezed(false),
              mOriginalEnabled(true),
              mOriginalEnabledSet(false),
              originalPosition(DiVec3::ZERO),
              latestPosition(DiVec3::ZERO),
              originalDirection(DiVec3::ZERO),
              originalDirectionLength(0.0f),
              originalScaledDirectionLength(0.0f),
              originalVelocity(0.0f),
              parentEmitter(0),
              visualData(0){}

          virtual ~DiParticle();

    public:

        DiParticleEmitter*    parentEmitter;

        DiVec3                position;

        DiVec3                direction;

        // some special controller (gravity) may use this
        float                 mass;

        float                 timeToLive;

        float                 totalTimeToLive;

        // usually used by the controllers
        float                 timeFraction;

        ParticleType          particleType;

        DiIVisualData*        visualData;

        DiVec3                originalPosition;

        DiVec3                originalDirection;

        float                 originalVelocity;

        float                 originalDirectionLength;

        float                 originalScaledDirectionLength;

        DiVec3                latestPosition;

    public:

        inline bool           IsMarkedForEmission(void) const { return mMarkedForEmission; }

        inline void           SetMarkedForEmission(bool markedForEmission) { mMarkedForEmission = markedForEmission; }

        virtual void          InitForEmission(void);

        virtual void          InitForExpiration(DiParticleElement* technique, float timeElapsed);

        virtual bool          IsEnabled(void) const;

        virtual void          SetEnabled(bool enabled);

        void                  SetOriginalEnabled(bool originalEnabled);

        bool                  GetOriginalEnabled(void) const;

        bool                  IsFreezed(void) const;

        void                  SetFreezed(bool freezed);

        inline void           SetEventFlags(uint32 flags) { mEventFlags = flags; }

        inline void           AddEventFlags(uint32 flags) { mEventFlags |= flags; }

        inline void           RemoveEventFlags(uint32 flags) { mEventFlags &= ~flags; }

        inline uint32         GetEventFlags(void) const {return mEventFlags;}

        inline bool           HasEventFlags(uint32 flags) const {return mEventFlags & flags;}

        virtual void          Process(DiParticleElement* technique, float timeElapsed);

        float                 CalculateVelocity(void) const;

        virtual void          CopyTo (DiParticle* particle);

    protected:

        uint32                mEventFlags;

        bool                  mMarkedForEmission;

        bool                  mEnabled;

        bool                  mFreezed;

        bool                  mOriginalEnabled;

        bool                  mOriginalEnabledSet;

        DiVec3                mDerivedPosition;
    };
}

#endif // Particle_h__
