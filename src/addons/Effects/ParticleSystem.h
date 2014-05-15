
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

#ifndef ParticleSystem_h__
#define ParticleSystem_h__


#include "FxPrerequisites.h"
#include "TransformUnit.h"
#include "Particle.h"

namespace Demi
{
    class DEMI_FX_API DiParticleSystem : public DiTransformUnit, public DiParticle
    {
    public:
        static const bool        DEFAULT_KEEP_LOCAL;
        static const float       DEFAULT_ITERATION_INTERVAL;
        static const float       DEFAULT_FIXED_TIMEOUT;
        static const float       DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT;
        static const bool        DEFAULT_SMOOTH_LOD;
        static const float       DEFAULT_FAST_FORWARD_TIME;
        static const DiString    DEFAULT_MAIN_CAMERA_NAME;
        static const float       DEFAULT_SCALE_VELOCITY;
        static const float       DEFAULT_SCALE_TIME;
        static const DiVec3      DEFAULT_SCALE;
        static const bool        DEFAULT_TIGHT_BOUNDINGBOX;

        enum ParticleSystemState
        {
            PSS_PREPARED,
            PSS_STARTED,
            PSS_STOPPED,
            PSS_PAUSED
        };

        typedef DiVector<float> LodDistanceList;
        typedef LodDistanceList::iterator LodDistanceIterator;
        typedef LodDistanceList::const_iterator LodDistanceConstIterator;

        typedef DiVector<DiParticleElement*> ParticleTechniqueList;
        typedef ParticleTechniqueList::iterator ParticleTechniqueIterator;
        typedef ParticleTechniqueList::const_iterator ParticleTechniqueConstIterator;

        typedef DiVector<DiParticleSystemListener*>::iterator ParticleSystemListenerIterator;
        typedef DiVector<DiParticleSystemListener*> ParticleSystemListenerList;

        DiParticleSystem(const DiString& name);

        virtual ~DiParticleSystem(void);

        DiEffectManager*        GetEffectManager();

        virtual const DiVec3&   GetDerivedPosition(void);

        virtual const DiQuat&   GetDerivedOrientation(void) const;

        const DiQuat&           GetLatestOrientation(void) const;

        bool                    HasRotatedBetweenUpdates(void) const;

        void                    RotationOffset(DiVec3& pos);

        inline bool             IsSmoothLod(void) const { return mSmoothLod; };

        void                    SetSmoothLod (bool smoothLod) {mSmoothLod = smoothLod;};

        inline float            GetTimeElapsedSinceStart(void) const {return mTimeElapsedSinceStart;};

        DiParticleSystem&       operator=(const DiParticleSystem& ps);

        DiParticleElement*      CreateElement(void);

        void                    AddElement(DiParticleElement* technique);

        void                    RemoveElement(DiParticleElement* technique);

        DiParticleElement*      GetElement(size_t index) const;

        DiParticleElement*      GetElement(const DiString& name) const;

        size_t                  GetNumElements(void) const;

        void                    DestroyElement(DiParticleElement* particleTechnique);

        void                    DestroyElement(size_t index);

        void                    DestroyAllTechniques(void);

        void                    NotifyAttached(DiNode* parent);

        virtual void            NotifyCurrentCamera(DiCamera* cam);

        const DiAABB&           GetBoundingBox(void) const;

        inline virtual void     CullingUpdate(DiRenderBatchGroup*, DiCamera*);

        virtual void            SetBatchGroup(DiBatchGroupType gt);

        void                    Update(float timeElapsed);

        size_t                  UpdateTechniques(float timeElapsed);

        const float             GetNonVisibleUpdateTimeout(void) const;

        void                    SetNonVisibleUpdateTimeout(float timeout);

        void                    Prepare(void);

        void                    Start(void);

        void                    Start(float stopTime);

        void                    StartAndStopFade(float stopTime);

        void                    Stop(void);

        void                    Stop(float stopTime);

        void                    StopFade(void);

        void                    StopFade(float stopTime);

        void                    Pause(void);

        void                    Pause(float pauseTime);

        void                    Resume(void);

        ParticleSystemState     GetState(void) const { return mState; };

        const float             GetFastForwardTime(void) const;

        const float             GetFastForwardInterval(void) const;

        void                    SetFastForward(float time, float interval);

        DiCamera*               GetMainCamera(void);

        void                    SetMainCamera(DiCamera* camera);

        DiCamera*               GetCurrentCamera(void);

        void                    FastForward(void);

        const LodDistanceList&  GetLodDistances(void) const;

        void                    ClearLodDistances(void);

        void                    AddLodDistance(float distance);

        void                    SetLodDistances(const LodDistanceList& lodDistances);

        size_t                  GetNumEmittedTechniques (void) const;

        void                    MarkForEmission(void);

        void                    ResetMarkForEmission(void);

        void                    SuppressNotifyEmissionChange(bool suppress);

        void                    NotifyEmissionChange(void);

        const float             GetIterationInterval(void) const;

        void                    SetIterationInterval(const float iterationInterval);

        const float             GetFixedTimeout(void) const;
        
        void                    SetFixedTimeout(const float fixedTimeout);

        void                    SetBoundsAutoUpdated(bool autoUpdate, float stopIn = 0.0f);

        void                    ResetBounds(void);

        const DiVec3&           GetScale(void) const;

        void                    SetScale(const DiVec3& scale);

        void                    NotifyRescaled(void);

        const float&            GetScaleVelocity(void) const;

        void                    SetScaleVelocity(const float& scaleVelocity);

        void                    NotifyVelocityRescaled(void);

        const float&            GetScaleTime(void) const;

        void                    SetScaleTime(const float& scaleTime);

        virtual void            InitForEmission(void);

        virtual void            InitForExpiration(DiParticleElement* technique, float timeElapsed);

        virtual void            Process(DiParticleElement* technique, float timeElapsed);

        bool                    IsKeepLocal(void) const;

        void                    SetKeepLocal(bool keepLocal);

        bool                    MakeParticleLocal(DiParticle* particle);

        bool                    HasTightBoundingBox(void) const;

        void                    SetTightBoundingBox(bool tightBoundingBox);

        void                    AddParticleSystemListener (DiParticleSystemListener* particleSystemListener);

        void                    RemoveParticleSystemListener (DiParticleSystemListener* particleSystemListener);

        void                    PushEvent(ParticleUniverseEvent& particleUniverseEvent);

        float                   GetPauseTime (void) const;

        void                    SetPauseTime (float pauseTime);

        const DiString&         GetTemplateName(void) const;

        void                    SetTemplateName(const DiString& templateName);

        virtual void            SetEnabled(bool enabled);

        inline size_t           GetNumberOfEmittedParticles(void);

        inline size_t           GetNumberOfEmittedParticles(DiParticle::ParticleType particleType);
        
        virtual DiString&       GetType();

    protected:
        
        // LOD索引
        unsigned short          GetLodIndex(float distance) const;

        // 根据摄像机距离平方计算LOD索引
        unsigned short          GetLodIndexSquaredDistance(float squaredDistance) const;

        inline void             CalulateRotationOffset(void);

        void                    PushSystemEvent(EventType eventType);

        ParticleSystemState     mState;

        DiAABB                  mAABB;

        float                   mBoundingRadius;

        ParticleTechniqueList   mElements;

        float                   mTimeSinceLastVisible;

        unsigned long           mLastVisibleFrame;

        float                   mNonvisibleUpdateTimeout;

        bool                    mNonvisibleUpdateTimeoutSet;
        
        float                   mTimeElapsedSinceStart;

        LodDistanceList         mLodDistanceList;

        bool                    mSmoothLod;

        bool                    mSuppressNotifyEmissionChange;

        float                   mIterationInterval;

        bool                    mIterationIntervalSet;

        float                   mTimeSinceLastUpdate;

        float                   mFixedTimeout;

        bool                    mFixedTimeoutSet;

        bool                    mBoundsAutoUpdate;

        float                   mBoundsUpdateTime;

        float                   mOriginalBoundsUpdateTime;

        bool                    mFastForwardSet;

        bool                    mOriginalFastForwardSet;

        float                   mFastForwardTime;

        float                   mFastForwardInterval;

        DiCamera*               mCurrentCamera;

        // 缩放，影响发射器 控制器 和渲染器
        DiVec3                  mParticleSystemScale;

        float                   mParticleSystemScaleVelocity;

        float                   mParticleSystemScaleTime;

        bool                    mKeepLocal;

        bool                    mTightBoundingBox;

        ParticleSystemListenerList mParticleSystemListenerList;

        float                   mPauseTime;

        bool                    mPauseTimeSet;
        
        float                   mPauseTimeElapsed;

        DiString                mTemplateName;

        bool                    mStopFadeSet;

        DiQuat                  mLatestOrientation;

        DiQuat                  mRotationOffset;

        DiVec3                  mRotationCentre;

        bool                    mAtLeastOneParticleEmitted;

        unsigned short          mLastLodIndex;
    };
}

#endif // ParticleSystem_h__
