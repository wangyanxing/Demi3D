
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

#ifndef ParticleEmitter_h__
#define ParticleEmitter_h__

#include "FxPrerequisites.h"
#include "Particle.h"
#include "DynamicAttribute.h"
#include "DynamicAttributeFactory.h"

namespace Demi
{
    class DEMI_FX_API DiParticleEmitter : public DiParticle
    {
    public:
        static const bool          DEFAULT_ENABLED;
        static const DiVec3        DEFAULT_POSITION;
        static const bool          DEFAULT_KEEP_LOCAL;
        static const DiVec3        DEFAULT_DIRECTION;
        static const DiQuat        DEFAULT_ORIENTATION;
        static const DiQuat        DEFAULT_ORIENTATION_RANGE_START;
        static const DiQuat        DEFAULT_ORIENTATION_RANGE_END;
        static const DiParticle::ParticleType DEFAULT_EMITS;
        static const uint16        DEFAULT_START_TEXTURE_COORDS;
        static const uint16        DEFAULT_END_TEXTURE_COORDS;
        static const uint16        DEFAULT_TEXTURE_COORDS;
        static const DiColor       DEFAULT_START_COLOUR_RANGE;
        static const DiColor       DEFAULT_END_COLOUR_RANGE;
        static const DiColor       DEFAULT_COLOUR;
        static const bool          DEFAULT_AUTO_DIRECTION;
        static const bool          DEFAULT_FORCE_EMISSION;
        static const float         DEFAULT_EMISSION_RATE;
        static const float         DEFAULT_TIME_TO_LIVE;
        static const float         DEFAULT_MASS;
        static const float         DEFAULT_VELOCITY;
        static const float         DEFAULT_DURATION;
        static const float         DEFAULT_REPEAT_DELAY;
        static const float         DEFAULT_ANGLE;
        static const float         DEFAULT_DIMENSIONS;
        static const float         DEFAULT_WIDTH;
        static const float         DEFAULT_HEIGHT;
        static const float         DEFAULT_DEPTH;

    public:

        DiParticleEmitter(void);
        
        virtual                      ~DiParticleEmitter(void);

        inline DiParticleElement*    GetParentElement(void) const {return mParentElement;};
        void                         SetParentElement(DiParticleElement* parentTechnique);

        inline const DiString&       GetEmitterType(void) const {return mEmitterType;};
        void                         SetEmitterType(const DiString& emitterType) {mEmitterType = emitterType;};

        inline const DiString&       GetName(void) const {return mName;};
        void                         SetName(const DiString& name) {mName = name;};

        inline DiDynamicAttribute*   GetDynAngle(void) const {return mDynAngle;};
        void                         SetDynAngle(DiDynamicAttribute* dynAngle);

        inline DiDynamicAttribute*   GetDynEmissionRate(void) const {return mDynEmissionRate;};
        void                         SetDynEmissionRate(DiDynamicAttribute* dynEmissionRate);

        inline DiDynamicAttribute*   GetDynTotalTimeToLive(void) const {return mDynTotalTimeToLive;};
        void                         SetDynTotalTimeToLive(DiDynamicAttribute* dynTotalTimeToLive);

        inline DiDynamicAttribute*   GetDynParticleMass(void) const {return mDynParticleMass;};
        void                         SetDynParticleMass(DiDynamicAttribute* dynParticleMass);

        inline DiDynamicAttribute*   GetDynVelocity(void) const {return mDynVelocity;};
        void                         SetDynVelocity(DiDynamicAttribute* dynVelocity);

        inline DiDynamicAttribute*   GetDynDuration(void) const {return mDynDuration;};
        void                         SetDynDuration(DiDynamicAttribute* dynDuration);
        void                         SetDynDurationSet(bool durationSet);

        inline DiDynamicAttribute*   GetDynRepeatDelay(void) const {return mDynRepeatDelay;};
        void                         SetDynRepeatDelay(DiDynamicAttribute* dynRepeatDelay);
        void                         SetDynRepeatDelaySet(bool repeatDelaySet);

        inline DiDynamicAttribute*   GetDynParticleAllDimensions(void) const {return mDynParticleAllDimensions;};
        void                         SetDynParticleAllDimensions(DiDynamicAttribute* dynParticleAllDimensions);
        void                         SetDynParticleAllDimensionsSet(bool particleAllDimensionsSet);

        inline DiDynamicAttribute*   GetDynParticleWidth(void) const {return mDynParticleWidth;};
        void                         SetDynParticleWidth(DiDynamicAttribute* dynParticleWidth);
        void                         SetDynParticleWidthSet(bool particleWidthSet);

        inline DiDynamicAttribute*   GetDynParticleHeight(void) const {return mDynParticleHeight;};
        void                         SetDynParticleHeight(DiDynamicAttribute* dynParticleHeight);
        void                         SetDynParticleHeightSet(bool particleHeightSet);

        inline DiDynamicAttribute*   GetDynParticleDepth(void) const {return mDynParticleDepth;};
        void                         SetDynParticleDepth(DiDynamicAttribute* dynParticleDepth);
        void                         SetDynParticleDepthSet(bool particleDepthSet);

        inline DiParticle::ParticleType    GetEmitsType(void) const {return mEmitsType;}
        void                        SetEmitsType(DiParticle::ParticleType emitsType) {mEmitsType = emitsType;}

        inline const DiString&      GetEmitsName(void) const {return mEmitsName;}
        void                        SetEmitsName(const DiString& emitsName){mEmitsName=emitsName;}

        virtual const DiVec3&       GetParticleDirection(void) const;

        const DiVec3&               GetOriginalParticleDirection(void) const;

        const DiQuat&               GetParticleOrientation(void) const;

        void                        SetParticleOrientation(const DiQuat& orientation);

        const DiQuat&               GetParticleOrientationRangeStart(void) const;

        void                        SetParticleOrientationRangeStart(const DiQuat& orientationRangeStart);

        const DiQuat&               GetParticleOrientationRangeEnd(void) const;

        void                        SetParticleOrientationRangeEnd(const DiQuat& orientationRangeEnd);

        virtual void                SetEnabled (bool enabled);

        bool                        IsEnabled()const{return mEnabled;}
    
        virtual void                SetParticleDirection(const DiVec3& direction);

        bool                        IsAutoDirection(void) const;

        void                        SetAutoDirection(bool autoDirection);

        bool                        IsForceEmission(void) const;

        void                        SetForceEmission(bool forceEmission);

        virtual void                InitParticlePosition(DiParticle* particle);

        virtual void                InitParticleForEmission(DiParticle* particle);

        virtual void                InitParticleDirection(DiParticle* particle);

        virtual void                InitParticleOrientation(DiParticle* particle);

        void                        GenerateAngle(DiRadian& angle);

        virtual void                InitParticleVelocity(DiParticle* particle);

        virtual void                InitParticleMass(DiParticle* particle);

        virtual void                InitParticleColour(DiParticle* particle);

        virtual void                InitParticleTextureCoords(DiParticle* particle);

        inline float                InitParticleTimeToLive(void);

        // calculate the real number of the particles which are ready to emit
        virtual unsigned short      CalculateRequestedParticles(float timeElapsed);

        // initialize the particle dimensions
        inline void                 InitParticleDimensions(DiParticle* particle);

        inline void                 InitTimeBased(void);

        virtual void                NotifyRescaled(const DiVec3& scale);

        virtual void                CopyTo (DiParticleEmitter* emitter);

        virtual void                CopyParentTo (DiParticleEmitter* emitter);

        const DiColor&              GetParticleColour(void) const;

        void                        SetParticleColour(const DiColor& particleColour);

        const uint16&               GetParticleTextureCoords(void) const;

        void                        SetParticleTextureCoords(const uint16& particleTextureCoords);

        bool                        IsKeepLocal(void) const;

        void                        SetKeepLocal(bool keepLocal);

        bool                        MakeParticleLocal(DiParticle* particle);

        DiVec3                      GetDerivedPosition();

        /** Perform initialising activities as soon as the emitter is emitted.
        */
        virtual void                InitForEmission(void);

        /** Initialise the ParticleEmitter before it is expired itself.
        */
        virtual void                InitForExpiration(DiParticleElement* element, float timeElapsed);

        virtual void                NotifyStart(void);

        /** Perform activities when a ParticleEmitter is stopped.
        */
        virtual void                NotifyStop(void);

        /** Perform activities when a ParticleEmitter is paused.
        */
        virtual void                NotifyPause(void){/* Do nothing */ };

        /** Perform activities when a ParticleEmitter is resumed.
        */
        virtual void                NotifyResume(void){/* Do nothing */ };

        void                        PushEvent(ParticleUniverseEvent& particleUniverseEvent);

    protected:

        /** Convenient function to generate events.
        */
        void                        _pushEmitterEvent(EventType eventType);

    protected:
        
        DiParticleElement*          mParentElement;

        DiVec3                      mEmitterScale;

        DiString                    mEmitterType;

        DiString                    mName;

        DiVec3                      mParticleDirection;

        DiVec3                      mOriginalParticleDirection;

        DiQuat                      mParticleOrientation;

        DiDynamicAttribute*         mDynAngle;

        DiDynamicAttribute*         mDynEmissionRate;

        DiDynamicAttribute*         mDynTotalTimeToLive;

        DiDynamicAttribute*         mDynParticleMass;

        DiDynamicAttribute*         mDynVelocity;

        DiDynamicAttribute*         mDynDuration;

        DiDynamicAttribute*         mDynRepeatDelay;

        DiDynamicAttribute*         mDynParticleAllDimensions;

        bool                        mDynParticleAllDimensionsSet;

        DiDynamicAttribute*         mDynParticleWidth;

        bool                        mDynParticleWidthSet;

        DiDynamicAttribute*         mDynParticleHeight;

        bool                        mDynParticleHeightSet;

        DiDynamicAttribute*         mDynParticleDepth;

        bool                        mDynParticleDepthSet;

        DiVec3                      mUpVector;

        float                       mRemainder;

        DiDynamicAttributeFactory   mDynamicAttributeFactory;

        DiDynamicAttributeHelper    mDynamicAttributeHelper;

        float                       mDurationRemain;

        // if we use 'duration'
        bool                        mDynDurationSet;

        // the rest value of Repeat/delay
        float                       mRepeatDelayRemain;

        // if we use 'repeat/delay'
        bool                        mDynRepeatDelaySet;

        bool                        mAutoDirection;

        bool                        mForceEmission;

        bool                        mOriginalForceEmission;

        bool                        mForceEmissionExecuted;

        bool                        mOriginalForceEmissionExecuted;

        DiColor                     mParticleColour;

        bool                        mKeepLocal;

        uint16                      mParticleTextureCoords;

        DiParticle::ParticleType    mEmitsType;

        DiString                    mEmitsName;
    };
}


#endif // ParticleEmitter_h__
