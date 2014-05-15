
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

#ifndef ParticleElement_h__
#define ParticleElement_h__

#include "FxPrerequisites.h"
#include "Particle.h"
#include "Material.h"
#include "RadixSort.h"
#include "ParticlePoolBase.h"
#include "ParticlePool.h"
#include "EffectListeners.h"

namespace Demi
{
    static const DiVec3 HALFSCALE = DiVec3::UNIT_SCALE * 0.5;

    class DEMI_FX_API DiParticleElement : public DiParticle
    {
    public:
        static const bool           DEFAULT_ENABLED;
        static const DiVec3         DEFAULT_POSITION;
        static const bool           DEFAULT_KEEP_LOCAL;
        static const size_t         DEFAULT_VISUAL_PARTICLE_QUOTA;
        static const size_t         DEFAULT_EMITTED_EMITTER_QUOTA;
        static const size_t         DEFAULT_EMITTED_TECHNIQUE_QUOTA;
        static const size_t         DEFAULT_EMITTED_AFFECTOR_QUOTA;
        static const size_t         DEFAULT_EMITTED_SYSTEM_QUOTA;
        static const unsigned short DEFAULT_LOD_INDEX;
        static const float          DEFAULT_DEFAULT_WIDTH;
        static const float          DEFAULT_DEFAULT_HEIGHT;
        static const float          DEFAULT_DEFAULT_DEPTH;
        static const unsigned short DEFAULT_SPATIAL_HASHING_CELL_DIM;
        static const unsigned short DEFAULT_SPATIAL_HASHING_CELL_OVERLAP;
        static const size_t         DEFAULT_SPATIAL_HASHING_TABLE_SIZE;
        static const float          DEFAULT_SPATIAL_HASHING_INTERVAL;
        static const float          DEFAULT_MAX_VELOCITY;

        typedef DiVector<DiParticleEmitter*>            ParticleEmitterList;
        typedef ParticleEmitterList::const_iterator     ParticleEmitterIterator;

        typedef DiVector<DiParticleController*>         ParticleControllerList;
        typedef ParticleControllerList::const_iterator  ParticleAffectorIterator;

        typedef DiVector<DiElementListener*>            ElementListenerList;
        typedef ElementListenerList::const_iterator     ElementListenerIterator;

    public:

        DiParticleElement(void);

        virtual                    ~DiParticleElement(void);

    public:

        inline DiParticleSystem*  GetParentSystem(void) const {return mParentSystem;}

        void                      SetParentSystem(DiParticleSystem* parentSystem) {mParentSystem = parentSystem;}

        inline const DiString&    GetName(void) const {return mName;}

        void                      SetName(const DiString& name) {mName = name;}

        size_t                    GetVisualParticleQuota(void) const;

        void                      SetVisualParticleQuota(size_t quota);

        size_t                    GetEmittedEmitterQuota(void) const;

        void                      SetEmittedEmitterQuota(size_t quota);

        size_t                    GetEmittedTechniqueQuota(void) const;

        void                      SetEmittedTechniqueQuota(size_t quota);

        size_t                    GetEmittedAffectorQuota(void) const;

        void                      SetEmittedAffectorQuota(size_t quota);

        size_t                    GetEmittedSystemQuota(void) const;

        void                      SetEmittedSystemQuota(size_t quota);

        const float               GetDefaultWidth(void) const;

        void                      SetDefaultWidth(const float width);

        const float               GetDefaultHeight(void) const;

        void                      SetDefaultHeight(const float height);

        const float               GetDefaultDepth(void) const;

        void                      SetDefaultDepth(const float depth);

        const DiVec3&             GetDerivedPosition(void);

        inline float              GetCameraSquareDistance(void) const {return mCameraSquareDistance;}

        void                      SetCameraSquareDistance(float cameraSquareDistance){mCameraSquareDistance = cameraSquareDistance;}

        void                      SuppressNotifyEmissionChange(bool suppress);

        const DiString&           GetMaterialName(void) const;

        const DiMaterialPtr       GetMaterial(void) const;

        void                      SetMaterialName(const DiString& materialName);

        DiParticleEmitter*        CreateEmitter (const DiString& emitterType);

        void                      AddEmitter (DiParticleEmitter* emitter);

        void                      RemoveEmitter(DiParticleEmitter* emitter);

        DiParticleEmitter*        GetEmitter (size_t index) const;

        DiParticleEmitter*        GetEmitter (const DiString& emitterName) const;

        size_t                    GetNumEmitters (void) const;

        void                      DestroyEmitter(size_t index);

        void                      DestroyEmitter(DiParticleEmitter* emitter);

        void                      DestroyAllEmitters(void);

        size_t                    GetNumEmittedEmitters(void) const;

        DiParticleController*     CreateController(const DiString& affectorType);

        void                      AddControlle(DiParticleController* affector);

        void                      RemoveController(DiParticleController* affector);

        DiParticleController*     GetController (size_t index) const;

        DiParticleController*     GetController (const DiString& affectorName) const;

        size_t                    GetNumControllers (void) const;

        void                      DestroyController(size_t index);

        void                      DestroyController(DiParticleController* affector);

        void                      DestroyAllControllers(void);

        size_t                    GetNumEmittedControllers (void) const;

        DiParticleRenderer*       GetRenderer(void) const;

        void                      SetRenderer(const DiString& rendererType);

        void                      RemoveRenderer(DiParticleRenderer* renderer);

        void                      SetRenderer(DiParticleRenderer* renderer);

        void                      DestroyRenderer(void);

        void                      UpdateBatchGroup(DiRenderBatchGroup* queue, DiCamera* cam);

        void                      SetBatchGroupID(uint8 queueId);

        void                      Prepare(void);

        void                      PrepareSystem(void);

        void                      UnprepareSystem(void);

        void                      PrepareTechnique(void);

        void                      UnprepareTechnique(void);

        void                      PrepareVisualParticles(void);

        void                      UnprepareVisualParticles(void);

        void                      PrepareRenderer(void);

        void                      UnprepareRenderer(void);

        void                      PrepareEmitters(void);

        void                      UnprepareEmitters(void);

        void                      PrepareAffectors(void);

        void                      UnprepareAffectors(void);

        void                      Update(float timeElapsed);

        // 有新的发射器添加或者删除现有发射器时触发函数
        // 触发后会对现已经发射的对象进行一次重新计算
        void                      NotifyEmissionChange(void);

        void                      NotifyAttached(DiNode* parent);

        void                      NotifyCurrentCamera(DiCamera* camera);

        void                      NotifyParticleResized(void);

        void                      NotifyStart(void);

        void                      NotifyStop(void);

        void                      NotifyPause(void);

        void                      NotifyResume(void);

        inline bool               IsExpired(DiParticle* particle, float timeElapsed);

        void                      ForceEmission(DiParticleEmitter* emitter, unsigned requested);

        void                      ForceEmission(const DiParticle::ParticleType particleType, unsigned requested);

        virtual void              CopyTo(DiParticleElement* technique);

        inline unsigned short     GetLodIndex(void) const {return m_usLodIndex;};

        void                      SetLodIndex(unsigned short lodIndex) {m_usLodIndex = lodIndex;};

        // 标记element 控制器和发射器可以被发射
        // 由于设计原因可能会废除除了visualParticle以外所有的MarkForEmission方法
        void                      MarkForEmission(void);

        void                      MarkForEmission(DiParticleEmitter* emitter, bool mark = true);

        void                      NotifyUpdateBounds(void);

        void                      ResetBounds(void);

        void                      NotifyRescaled(const DiVec3& scale);

        void                      NotifyVelocityRescaled(const float& scaleVelocity);

        inline const DiAABB&      GetWorldBoundingBox(void) const { return mWorldAABB; }

        void                      SortVisualParticles(DiCamera* camera);

        size_t                    GetNumberOfEmittedParticles(void);

        size_t                    GetNumberOfEmittedParticles(DiParticle::ParticleType particleType);

        // 使所有已经发射的粒子（活动粒子）过期，并回收进粒子池
        void                      InitAllParticlesForExpiration(void);

        // 把所有已发射的粒子重新回收进粒子池
        void                      LockAllParticles(void);

        // 把所有可见粒子重新回收进粒子池
        void                      InitVisualDataInPool(void);

        DiParticlePool*           GetParticlePool(void);

        bool                      IsKeepLocal(void) const;

        void                      SetKeepLocal(bool keepLocal);

        // 变换粒子位置为本地
        inline bool               MakeParticleLocal(DiParticle* particle);

        float                     GetMaxVelocity(void) const;
                                  
        void                      SetMaxVelocity(float maxVelocity);
        
        // 元素监听器，用于监听元素事件
        void                      AddElementListener(DiElementListener* elementListener);

        void                      RemoveElementListener(DiElementListener* elementListener);

        float                     GetParticleSystemScaleVelocity(void) const;

        // 提交事件
        void                      PushEvent(ParticleUniverseEvent& particleUniverseEvent);

    protected:

        void                      PushElementEvent(EventType eventType);

        // 按方向排序
        struct DirectionSorter
        {
            DiVec3 sortDirection;

            DirectionSorter(const DiVec3& dir);
            float operator()(DiParticle* p) const;
        };

        // 按距离配需
        struct DistanceSorter
        {
            DiVec3 sortPosition;

            DistanceSorter(const DiVec3& pos);
            float operator()(DiParticle* p) const;
        };

        // 为粒子排序
        static DiRadixSort<DiParticlePoolBase<DiVisualParticle>::PoolList, DiParticle*, float> mRadixSorter;

        void                     InitParticleForEmission(DiParticle* particle);

        virtual void             InitForEmission(void);

        virtual void             InitForExpiration(DiParticleElement* technique, float timeElapsed);

        void                     ResetMarkForEmission(void);

        void                     ProcessParticles(float timeElapsed);

        void                     ProcessControllers(DiParticle* particle, float timeElapsed, bool firstParticle);

        void                     PreProcessParticles(float timeElapsed);

        void                     PostProcessParticles(float timeElapsed);

        void                     ProcessRenderer(DiParticle* particle, float timeElapsed, bool firstParticle);

        inline void              ProcessParticleSelf(DiParticle* particle, float timeElapsed, bool firstParticle);

        void                     InitParticleForExpiration(DiParticle* particle, float timeElapsed);

        void                     EmitParticles(float timeElapsed);

        void                     ExecuteEmitParticles(DiParticleEmitter* emitter, unsigned requested, float timeElapsed);

        inline void              ProcessMotion(DiParticle* particle, float timeElapsed, bool firstParticle);

        void                     NotifyStartPooledComponents(void);

        void                     NotifyStopPooledComponents(void);

        void                     NotifyPausePooledComponents(void);

        void                     NotifyResumePooledComponents(void);

    protected:

        DiParticleSystem*        mParentSystem;

        DiString                 mName;

        bool                     mVisualParticlePoolIncreased;

        size_t                   mVisualParticleQuota;
        
        size_t                   mEmittedEmitterQuota;

        size_t                   mEmittedTechniqueQuota;

        size_t                   mEmittedAffectorQuota;

        size_t                   mEmittedSystemQuota;

        ParticleEmitterList      mEmitters;

        ParticleControllerList   mControllers;

        DiParticlePool           mPool;

        DiParticleRenderer*      mRenderer;

        bool                     mSuppressNotifyEmissionChange;

        DiString                 mMaterialName;

        float                    mDefaultWidth;
                                 
        float                    mDefaultHeight;
                                 
        float                    mDefaultDepth;
                                 
        unsigned short           m_usLodIndex;
                                 
        float                    mCameraSquareDistance;

        bool                     mPrepareController;

        bool                     mPrepareEmitter;

        DiAABB                   mWorldAABB;

        // 是否需要更新
        bool                     mUpdateWorldAABB;

        DiVec3                   mMinWorldExtend;

        DiVec3                   mMaxWorldExtend;

        bool                     mHasExtents;

        // 保持本地，决定了移动的粒子特效是否有拖尾效果
        bool                     mKeepLocal;

        DiVec3                   mParticleSystemScale;

        float                    mParticleSystemScaleVelocity;

        float                    mMaxVelocity;

        bool                     mMaxVelocitySet;

        ElementListenerList      mElementListenerList;

        // 发射粒子的记录
        size_t                   mMaxNumVisualParticles;

        size_t                   mMaxNumEmitterParticles;

        size_t                   mMaxNumTechniqueParticles;

        size_t                   mMaxNumAffectorParticles;

        size_t                   mMaxNumSystemParticles;
};
}

#endif // ParticleElement_h__
