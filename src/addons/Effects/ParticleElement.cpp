
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
#include "ParticleElement.h"
#include "ParticleEmitter.h"
#include "ParticleRenderer.h"
#include "ParticleController.h"
#include "ParticleSystem.h"
#include "VisualParticle.h"
#include "AssetManager.h"
#include "EffectManager.h"

namespace Demi
{
    DiRadixSort<DiParticlePoolBase<DiVisualParticle>::PoolList, DiParticle*, float> DiParticleElement::mRadixSorter;
    
    const bool                DiParticleElement::DEFAULT_ENABLED                        = true;
    const DiVec3            DiParticleElement::DEFAULT_POSITION                        = DiVec3::ZERO;
    const bool                DiParticleElement::DEFAULT_KEEP_LOCAL                    = false;
    const size_t             DiParticleElement::DEFAULT_VISUAL_PARTICLE_QUOTA         = 500;
    const size_t             DiParticleElement::DEFAULT_EMITTED_EMITTER_QUOTA         = 50;
    const size_t             DiParticleElement::DEFAULT_EMITTED_TECHNIQUE_QUOTA        = 10;
    const size_t             DiParticleElement::DEFAULT_EMITTED_AFFECTOR_QUOTA        = 10;
    const size_t             DiParticleElement::DEFAULT_EMITTED_SYSTEM_QUOTA            = 10;
    const unsigned short    DiParticleElement::DEFAULT_LOD_INDEX                    = 0;
    const float             DiParticleElement::DEFAULT_DEFAULT_WIDTH                = 50;
    const float             DiParticleElement::DEFAULT_DEFAULT_HEIGHT                = 50;
    const float             DiParticleElement::DEFAULT_DEFAULT_DEPTH                = 50;
    const unsigned short     DiParticleElement::DEFAULT_SPATIAL_HASHING_CELL_DIM     = 15;
    const unsigned short     DiParticleElement::DEFAULT_SPATIAL_HASHING_CELL_OVERLAP = 0;
    const size_t            DiParticleElement::DEFAULT_SPATIAL_HASHING_TABLE_SIZE    = 50;
    const float             DiParticleElement::DEFAULT_SPATIAL_HASHING_INTERVAL        = 0.05f;
    const float             DiParticleElement::DEFAULT_MAX_VELOCITY                    = 9999.0f;

    DiParticleElement::DiParticleElement(void) : 
        DiParticle(),
        mParentSystem(NULL),
        mName(DiString::BLANK),
        mRenderer(NULL),
        mVisualParticleQuota(DEFAULT_VISUAL_PARTICLE_QUOTA),
        mEmittedEmitterQuota(DEFAULT_EMITTED_EMITTER_QUOTA),
        mEmittedTechniqueQuota(DEFAULT_EMITTED_TECHNIQUE_QUOTA),
        mEmittedAffectorQuota(DEFAULT_EMITTED_AFFECTOR_QUOTA),
        mEmittedSystemQuota(DEFAULT_EMITTED_SYSTEM_QUOTA),
        mVisualParticlePoolIncreased(false),
        mSuppressNotifyEmissionChange(true),
        mDefaultWidth(DEFAULT_DEFAULT_WIDTH),
        mDefaultHeight(DEFAULT_DEFAULT_HEIGHT),
        mDefaultDepth(DEFAULT_DEFAULT_DEPTH),
        mMaterialName(DiString::BLANK),
        m_usLodIndex(DEFAULT_LOD_INDEX),
        mCameraSquareDistance(0),
        mPrepareController(false),
        mPrepareEmitter(false),
        mWorldAABB(),
        mUpdateWorldAABB(false),
        mMinWorldExtend(DiVec3::ZERO),
        mMaxWorldExtend(DiVec3::ZERO),
        mHasExtents(false),
        mKeepLocal(DEFAULT_KEEP_LOCAL),
        mParticleSystemScale(DiVec3::UNIT_SCALE),
        mParticleSystemScaleVelocity(1.0),
        mMaxVelocity(DEFAULT_MAX_VELOCITY),
        mMaxVelocitySet(false),
        mMaxNumVisualParticles(0),
        mMaxNumEmitterParticles(0),
        mMaxNumTechniqueParticles(0),
        mMaxNumAffectorParticles(0),
        mMaxNumSystemParticles(0)
    {
        particleType = PT_UNUSED;
        SetMaterialName("BaseWhite");
        mPool.SetParentElement(this);
    }
    
    DiParticleElement::~DiParticleElement(void)
    {
        mSuppressNotifyEmissionChange = true;
        DestroyAllEmitters();
        DestroyAllControllers();
        DestroyRenderer();
    }
    
    void DiParticleElement::InitParticleForEmission(DiParticle* particle)
    {
        if (!mElementListenerList.empty())
        {
            ElementListenerIterator it;
            ElementListenerIterator itEnd = mElementListenerList.end();
            for (it = mElementListenerList.begin(); it != itEnd; ++it)
            {
                (*it)->ParticleEmitted(this, particle);
            }
        }
    }
    
    void DiParticleElement::InitForEmission(void)
    {
        DiParticle::InitForEmission();

        NotifyStart();

        mPool.LockAllParticles();
    }
    
    void DiParticleElement::InitForExpiration(DiParticleElement* technique, float timeElapsed)
    {
        DiParticle::InitForExpiration(technique, timeElapsed);
        NotifyStop();
    }
    
    void DiParticleElement::MarkForEmission(void)
    {
        ResetMarkForEmission();

        ParticleEmitterList::iterator emitterIt;
        ParticleEmitterList::iterator emitterItEnd = mEmitters.end();

        for (emitterIt = mEmitters.begin(); emitterIt != emitterItEnd; ++emitterIt)
        {
            MarkForEmission(*emitterIt);
        }
    }
    
    void DiParticleElement::MarkForEmission(DiParticleEmitter* emitter, bool mark)
    {
    }
    
    void DiParticleElement::ResetMarkForEmission(void)
    {
        ParticleAffectorIterator affectorIt;
        ParticleAffectorIterator affectorItEnd = mControllers.end();
        for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
        {
            (*affectorIt)->SetMarkedForEmission(false);
        }
    }
    
    void DiParticleElement::NotifyUpdateBounds()
    {
        if (mPool.IsEmpty())
        {
            ResetBounds();
            mUpdateWorldAABB = false;
        }
        else
        {
            mUpdateWorldAABB = true;
        }
    }
    
    void DiParticleElement::ResetBounds()
    {
        mWorldAABB.SetNull();
        DiVec3 position = GetDerivedPosition();
        mMinWorldExtend = position;
        mMaxWorldExtend = position;
        mHasExtents = false;
    }
    
    void DiParticleElement::NotifyRescaled(const DiVec3& scale)
    {
        mParticleSystemScale = scale;

        ParticleEmitterList::iterator itEmitter;
        ParticleEmitterList::iterator itEmitterEnd = mEmitters.end();
        for (itEmitter = mEmitters.begin(); itEmitter != itEmitterEnd; ++itEmitter)
        {
            (*itEmitter)->NotifyRescaled(scale);
        }
        
        ParticleAffectorIterator itAffector;
        ParticleAffectorIterator itAffectorEnd = mControllers.end();
        for (itAffector = mControllers.begin(); itAffector != itAffectorEnd; ++itAffector)
        {
            (*itAffector)->NotifyRescaled(scale);
        }

        if (mRenderer)
        {
            mRenderer->NotifyRescaled(scale);
        }
    }
    
    void DiParticleElement::NotifyVelocityRescaled(const float& scaleVelocity)
    {
        mParticleSystemScaleVelocity = scaleVelocity;
    }
    
    size_t DiParticleElement::GetVisualParticleQuota(void) const 
    {
        return mVisualParticleQuota;
    }
    
    void DiParticleElement::SetVisualParticleQuota(size_t quota) 
    {
        mVisualParticleQuota = quota;
        mVisualParticlePoolIncreased = false;
    }
    
    size_t DiParticleElement::GetEmittedEmitterQuota(void) const
    {
        return mEmittedEmitterQuota;
    }
    
    void DiParticleElement::SetEmittedEmitterQuota(size_t quota)
    {
        mEmittedEmitterQuota = quota;
        //mParticleEmitterPoolIncreased = false;
    }
    
    size_t DiParticleElement::GetEmittedTechniqueQuota(void) const
    {
        return mEmittedTechniqueQuota;
    }
    
    void DiParticleElement::SetEmittedTechniqueQuota(size_t quota)
    {
        mEmittedTechniqueQuota = quota;
        //mParticleTechniquePoolIncreased = false;
    }
    
    size_t DiParticleElement::GetEmittedAffectorQuota(void) const
    {
        return mEmittedAffectorQuota;
    }
    
    void DiParticleElement::SetEmittedAffectorQuota(size_t quota)
    {
        mEmittedAffectorQuota = quota;
        //mParticleAffectorPoolIncreased = false;
    }
    
    size_t DiParticleElement::GetEmittedSystemQuota(void) const
    {
        return mEmittedSystemQuota;
    }
    
    void DiParticleElement::SetEmittedSystemQuota(size_t quota)
    {
        mEmittedSystemQuota = quota;
        //mParticleSystemPoolIncreased = false;
    }
    
    const float DiParticleElement::GetDefaultWidth(void) const
    {
        return mDefaultWidth;
    }
    
    void DiParticleElement::SetDefaultWidth(const float width)
    {
        mDefaultWidth = width;
        if (mRenderer)
        {
            mRenderer->NotifyDefaultDimensions(mParticleSystemScale.x * mDefaultWidth, 
                mParticleSystemScale.y * mDefaultHeight, 
                mParticleSystemScale.z * mDefaultDepth);
        }
    }
    
    const float DiParticleElement::GetDefaultHeight(void) const
    {
        return mDefaultHeight;
    }
    
    void DiParticleElement::SetDefaultHeight(const float height)
    {
        mDefaultHeight = height;
        if (mRenderer)
        {
            mRenderer->NotifyDefaultDimensions(mParticleSystemScale.x * mDefaultWidth, 
                mParticleSystemScale.y * mDefaultHeight, 
                mParticleSystemScale.z * mDefaultDepth);
        }
    }
    
    const float DiParticleElement::GetDefaultDepth(void) const
    {
        return mDefaultDepth;
    }
    
    void DiParticleElement::SetDefaultDepth(const float depth)
    {
        mDefaultDepth = depth;
        if (mRenderer)
        {
            mRenderer->NotifyDefaultDimensions(mParticleSystemScale.x * mDefaultWidth, 
                mParticleSystemScale.y * mDefaultHeight, 
                mParticleSystemScale.z * mDefaultDepth);
        }
    }
    
    void DiParticleElement::SuppressNotifyEmissionChange(bool suppress)
    {
        mSuppressNotifyEmissionChange = suppress;
    }
    
    const DiString& DiParticleElement::GetMaterialName(void) const
    {
        return mMaterialName;
    }
    
    const DiMaterialPtr DiParticleElement::GetMaterial(void) const
    {
        return DiAssetManager::GetInstance().GetAsset<DiMaterial>(mMaterialName);
    }
    
    void DiParticleElement::SetMaterialName(const DiString& materialName)
    {
        mMaterialName = materialName;
        if (mRenderer)
        {
            mRenderer->SetMaterialName(mMaterialName);
        }
    }
    
    DiParticleEmitter* DiParticleElement::CreateEmitter(const DiString& emitterType)
    {
        DI_ASSERT(emitterType != DiString::BLANK);
        DiParticleEmitter* emitter = GetParentSystem()->GetEffectManager()->CreateEmitter(emitterType);
        AddEmitter(emitter);
        return emitter;
    }
    
    void DiParticleElement::AddEmitter (DiParticleEmitter* emitter)
    {
        DI_ASSERT(emitter);
        mEmitters.push_back(emitter);
        emitter->SetParentElement(this);
        mPrepareEmitter = true;
        NotifyEmissionChange();
    }
    
    void DiParticleElement::RemoveEmitter(DiParticleEmitter* emitter)
    {
        DI_ASSERT(emitter);
        ParticleEmitterList::iterator it;
        ParticleEmitterList::iterator itEnd = mEmitters.end();
        bool notify = false;
        for (it = mEmitters.begin(); it != itEnd; ++it)
        {
            if (*it == emitter)
            {
                notify = emitter->GetEmitsName() != DiString::BLANK;

                mEmitters.erase(it);
                break;
            }
        }

        emitter->SetParentElement(0);

        if (notify)
        {
            NotifyEmissionChange();
        }
    }
    
    DiParticleEmitter* DiParticleElement::GetEmitter (size_t index) const
    {
        DI_ASSERT(index < mEmitters.size());
        return mEmitters[index];
    }
    
    DiParticleEmitter* DiParticleElement::GetEmitter (const DiString& emitterName) const
    {
        if (emitterName == DiString::BLANK)
        {
            return 0;
        }

        ParticleEmitterIterator it;
        ParticleEmitterIterator itEnd = mEmitters.end();
        for (it = mEmitters.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == emitterName)
            {
                return *it;
            }
        }
        
        return 0;
    }
    
    size_t DiParticleElement::GetNumEmitters (void) const
    {
        return mEmitters.size();
    }
    
    size_t DiParticleElement::GetNumEmittedEmitters (void) const
    {
        return 0;
    }
    
    void DiParticleElement::DestroyEmitter(DiParticleEmitter* emitter)
    {
        DI_ASSERT(emitter);
        ParticleEmitterList::iterator it;
        ParticleEmitterList::iterator itEnd = mEmitters.end();
        bool notify = false;
        for (it = mEmitters.begin(); it != itEnd; ++it)
        {
            if (*it == emitter)
            {
                notify = emitter->GetEmitsName() != DiString::BLANK;

                GetParentSystem()->GetEffectManager()->DestroyEmitter(*it);
                mEmitters.erase(it);
                break;
            }
        }

        if (notify)
        {
            NotifyEmissionChange();
        }
    }
    
    void DiParticleElement::DestroyEmitter (size_t index)
    {
        DestroyEmitter(GetEmitter(index));
    }
    
    void DiParticleElement::DestroyAllEmitters(void)
    {
        ParticleEmitterList::iterator it;
        ParticleEmitterList::iterator itEnd = mEmitters.end();
        for (it = mEmitters.begin(); it != itEnd; ++it)
        {
            GetParentSystem()->GetEffectManager()->DestroyEmitter(*it);
        }
        mEmitters.clear();
    }

    
    DiParticleController* DiParticleElement::CreateController(const DiString& affectorType)
    {
        DI_ASSERT(affectorType != DiString::BLANK && "affectorType is empty!");
        DiParticleController* affector = GetParentSystem()->GetEffectManager()->CreateController(affectorType);
        AddControlle(affector);
        return affector;
    }
    
    void DiParticleElement::AddControlle (DiParticleController* affector)
    {
        DI_ASSERT(affector);
        mControllers.push_back(affector);
        affector->SetParentElement(this);
        mPrepareController = true;
    }
    
    void DiParticleElement::RemoveController (DiParticleController* affector)
    {
        DI_ASSERT(affector);
        ParticleControllerList::iterator it;
        ParticleControllerList::iterator itEnd = mControllers.end();
        for (it = mControllers.begin(); it != itEnd; ++it)
        {
            if (*it == affector)
            {
                mControllers.erase(it);
                break;
            }
        }

        affector->SetParentElement(NULL);
    }
    
    DiParticleController* DiParticleElement::GetController (size_t index) const
    {
        DI_ASSERT(index < mControllers.size());
        return mControllers[index];
    }
    
    DiParticleController* DiParticleElement::GetController (const DiString& controllerName) const
    {
        if (controllerName == DiString::BLANK)
        {
            return 0;
        }

        ParticleAffectorIterator it;
        ParticleAffectorIterator itEnd = mControllers.end();
        for (it = mControllers.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == controllerName)
            {
                return *it;
            }
        }
        
        return 0;
    }
    
    size_t DiParticleElement::GetNumControllers (void) const
    {
        return mControllers.size();
    }
    
    size_t DiParticleElement::GetNumEmittedControllers (void) const
    {
        ParticleAffectorIterator it;
        ParticleAffectorIterator itEnd = mControllers.end();
        size_t count = 0;
        for (it = mControllers.begin(); it != itEnd; ++it)
        {
            if ((*it)->IsMarkedForEmission())
            {
                count++;
            }
        }

        return count;
    }
    
    void DiParticleElement::DestroyController(DiParticleController* affector)
    {
        DI_ASSERT(affector);
        ParticleControllerList::iterator it;
        ParticleControllerList::iterator itEnd = mControllers.end();
        for (it = mControllers.begin(); it != itEnd; ++it)
        {
            if (*it == affector)
            {
                GetParentSystem()->GetEffectManager()->DestroyController(*it);
                mControllers.erase(it);
                break;
            }
        }
    }
    
    void DiParticleElement::DestroyController (size_t index)
    {
        DestroyController(GetController(index));
    }
    
    void DiParticleElement::DestroyAllControllers(void)
    {
        ParticleAffectorIterator it;
        ParticleAffectorIterator itEnd = mControllers.end();
        for (it = mControllers.begin(); it != itEnd; ++it)
        {
            GetParentSystem()->GetEffectManager()->DestroyController(*it);
        }
        mControllers.clear();
    }

    DiParticleRenderer* DiParticleElement::GetRenderer(void) const
    {
        return mRenderer;
    }
    
    void DiParticleElement::SetRenderer(const DiString& rendererType)
    {
        if (mRenderer)
        {
            DestroyRenderer();
        }

        if (rendererType != DiString::BLANK)
        {
            mRenderer = GetParentSystem()->GetEffectManager()->CreateRenderer(rendererType);
            mRenderer->SetParentElement(this);
            mRenderer->SetRendererInitialised(false);
        }
    }
    
    void DiParticleElement::SetRenderer(DiParticleRenderer* renderer)
    {
        if (!renderer)
        {
            return;
        }

        if (mRenderer)
        {
            DestroyRenderer();
        }

        mRenderer = renderer;
        mRenderer->SetParentElement(this);
        mRenderer->SetRendererInitialised(false);
    }
    
    void DiParticleElement::RemoveRenderer(DiParticleRenderer* renderer)
    {
        mRenderer = NULL;
        renderer->SetParentElement(NULL);
    }
    
    void DiParticleElement::DestroyRenderer(void)
    {
        if (mRenderer)
        {
            GetParentSystem()->GetEffectManager()->DestroyRenderer(mRenderer);
            mRenderer = 0;
        }
    }
    
    void DiParticleElement::Update(DiCamera* cam)
    {
        if (mRenderer && mRenderer->IsRendererInitialised())
        {
            if (mEnabled || mParentSystem->IsSmoothLod())
            {
                mRenderer->Update(cam, &mPool);
            }
        }
    }
    
    void DiParticleElement::AddToBatchGroup(DiRenderBatchGroup* group)
    {
        if (mRenderer && mRenderer->IsRendererInitialised())
        {
            if (mEnabled || mParentSystem->IsSmoothLod())
            {
                mRenderer->AddToBatchGroup(group);
            }
        }
    }
    
    void DiParticleElement::SetBatchGroupID(uint8 queueId)
    {
        if (mRenderer)
        {
            mRenderer->SetBatchGroupID(queueId);
        }
    }
    
    void DiParticleElement::Prepare(void)
    {
        PrepareRenderer();
        PrepareEmitters();
        PrepareAffectors();
        PrepareTechnique();
        PrepareSystem();
        PrepareVisualParticles();

        NotifyRescaled(mParticleSystemScale);
        NotifyVelocityRescaled(mParticleSystemScaleVelocity);
    }
    
    void DiParticleElement::PrepareSystem(void)
    {
    }
    
    void DiParticleElement::UnprepareSystem(void)
    {
    }
    
    void DiParticleElement::PrepareTechnique(void)
    {
    }
    
    void DiParticleElement::UnprepareTechnique(void)
    {
    }
    
    void DiParticleElement::PrepareVisualParticles(void)
    {
        if (!mVisualParticlePoolIncreased)
        {
            mPool.IncreasePool(DiParticle::PT_VISUAL, mVisualParticleQuota, this);
            mVisualParticlePoolIncreased = true;
        }
    }
    
    void DiParticleElement::UnprepareVisualParticles(void)
    {
        mPool.DestroyParticles(DiParticle::PT_VISUAL);
        mVisualParticlePoolIncreased = false;
    }
    
    void DiParticleElement::PrepareRenderer(void)
    {
        if (mRenderer && !mRenderer->IsRendererInitialised())
        {
            mRenderer->Prepare(this);
            mRenderer->SetRendererInitialised(true);
        }
    }
    
    void DiParticleElement::UnprepareRenderer(void)
    {
        if (mRenderer && mRenderer->IsRendererInitialised())
        {
            mRenderer->Unprepare(this);
            mRenderer->SetRendererInitialised(false);
        }
    }
    
    void DiParticleElement::PrepareEmitters(void)
    {
    }
    
    void DiParticleElement::UnprepareEmitters(void)
    {
    }
    
    void DiParticleElement::PrepareAffectors(void)
    {
    }
    
    void DiParticleElement::UnprepareAffectors(void)
    {
    }
    
    void DiParticleElement::Update(float timeElapsed)
    {
        if (!mEnabled)
        {
            if (!mParentSystem->IsSmoothLod())
            {
                return;
            }
            else if (mPool.IsEmpty())
            {
                return;
            }
        }

        Prepare();

        if (mParentSystem && mParentSystem->HasTightBoundingBox())
        {
            ResetBounds();
        }

        EmitParticles(timeElapsed);

        PreProcessParticles(timeElapsed);

        ProcessParticles(timeElapsed);

        PostProcessParticles(timeElapsed);

        latestPosition = GetDerivedPosition();
    }
    
    void DiParticleElement::NotifyEmissionChange(void)
    {
        if (mSuppressNotifyEmissionChange)
        {
            return;
        }

        if (GetParentSystem())
        {
            GetParentSystem()->NotifyEmissionChange();
        }
        else
        {
            MarkForEmission();
        }
    }
    
    void DiParticleElement::NotifyAttached(DiNode* parent)
    {
        if (mRenderer && mRenderer->IsRendererInitialised())
        {
            mRenderer->NotifyAttached(parent);
        }
    }
    
    void DiParticleElement::NotifyCurrentCamera(DiCamera* camera)
    {
        if (mRenderer && mRenderer->IsRendererInitialised())
        {
            if (mRenderer->IsSorted())
            {
                SortVisualParticles(camera);
            }

            mRenderer->NotifyCurrentCamera(camera);
        }
    }
    
    DiParticleElement::DirectionSorter::DirectionSorter(const DiVec3& direction) : sortDirection(direction)
    {
    }
    
    float DiParticleElement::DirectionSorter::operator()(DiParticle* p) const
    {
        return sortDirection.dotProduct(p->position);
    }
    
    DiParticleElement::DistanceSorter::DistanceSorter(const DiVec3& position) : sortPosition(position)
    {
    }
    
    float DiParticleElement::DistanceSorter::operator()(DiParticle* p) const
    {
        return - (sortPosition - p->position).squaredLength();
    }
    
    void DiParticleElement::SortVisualParticles(DiCamera* camera)
    {
        if (mRenderer)
        {
            DiSortMode sortMode = mRenderer->GetSortMode();
            if (sortMode == SM_DIRECTION)
            {
                DiVec3 cameraDirection = camera->GetDerivedDirection();
                mRadixSorter.Sort(mPool.GetVisualParticlesList(), DirectionSorter(- cameraDirection));
            }
            else if (sortMode == SM_DISTANCE)
            {
                DiVec3 cameraPosition = camera->GetDerivedPosition();
                mRadixSorter.Sort(mPool.GetVisualParticlesList(), DistanceSorter(cameraPosition));
            }
        }
    }
    
    void DiParticleElement::NotifyParticleResized(void)
    {
        if (mRenderer)
        {
            mRenderer->NotifyParticleResized();
        }
    }
    
    void DiParticleElement::NotifyStart (void)
    {
        latestPosition = GetDerivedPosition();

        mRenderer->NotifyStart();

        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission())
                {
                    (*affectorIt)->NotifyStart();
                }
            }
        }

        NotifyStartPooledComponents();

        SetEnabled(mOriginalEnabled);
    }
    
    void DiParticleElement::NotifyStartPooledComponents(void)
    {
        if (mPool.IsEmpty())
        {
            return;
        }

        DiParticle* particle = static_cast<DiParticle*>(mPool.GetFirst());
        while (!mPool.End())
        {
            particle = static_cast<DiParticle*>(mPool.GetNext());
        }
    }
    
    void DiParticleElement::NotifyStop (void)
    {
        mRenderer->NotifyStop();

        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission())
                {
                    (*affectorIt)->NotifyStop();
                }
            }
        }

        NotifyStopPooledComponents();

        InitAllParticlesForExpiration();

        ResetBounds();
    }
    
    void DiParticleElement::NotifyStopPooledComponents(void)
    {
        if (mPool.IsEmpty())
        {
            return;
        }

        DiParticle* particle = static_cast<DiParticle*>(mPool.GetFirst());
        while (!mPool.End())
        {
            particle = static_cast<DiParticle*>(mPool.GetNext());
        }
    }
    
    void DiParticleElement::NotifyPause (void)
    {
        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission())
                {
                    (*affectorIt)->NotifyPause();
                }
            }
        }

        NotifyPausePooledComponents();
    }
    
    void DiParticleElement::NotifyPausePooledComponents(void)
    {
        if (mPool.IsEmpty())
        {
            return;
        }

        DiParticle* particle = static_cast<DiParticle*>(mPool.GetFirst());
        while (!mPool.End())
        {
            particle = static_cast<DiParticle*>(mPool.GetNext());
        }
    }
    
    void DiParticleElement::NotifyResume (void)
    {
        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission())
                {
                    (*affectorIt)->NotifyResume();
                }
            }
        }

        NotifyResumePooledComponents();
    }
    
    void DiParticleElement::NotifyResumePooledComponents(void)
    {
        if (mPool.IsEmpty())
        {
            return;
        }

        DiParticle* particle = static_cast<DiParticle*>(mPool.GetFirst());
        while (!mPool.End())
        {
            particle = static_cast<DiParticle*>(mPool.GetNext());
        }
    }
    
    void DiParticleElement::ProcessParticles(float timeElapsed)
    {
        if (mPool.IsEmpty())
        {
            return;
        }

        DiParticle* particle = mPool.GetFirst();
        bool firstParticle = true; 
        bool firstActiveParticle = true; 
        while (!mPool.End())
        {
            if (particle)
            {
                if (!IsExpired(particle, timeElapsed))
                {
                    ProcessParticleSelf(particle, timeElapsed, firstActiveParticle);
                    ProcessControllers(particle, timeElapsed, firstActiveParticle);
                    ProcessRenderer(particle, timeElapsed, firstActiveParticle);

                    firstActiveParticle = false;

                    particle->latestPosition = particle->position;

                    ProcessMotion(particle, timeElapsed, firstActiveParticle);
                }
                else
                {
                    InitParticleForExpiration(particle, timeElapsed);

                    mPool.LockLatestParticle();
                }

                if (particle->HasEventFlags(PEF_EXPIRED))
                {
                    particle->SetEventFlags(0);
                    particle->AddEventFlags(PEF_EXPIRED);
                }
                else
                {
                    particle->SetEventFlags(0);
                }

                particle->timeToLive -= timeElapsed;
            }

            firstParticle = false;
            particle = mPool.GetNext();
        }
    }

    void DiParticleElement::ProcessControllers(DiParticle* particle, float timeElapsed, bool firstParticle)
    {
        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission() && (*affectorIt)->IsEnabled())
                {
                    (*affectorIt)->ProcessParticle(this, particle, timeElapsed, firstParticle);
                }
            }
        }
    }
    
    void DiParticleElement::PreProcessParticles(float timeElapsed)
    {
        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission() && (*affectorIt)->IsEnabled())
                {
                    (*affectorIt)->PreProcessParticles(this, timeElapsed);
                }
            }
        }
        
    }    
    
    void DiParticleElement::PostProcessParticles(float timeElapsed)
    {
        mUpdateWorldAABB = false;
        if (mHasExtents)
        {
            mWorldAABB.SetExtents(mMinWorldExtend, mMaxWorldExtend);
        }

        if (!mControllers.empty())
        {
            ParticleAffectorIterator affectorIt;
            ParticleAffectorIterator affectorItEnd = mControllers.end();
            for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
            {
                if (!(*affectorIt)->IsMarkedForEmission() && (*affectorIt)->IsEnabled())
                {
                    (*affectorIt)->PostProcessParticles(this, timeElapsed);
                }
            }
        }
    }
    
    void DiParticleElement::ProcessRenderer(DiParticle* particle, float timeElapsed, bool firstParticle)
    {
        if (!mRenderer)
        {
            return;
        }

        mRenderer->ProcessParticle(this, particle, timeElapsed, firstParticle);
    }
    
    void DiParticleElement::ProcessParticleSelf(DiParticle* particle, float timeElapsed, bool firstParticle)
    {
        particle->Process(this, timeElapsed);

        if (mUpdateWorldAABB && particle->particleType == DiParticle::PT_VISUAL)
        {
            DiVisualParticle* p = static_cast<DiVisualParticle*>(particle);
            float maxDim = 0;
            DiVec3 padding = DiVec3::ZERO;
            if (p->ownDimensions)
            {
                maxDim = DiMath::Max(p->width, p->height);
                maxDim = DiMath::Max(maxDim, p->depth);
            }
            else
            {
                maxDim = DiMath::Max(mDefaultWidth, mDefaultHeight);
                maxDim = DiMath::Max(maxDim, mDefaultDepth);
            }

            padding = HALFSCALE * maxDim;
            if (mHasExtents)
            {
                mMinWorldExtend.makeFloor(p->position - padding);
                mMaxWorldExtend.makeCeil(p->position + padding);
            }
            else
            {
                mMinWorldExtend = p->position - padding;
                mMaxWorldExtend = p->position + padding;
                mHasExtents = true;
            }
        }
    }
    
    void DiParticleElement::InitParticleForExpiration(DiParticle* particle, float timeElapsed)
    {
        particle->InitForExpiration(this, timeElapsed);

        if (!mElementListenerList.empty())
        {
            ElementListenerIterator it;
            ElementListenerIterator itEnd = mElementListenerList.end();
            for (it = mElementListenerList.begin(); it != itEnd; ++it)
            {
                (*it)->ParticleExpired(this, particle);
            }
        }
    }
    
    bool DiParticleElement::IsExpired(DiParticle* particle, float timeElapsed)
    {
        bool expired = particle->timeToLive < timeElapsed;
        if (expired)
        {
            particle->AddEventFlags(DiParticle::PEF_EXPIRED);
        }

        return expired;
    }
    
    void DiParticleElement::EmitParticles(float timeElapsed)
    {
        if (!mEnabled)
        {
            return;
        }

        if (mEmitters.empty())
        {
            return;
        }

        ParticleEmitterIterator itEmitter;
        ParticleEmitterIterator itEmitterBegin = mEmitters.begin();
        ParticleEmitterIterator itEmitterEnd = mEmitters.end();
        for (itEmitter = itEmitterBegin; itEmitter != itEmitterEnd; ++itEmitter)
        {
            ExecuteEmitParticles(*itEmitter, (*itEmitter)->CalculateRequestedParticles(timeElapsed), timeElapsed);
        }
    }
    
    void DiParticleElement::ForceEmission(DiParticleEmitter* emitter, unsigned requested)
    {
        if (!mEnabled)
        {
            return;
        }

        ExecuteEmitParticles(emitter, requested, 0);
    }
    
    void DiParticleElement::ForceEmission(const DiParticle::ParticleType particleType, unsigned requested)
    {
        if (!mEnabled)
        {
            return;
        }

        if (mEmitters.empty())
        {
            return;
        }

        ParticleEmitterIterator emitterIt;
        ParticleEmitterIterator emitterItEnd = mEmitters.end();
        for (emitterIt = mEmitters.begin(); emitterIt != emitterItEnd; ++emitterIt)
        {
            if ((*emitterIt)->GetEmitsType() == particleType)
            {
                ExecuteEmitParticles(*emitterIt, requested, 0);
                return;
            }
        }
    }
    
    void DiParticleElement::ExecuteEmitParticles(DiParticleEmitter* emitter, unsigned requested, float timeElapsed)
    {
        if (!(mEnabled && emitter->IsEnabled()))
        {
            return;
        }

        float timePoint = 0.0f;
        float timeInc = timeElapsed / requested;
        for (unsigned int j = 0; j < requested; ++j)
        {
            DiParticle* particle = mPool.ReleaseParticle(emitter->GetEmitsType(), emitter->GetEmitsName());

            if (!particle)
            {
                return;
            }

            particle->InitForEmission();

            emitter->InitParticleForEmission(particle);

            particle->direction = (GetParentSystem()->GetDerivedOrientation() * particle->direction);
            particle->originalDirection = (GetParentSystem()->GetDerivedOrientation() * particle->originalDirection);

            if (!mControllers.empty())
            {
                ParticleAffectorIterator affectorIt;
                ParticleAffectorIterator affectorItEnd = mControllers.end();
                for (affectorIt = mControllers.begin(); affectorIt != affectorItEnd; ++affectorIt)
                {
                    if ((*affectorIt)->IsEnabled())
                    {
                        (*affectorIt)->InitParticleForEmission(particle);
                    }
                }
            }

            InitParticleForEmission(particle);

            particle->position += (particle->direction * mParticleSystemScaleVelocity * timePoint);

            timePoint += timeInc;
        }
    }
    
    const DiVec3& DiParticleElement::GetDerivedPosition(void)
    {
        if (mMarkedForEmission)
        {
            mDerivedPosition = position;
        }
        else
        {
            if (mParentSystem)
            {
                mDerivedPosition = mParentSystem->GetDerivedPosition() + GetParentSystem()->GetDerivedOrientation() * (mParticleSystemScale * position);
            }
        }
        return mDerivedPosition;
    }
    
    void DiParticleElement::ProcessMotion(DiParticle* particle, float timeElapsed, bool firstParticle)
    {
        if (particle->IsFreezed())
        {
            return;
        }

        if (!particle->parentEmitter->MakeParticleLocal(particle))
        {
            if (!MakeParticleLocal(particle))
            {
                GetParentSystem()->MakeParticleLocal(particle);
            }
        }

        if (GetParentSystem()->IsKeepLocal() || mKeepLocal)
        {
            if (!mRenderer->autoRotate)
            {
                GetParentSystem()->RotationOffset(particle->position);
            }
        }

        if (particle->HasEventFlags(DiParticle::PEF_EMITTED))
        {
            return;
        }

        if (mMaxVelocitySet && particle->CalculateVelocity() > mMaxVelocity)
        {
            particle->direction *= (mMaxVelocity / particle->direction.length());
        }

        particle->position += (particle->direction * mParticleSystemScaleVelocity * timeElapsed);
    }
    
    size_t DiParticleElement::GetNumberOfEmittedParticles(void)
    {
        size_t total = mPool.GetSize();
        return total;
    }
    
    size_t DiParticleElement::GetNumberOfEmittedParticles(DiParticle::ParticleType particleType)
    {
        size_t total = mPool.GetSize(particleType);
        return total;
    }
    
    void DiParticleElement::InitAllParticlesForExpiration(void)
    {
        if (!mPool.IsEmpty())
        {
            DiParticle* particle = static_cast<DiParticle*>(mPool.GetFirst());
            while (!mPool.End())
            {
                if (particle)
                {
                    InitParticleForExpiration(particle, 0);
                }
            
                particle = static_cast<DiParticle*>(mPool.GetNext());
            }
        }

        mPool.LockAllParticles();
    }
    
    void DiParticleElement::LockAllParticles(void)
    {
        mPool.LockAllParticles();
    }
    
    void DiParticleElement::InitVisualDataInPool(void)
    {
        mPool.ReleaseAllParticles();
        DiParticle* particle = static_cast<DiParticle*>(mPool.GetFirst());
        while (!mPool.End())
        {
            if (particle)
            {
                particle->visualData = 0;
            }
            particle = static_cast<DiParticle*>(mPool.GetNext());
        }
        mPool.LockAllParticles();
    }
    
    DiParticlePool* DiParticleElement::GetParticlePool(void)
    {
        return &mPool;
    }
    
    bool DiParticleElement::IsKeepLocal(void) const
    {
        return mKeepLocal;
    }
    
    void DiParticleElement::SetKeepLocal(bool keepLocal)
    {
        mKeepLocal = keepLocal;
    }
    
    bool DiParticleElement::MakeParticleLocal(DiParticle* particle)
    {
        if (!particle)
        {
            return true;
        }

        if (!mKeepLocal || HasEventFlags(DiParticle::PEF_EXPIRED))
            return false;

        DiVec3 diff = GetDerivedPosition() - latestPosition; // diff of the technique
        particle->position += diff;
        return true;
    }
    
    float DiParticleElement::GetMaxVelocity(void) const
    {
        return mMaxVelocity;
    }
    
    void DiParticleElement::SetMaxVelocity(float maxVelocity)
    {
        mMaxVelocity = maxVelocity;
        mMaxVelocitySet = true;
    }
    
    void DiParticleElement::AddElementListener (DiElementListener* elementListener)
    {
        mElementListenerList.push_back(elementListener);
    }
    
    void DiParticleElement::RemoveElementListener (DiElementListener* elementListener)
    {
        DI_ASSERT(elementListener && "DiElementListener is null!");
         ElementListenerList::iterator it;
         ElementListenerList::iterator itEnd = mElementListenerList.end();
        for (it = mElementListenerList.begin(); it != itEnd; ++it)
        {
            if (*it == elementListener)
            {
                mElementListenerList.erase(it);
                break;
            }
        }
    }
    
    float DiParticleElement::GetParticleSystemScaleVelocity(void) const
    {
        return mParticleSystemScaleVelocity;
    }
    
    void DiParticleElement::PushElementEvent(EventType eventType)
    {
        ParticleUniverseEvent evt;
        evt.eventType = eventType;
        evt.componentType = CT_TECHNIQUE;
        evt.componentName = GetName();
        evt.technique = this;
        evt.emitter = 0;
        PushEvent(evt);
    }
    
    void DiParticleElement::PushEvent(ParticleUniverseEvent& particleUniverseEvent)
    {
        if (mParentSystem)
        {
            mParentSystem->PushEvent(particleUniverseEvent);
        }
    }
    
    void DiParticleElement::CopyTo (DiParticleElement* technique)
    {
        technique->mParentSystem = mParentSystem;

        technique->DestroyAllEmitters();
        technique->DestroyAllControllers();
        technique->DestroyRenderer();

        DiParticle::CopyTo(technique);

        technique->mName = mName;
        technique->mVisualParticlePoolIncreased = false;
        technique->mVisualParticleQuota = mVisualParticleQuota;
        technique->mEmittedEmitterQuota = mEmittedEmitterQuota;
        technique->mEmittedTechniqueQuota = mEmittedTechniqueQuota;
        technique->mEmittedAffectorQuota = mEmittedAffectorQuota;
        technique->mEmittedSystemQuota = mEmittedSystemQuota;
        technique->mMaterialName = mMaterialName;
        technique->mDefaultWidth = mDefaultWidth;
        technique->mDefaultHeight = mDefaultHeight;
        technique->mDefaultDepth = mDefaultDepth;
        technique->m_usLodIndex = m_usLodIndex;
        technique->mKeepLocal = mKeepLocal;
        technique->mMaxVelocity = mMaxVelocity;
        technique->mMaxVelocitySet = mMaxVelocitySet;
        technique->mParticleSystemScale = mParticleSystemScale;

        DiParticleRenderer* clonedRenderer = GetParentSystem()->GetEffectManager()->CloneRenderer(mRenderer);
        technique->SetRenderer(clonedRenderer);

        DiParticleEmitter* clonedEmitter = 0;
        ParticleEmitterIterator itEmitter;
        ParticleEmitterIterator itEmitterEnd = mEmitters.end();
        for (itEmitter = mEmitters.begin(); itEmitter != itEmitterEnd; ++itEmitter)
        {
            clonedEmitter = GetParentSystem()->GetEffectManager()->CloneEmitter(*itEmitter);
            technique->AddEmitter(clonedEmitter);
        }

        DiParticleController* clonedAffector = 0;
        ParticleAffectorIterator itAffector;
        ParticleAffectorIterator itAffectorEnd = mControllers.end();
        for (itAffector = mControllers.begin(); itAffector != itAffectorEnd; ++itAffector)
        {
            clonedAffector = GetParentSystem()->GetEffectManager()->CloneController(*itAffector);
            technique->AddControlle(clonedAffector);
        }
    }
}

