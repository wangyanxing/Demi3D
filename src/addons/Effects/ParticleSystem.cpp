
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
#include "ParticleSystem.h"
#include "SceneManager.h"
#include "CullNode.h"
#include "RenderWindow.h"
#include "ParticleElement.h"
#include "ParticleEmitter.h"
#include "EffectManager.h"

namespace Demi
{
    const bool      DiParticleSystem::DEFAULT_KEEP_LOCAL                 = false;
    const float     DiParticleSystem::DEFAULT_ITERATION_INTERVAL         = 0.0f;
    const float     DiParticleSystem::DEFAULT_FIXED_TIMEOUT              = 0.0f;
    const float     DiParticleSystem::DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT = 0.0f;
    const bool      DiParticleSystem::DEFAULT_SMOOTH_LOD                 = false;
    const float     DiParticleSystem::DEFAULT_FAST_FORWARD_TIME          = 0.0f;
    const float     DiParticleSystem::DEFAULT_SCALE_VELOCITY             = 1.0f;
    const float     DiParticleSystem::DEFAULT_SCALE_TIME                 = 1.0f;
    const DiVec3    DiParticleSystem::DEFAULT_SCALE                      = DiVec3::UNIT_SCALE;
    const bool      DiParticleSystem::DEFAULT_TIGHT_BOUNDINGBOX          = false;
    
    DiParticleSystem::DiParticleSystem(const DiString& name) :
        mAABB(),
        DiTransformUnit(name),
        mTimeSinceLastVisible(0.0f),
        mLastVisibleFrame(0),
        mState(DiParticleSystem::PSS_STOPPED),
        mNonvisibleUpdateTimeout(DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT),
        mNonvisibleUpdateTimeoutSet(false),
        mSmoothLod(DEFAULT_SMOOTH_LOD),
        mSuppressNotifyEmissionChange(true),
        mIterationInterval(DEFAULT_ITERATION_INTERVAL),
        mIterationIntervalSet(false),
        mTimeSinceLastUpdate(0.0f),
        mFixedTimeout(DEFAULT_FIXED_TIMEOUT),
        mFixedTimeoutSet(false),
        mBoundsAutoUpdate(true),
        mBoundsUpdateTime(10.0f),
        mOriginalBoundsUpdateTime(10.0f),
        mFastForwardSet(false),
        mOriginalFastForwardSet(false),
        mFastForwardTime(DEFAULT_FAST_FORWARD_TIME),
        mFastForwardInterval(0.0f),
        mCurrentCamera(0),
        mParticleSystemScale(DEFAULT_SCALE),
        mParticleSystemScaleVelocity(DEFAULT_SCALE_VELOCITY),
        mParticleSystemScaleTime(DEFAULT_SCALE_TIME),
        mKeepLocal(DEFAULT_KEEP_LOCAL),
        mTightBoundingBox(DEFAULT_TIGHT_BOUNDINGBOX),
        mPauseTime(0.0f),
        mPauseTimeSet(false),
        mPauseTimeElapsed(0.0f),
        mTemplateName(DiString::BLANK),
        mStopFadeSet(false),
        mLatestOrientation(DiQuat::IDENTITY),
        mRotationOffset(DiQuat::IDENTITY),
        mRotationCentre(DiVec3::ZERO),
        mAtLeastOneParticleEmitted(false),
        mLastLodIndex(0)
    {
        particleType = PT_UNUSED;
        mGroupType = BATCH_EFFECT;
    }
    
    DiParticleSystem::~DiParticleSystem(void)
    {
        PushSystemEvent(PU_EVT_SYSTEM_DELETING);
        DestroyAllElements();
    }
    
    DiParticleSystem& DiParticleSystem::operator=(const DiParticleSystem& ps)
    {
        DestroyAllElements();

        size_t i = 0;
        DiParticleElement* psTechnique = 0;
        DiParticleElement* clonedTechnique = 0;
        for(i = 0; i < ps.GetNumElements(); ++i)
        {
            psTechnique = ps.GetElement(i);
            clonedTechnique = GetEffectManager()->CloneElement(psTechnique);
            AddElement(clonedTechnique);
        }

        mNonvisibleUpdateTimeout      = ps.mNonvisibleUpdateTimeout;
        mNonvisibleUpdateTimeoutSet   = ps.mNonvisibleUpdateTimeoutSet;
        mLodDistanceList              = ps.mLodDistanceList;
        mSmoothLod                    = ps.mSmoothLod;
        mIterationIntervalSet         = ps.mIterationIntervalSet;
        mIterationInterval            = ps.mIterationInterval;
        mTimeSinceLastUpdate          = ps.mTimeSinceLastUpdate;
        mFixedTimeout                 = ps.mFixedTimeout;
        mFixedTimeoutSet              = ps.mFixedTimeoutSet;
        mState                        = ps.mState;
        mTimeElapsedSinceStart        = ps.mTimeElapsedSinceStart;
        mSuppressNotifyEmissionChange = ps.mSuppressNotifyEmissionChange;
        mFastForwardSet               = ps.mFastForwardSet;
        mOriginalFastForwardSet       = ps.mOriginalFastForwardSet;
        mFastForwardTime              = ps.mFastForwardTime;
        mFastForwardInterval          = ps.mFastForwardInterval;
        mParticleSystemScale          = ps.mParticleSystemScale;
        mParticleSystemScaleVelocity  = ps.mParticleSystemScaleVelocity;
        mParticleSystemScaleTime      = ps.mParticleSystemScaleTime;
        mKeepLocal                    = ps.mKeepLocal;
        mTightBoundingBox             = ps.mTightBoundingBox;
        mPauseTime                    = ps.mPauseTime;
        mPauseTimeSet                 = ps.mPauseTimeSet;
        mPauseTimeElapsed             = ps.mPauseTimeElapsed;
        mTemplateName                 = ps.mTemplateName;
        mStopFadeSet                  = ps.mStopFadeSet;

        mSuppressNotifyEmissionChange = false;
        NotifyEmissionChange();
        NotifyRescaled();
        NotifyVelocityRescaled();

        return *this;
    }
    
    void DiParticleSystem::AddParticleSystemListener (DiParticleSystemListener* particleSystemListener)
    {
        mParticleSystemListenerList.push_back(particleSystemListener);
    }
    
    void DiParticleSystem::RemoveParticleSystemListener (DiParticleSystemListener* particleSystemListener)
    {
        DI_ASSERT(particleSystemListener && "ParticleSystemListener is null!");

        for (auto it = mParticleSystemListenerList.begin();
             it != mParticleSystemListenerList.end(); ++it)
        {
            if (*it == particleSystemListener)
            {
                mParticleSystemListenerList.erase(it);
                break;
            }
        }
    }
    
    void DiParticleSystem::PushSystemEvent(EventType eventType)
    {
        ParticleUniverseEvent evt;
        evt.eventType     = eventType;
        evt.componentType = CT_SYSTEM;
        evt.componentName = GetName();
        evt.technique     = 0;
        evt.emitter       = 0;
        PushEvent(evt);
    }
    
    void DiParticleSystem::PushEvent(ParticleUniverseEvent& particleUniverseEvent)
    {
        if (mParticleSystemListenerList.empty())
            return;

        for (auto it = mParticleSystemListenerList.begin();
             it != mParticleSystemListenerList.end(); ++it)
        {
            (*it)->HandleParticleSystemEvent(this, particleUniverseEvent);
        }
    }
    
    bool DiParticleSystem::HasTightBoundingBox(void) const
    {
        return mTightBoundingBox;
    }
    
    void DiParticleSystem::SetTightBoundingBox(bool tightBoundingBox)
    {
        mTightBoundingBox = tightBoundingBox;
    }
    
    float DiParticleSystem::GetPauseTime(void) const
    {
        return mPauseTime;
    }
    
    void DiParticleSystem::SetPauseTime(float pauseTime)
    {
        mPauseTime = pauseTime;
        mPauseTimeSet = true;
    }
    
    const DiString& DiParticleSystem::GetTemplateName(void) const
    {
        return mTemplateName;
    }
    
    void DiParticleSystem::SetTemplateName(const DiString& templateName)
    {
        mTemplateName = templateName;
    }
    
    void DiParticleSystem::SetEnabled(bool enabled)
    {
        DiParticle::SetEnabled(enabled);
        if (enabled && mState != PSS_STARTED)
            Start();
        else
            Stop();
    }
    
    bool DiParticleSystem::IsKeepLocal(void) const
    {
        return mKeepLocal;
    }
    
    void DiParticleSystem::SetKeepLocal(bool keepLocal)
    {
        mKeepLocal = keepLocal;
    }
    
    bool DiParticleSystem::MakeParticleLocal(DiParticle* particle)
    {
        if (!particle)
            return true;

        if (!mKeepLocal || HasEventFlags(DiParticle::PEF_EXPIRED))
            return false;

        DiVec3 diff = GetDerivedPosition() - latestPosition;
        particle->position += diff;
        return true;
    }
    
    const DiVec3& DiParticleSystem::GetDerivedPosition(void)
    {
        if (mMarkedForEmission)
        {
            mDerivedPosition = position;
        }
        else
        {
            if (mParentNode)
                mDerivedPosition = mParentNode->GetDerivedPosition();
            else
                mDerivedPosition = DiVec3::ZERO;
        }
        return mDerivedPosition;
    }
    
    const DiQuat& DiParticleSystem::GetDerivedOrientation(void) const
    {
        if (mMarkedForEmission)
        {
            if (parentEmitter)
                return parentEmitter->GetParentElement()->GetParentSystem()->GetDerivedOrientation();
            else
                return DiQuat::IDENTITY;
        }
        else
        {
            if (mParentNode)
                return mParentNode->GetDerivedOrientation();
            else
                return DiQuat::IDENTITY;
        }
    }
    
    const DiQuat& DiParticleSystem::GetLatestOrientation(void) const
    {
        return mLatestOrientation;
    }
    
    bool DiParticleSystem::HasRotatedBetweenUpdates(void) const
    {
        return mLatestOrientation != GetDerivedOrientation();
    }
    
    void DiParticleSystem::CalulateRotationOffset(void)
    {
        if (mMarkedForEmission)
            mRotationCentre = parentEmitter->GetParentElement()->GetParentSystem()->GetDerivedPosition();
        else
            mRotationCentre = GetDerivedPosition();

        mRotationOffset = GetDerivedOrientation() * mLatestOrientation.Inverse();
    }
    
    void DiParticleSystem::RotationOffset(DiVec3& pos)
    {
        pos = mRotationCentre + mRotationOffset * (pos - mRotationCentre);
    }
    
    DiParticleElement* DiParticleSystem::CreateElement(void)
    {
        DiParticleElement* technique = GetEffectManager()->CreateElement();
        AddElement(technique);
        return technique;
    }
    
    void DiParticleSystem::AddElement(DiParticleElement* element)
    {
        mElements.push_back(element);
        element->SetParentSystem(this);
        element->NotifyRescaled(mParticleSystemScale);
        element->NotifyVelocityRescaled(mParticleSystemScaleVelocity);
        NotifyEmissionChange();
    }
    
    void DiParticleSystem::RemoveElement(DiParticleElement* element)
    {
        DI_ASSERT(element);

        for (auto it = mElements.begin(); it != mElements.end(); ++it)
        {
            if ((*it) == element)
            {
                mElements.erase(it);
                break;
            }
        }

        element->SetParentSystem(NULL);

        NotifyEmissionChange();
    }
    
    DiParticleElement* DiParticleSystem::GetElement (size_t index) const
    {
        DI_ASSERT(index < mElements.size());
        return mElements[index];
    }
    
    DiParticleElement* DiParticleSystem::GetElement (const DiString& techniqueName) const
    {
        if (techniqueName == DiString::BLANK)
            return nullptr;

        for (auto it = mElements.begin(); it != mElements.end(); ++it)
        {
            if ((*it)->GetName() == techniqueName)
                return *it;
        }

        return nullptr;
    }
    
    size_t DiParticleSystem::GetNumElements (void) const
    {
        return mElements.size();
    }
    
    void DiParticleSystem::DestroyElement(DiParticleElement* element)
    {
        DI_ASSERT(element);
        ParticleTechniqueIterator it;
        for (it = mElements.begin(); it != mElements.end(); ++it)
        {
            if ((*it) == element)
            {
                DI_DELETE(*it);
                mElements.erase(it);
                break;
            }
        }

        NotifyEmissionChange();
    }
    
    void DiParticleSystem::DestroyElement (size_t index)
    {
        DestroyElement(GetElement(index));
    }
    
    void DiParticleSystem::DestroyAllElements (void)
    {
        for (auto it = mElements.begin(); it != mElements.end(); ++it)
        {
            DI_DELETE(*it);
        }
        mElements.clear();
    }
    
    float DiParticleSystem::GetFastForwardTime(void) const
    {
        return mFastForwardTime;
    }
    
    float DiParticleSystem::GetFastForwardInterval(void) const
    {
        return mFastForwardInterval;
    }
    
    void DiParticleSystem::SetFastForward(float time, float interval)
    {
        mFastForwardSet         = true;
        mOriginalFastForwardSet = true;
        mFastForwardTime        = time;
        mFastForwardInterval    = interval;
    }

    DiCamera* DiParticleSystem::GetMainCamera(void)
    {
        if (Driver->GetSceneManager())
        {
            mCurrentCamera = Driver->GetSceneManager()->GetCamera();
            return mCurrentCamera;
        }
        return nullptr;
    }
    
    void DiParticleSystem::SetMainCamera(DiCamera* camera)
    {
        if (!camera)
            return;
        
        mCurrentCamera = camera;
    }
    
    DiCamera* DiParticleSystem::GetCurrentCamera(void)
    {
        return mCurrentCamera;
    }
    
    void DiParticleSystem::FastForward(void)
    {
        if (!mFastForwardSet)
            return;

        for (float ftime = 0; ftime < mFastForwardTime; ftime += mFastForwardInterval)
            Update(mFastForwardInterval);
        
        mFastForwardSet = false;
    }
    
    void DiParticleSystem::NotifyAttached(DiNode* parent)
    {
        PushSystemEvent(PU_EVT_SYSTEM_ATTACHING);

        DiTransformUnit::NotifyAttached(parent);
        
        if (parent)
        {
            mTimeSinceLastVisible = 0;
            mLastVisibleFrame = Driver->GetFrameNum();
        }

        for (auto it = mElements.begin();
             it != mElements.end(); ++it)
        {
            (*it)->NotifyAttached(parent);
        }

        PushSystemEvent(PU_EVT_SYSTEM_ATTACHED);
    }
    
    void DiParticleSystem::NotifyCurrentCamera(DiCamera* cam)
    {
        mCurrentCamera = cam;
        DiTransformUnit::NotifyCurrentCamera(cam);
        mLastVisibleFrame = Driver->GetFrameNum();
        mTimeSinceLastVisible = 0.0f;

        ParticleTechniqueIterator it;
        ParticleTechniqueIterator itEnd = mElements.end();
        DiVec3 vec = DiVec3::ZERO;
        float squareDistance = 0;
        DiVec3 vecCameraParentNode = cam->GetDerivedPosition() - GetParentNode()->GetDerivedPosition();
        float squareDistanceCameraParentNode = vecCameraParentNode.squaredLength();
        unsigned short index = 0;
        bool doCamera = true;

        for (it = mElements.begin(); it != itEnd; ++it)
        {
            if ((*it)->IsMarkedForEmission())
            {
                vec = cam->GetDerivedPosition() - (*it)->position;
                squareDistance = vec.squaredLength();
            }
            else
            {
                vec = vecCameraParentNode;
                squareDistance = squareDistanceCameraParentNode;
            }

            if (doCamera)
                (*it)->SetCameraSquareDistance(squareDistance);

            if (!mLodDistanceList.empty())
            {
                if (doCamera)
                {
                    index = GetLodIndexSquaredDistance(squareDistance);
                    if ((*it)->GetLodIndex() == index)
                    {
                        (*it)->SetEnabled(true);

                        if (index != mLastLodIndex)
                        {
                            ParticleUniverseEvent evt;
                            evt.eventType     = PU_EVT_LOD_TRANSITION;
                            evt.componentType = CT_TECHNIQUE;
                            evt.componentName = (*it)->GetName();
                            evt.technique     = *it;
                            evt.emitter       = 0;
                            PushEvent(evt);
                        }
                        mLastLodIndex = index;
                    }
                    else
                    {
                        (*it)->SetEnabled(false);
                    }
                }
            }

            (*it)->NotifyCurrentCamera(cam);
        }
    }

    const DiAABB& DiParticleSystem::GetBoundingBox(void) const
    {
        return mAABB;
    }
    
    void DiParticleSystem::Update(DiCamera* camera)
    {
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
            (*it)->Update(camera);
        
        Update(Driver->GetDeltaSecond());
    }
    
    void DiParticleSystem::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
            (*it)->AddToBatchGroup(bg);
    }
    
    void DiParticleSystem::SetBatchGroup(DiBatchGroupType gt)
    {
        DiTransformUnit::SetBatchGroup(gt);

        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->SetBatchGroupID((uint8)gt);
        }
    }
    
    void DiParticleSystem::Update(float timeElapsed)
    {
        if (!mParentNode)
            return;

        // Only update if the particle system is started or prepare the particle system before starting.
        if (mState == DiParticleSystem::PSS_STARTED)
        {
            if (mNonvisibleUpdateTimeoutSet)
            {
                long frameDiff = Driver->GetFrameNum() - mLastVisibleFrame;
                if (frameDiff > 1 || frameDiff < 0)
                {
                    mTimeSinceLastVisible += timeElapsed;
                    if (mTimeSinceLastVisible >= mNonvisibleUpdateTimeout)
                    {
                        return;
                    }
                }
            }

            // Speedup or slowdown
            timeElapsed *= mParticleSystemScaleTime;

            // Only update the time since start if the DiParticleSystem is in the 'start' state.
            // Stop the DiParticleSystem if the fixed timeout has been reached (if applicable).
            mTimeElapsedSinceStart += timeElapsed;
            if (mFixedTimeoutSet)
            {
                if (mTimeElapsedSinceStart >= mFixedTimeout)
                {
                    // stop playing
                    if (mStopFadeSet)
                    {
                        StopFade();
                        mFixedTimeoutSet = false;
                    }
                    else
                    {
                        Stop();
                        return;
                    }
                }
            }

            // Update bound timer (if not auto updated)
            if (!mBoundsAutoUpdate && mBoundsUpdateTime > 0.0f)
            {
                mBoundsUpdateTime -= timeElapsed;
            }

            // Calculate rotation of the node
            CalulateRotationOffset();

            // Determine whether timeElapsed or iterationInterval is used
            size_t particlesLeft = 0;
            if (mIterationIntervalSet)
            {
                // Update time since last update
                mTimeSinceLastUpdate += timeElapsed;
                while (mTimeSinceLastUpdate >= mIterationInterval)
                {
                    // Update all techniques using the iteration interval value
                    particlesLeft = UpdateElements(mIterationInterval);
                    mTimeSinceLastUpdate -= mIterationInterval;
                }
            }
            else
            {
                // Update all techniques using the time elapsed (since last frame)
                particlesLeft = UpdateElements(timeElapsed);
            }

            // Handle situation when no particles are emitted anymore
            if (particlesLeft == 0)
            {
                if (mAtLeastOneParticleEmitted)
                {
                    // Generate the event
                    PushSystemEvent(PU_EVT_NO_PARTICLES_LEFT);
                    mAtLeastOneParticleEmitted = false;
                }

                // Determine whether the particle system should be stopped because of a fade out
                if (mStopFadeSet)
                {
                    if (!mFixedTimeoutSet || (mFixedTimeoutSet && mTimeElapsedSinceStart >= mFixedTimeout))
                    {
                        Stop();
                        return;
                    }
                }
            }
            else
            {
                // At least one particle was emitted, so if 'particlesLef' becomes 0, it concerns the period after the last emitted particle.
                mAtLeastOneParticleEmitted = true;
            }
        }
        else if (mState == DiParticleSystem::PSS_PREPARED)
        {
            // Generate the event
            PushSystemEvent(PU_EVT_SYSTEM_PREPARING);

            // Prepare all techniques (perform some initialisation in advance)
            auto itEnd = mElements.end();
            for (auto it = mElements.begin(); it != itEnd; ++it)
            {
                (*it)->Prepare();
            }

            // Only do it once.
            mState = DiParticleSystem::PSS_STOPPED;

            // Generate the event
            PushSystemEvent(PU_EVT_SYSTEM_PREPARED);
        }
        else if (mState == DiParticleSystem::PSS_PAUSED)
        {
            // Determine whether there is a limit to the pause
            if (mPauseTimeSet)
            {
                mPauseTimeElapsed += timeElapsed;
                if (mPauseTimeElapsed > mPauseTime)
                {
                    mPauseTimeElapsed = 0.0f;
                    Resume();
                }
            }
        }

        // Set the latest position. This value is always needed, whether the Particle System is emitted or not.
        latestPosition = GetDerivedPosition();
        mLatestOrientation = GetDerivedOrientation();
    }
    
    size_t DiParticleSystem::UpdateElements(float timeElapsed)
    {
        /** Update all techniques if particle system is started (only if the techniques aren't emitted 
            themselves) and return the total number of emitted particles.
            Note, that emitted techniques are updated by the technique that is responsible for emitting 
            them. The technique that is emitting pooled techniques is also responsible to take the 
            (world)AABB of that pooled technique into account.
        */
        ParticleTechniqueIterator it;
        ParticleTechniqueIterator itEnd = mElements.end();
        size_t particlesLeft = 0;
        bool mAABBUpdate = mParentNode && (mBoundsAutoUpdate || mBoundsUpdateTime > 0.0f);
        bool merge = mAABBUpdate;
        DiAABB worldAABB(mParentNode->GetDerivedPosition(), mParentNode->GetDerivedPosition());
        for (it = mElements.begin(); it != itEnd; ++it)
        {
            if (!(*it)->IsMarkedForEmission())
            {
                // Only call this if update bounds is needed.
                if (merge)
                {
                    // Call NotifyUpdateBounds() for each Particle Technique, so the mWorldAABB in the 
                    // DiParticleElement::_update() function is (re)calculated.
                    (*it)->NotifyUpdateBounds();
                }

                // Always update the ParticleTechniques
                (*it)->Update(timeElapsed);

                // Merge worldAABB's of all ParticleTechniques
                if (merge)
                {
                    // Get the WorldAABB from each technique and merge it with the worldAABB.
                    worldAABB.Merge((*it)->GetWorldBoundingBox());

                    // If worldAABB is infinite, ignore the other Particle Techniques.
                    if (worldAABB.IsInfinite())
                    {
                        merge = false;
                    }
                }

                // Count all left particles, used for anyone who needs it.
                particlesLeft += (*it)->GetNumberOfEmittedParticles();
            }
        }

        if (mState == DiParticleSystem::PSS_STOPPED)
        {
            /** Don't bother to update the mAABB, because the system is stopped in one of the techniques.
                The bounds must be reset, because even though the stop() has been called (which alread resets the
                bounds) another technique in the list might set the bounds again.
            */
            ResetBounds();
        }
        else if (mAABBUpdate)
        {
            // If needed, update mAABB
            if (!worldAABB.IsNull())
            {
                if (mTightBoundingBox)
                {
                    // Wrap the bounding box tight around the particle system
                    mAABB = worldAABB;
                    mAABB.TransformAffine(mParentNode->GetFullTransform().inverseAffine());
                }
                else
                {
                    // Merge with the current bounding box
                    // Note, that the mAABB must in localspace, so transformation of the worldAABB is required.
                    DiAABB newAABB(worldAABB);
                    newAABB.TransformAffine(mParentNode->GetFullTransform().inverseAffine());
    
                    // Merge calculated box with current AABB.
                    mAABB.Merge(newAABB);
                }
            }
            else
            {
                ResetBounds();
            }

            // Notify the parent node that the bounds are changed
            mParentNode->NeedUpdate();
        }

        return particlesLeft;
    }
    
    void DiParticleSystem::ResetBounds(void)
    {
        // Reset the bounds to zero
        if (!mAABB.IsNull())
        {
            mAABB.SetNull();
        }
        if (mParentNode)
        {
            mParentNode->NeedUpdate();
        }
    }
    
    void DiParticleSystem::SetBoundsAutoUpdated(bool autoUpdate, float stopIn)
    {
        mBoundsAutoUpdate = autoUpdate;
        mBoundsUpdateTime = stopIn;
        mOriginalBoundsUpdateTime = stopIn;
    }
    
    const DiVec3& DiParticleSystem::GetScale(void) const
    {
        return mParticleSystemScale;
    }
    
    void DiParticleSystem::SetScale(const DiVec3& scale)
    {
        // Set the scale and notify the particle techniques
        mParticleSystemScale = scale;
        NotifyRescaled();
    }
    
    void DiParticleSystem::NotifyRescaled(void)
    {
        // Notify the particle techniques
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->NotifyRescaled(mParticleSystemScale);
        }
    }
    
    const float& DiParticleSystem::GetScaleVelocity(void) const
    {
        return mParticleSystemScaleVelocity;
    }
    
    void DiParticleSystem::SetScaleVelocity(const float& scaleVelocity)
    {
        // Set the scale and notify the particle techniques
        mParticleSystemScaleVelocity = scaleVelocity;
        NotifyVelocityRescaled();
    }
    
    void DiParticleSystem::NotifyVelocityRescaled(void)
    {
        // Notify the particle techniques
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->NotifyVelocityRescaled(mParticleSystemScaleVelocity);
        }
    }
    
    const float& DiParticleSystem::GetScaleTime(void) const
    {
        return mParticleSystemScaleTime;
    }
    
    void DiParticleSystem::SetScaleTime(const float& scaleTime)
    {
        mParticleSystemScaleTime = scaleTime;
    }
    
    void DiParticleSystem::InitForEmission(void)
    {
        DiParticle::InitForEmission();

        Start();
    }
    
    void DiParticleSystem::InitForExpiration(DiParticleElement* technique, float timeElapsed)
    {
        DiParticle::InitForExpiration(technique, timeElapsed);

        Stop();
    }
    
    void DiParticleSystem::Process(DiParticleElement* technique, float timeElapsed)
    {
        // Inherited from the Particle class and is only called if the Particle System is emitted.
        // Update node position.
        DiParticle::Process(technique, timeElapsed);
        DiNode* node = technique->GetParentSystem()->GetParentNode();
        if (mParentNode && node)
        {
            // position attribute is derived, but the the parentNode position must be set in relation to its parent.
            mParentNode->SetPosition(position - node->GetPosition());
        }
    }
    
    float DiParticleSystem::GetNonVisibleUpdateTimeout(void) const
    {
        return mNonvisibleUpdateTimeout;
    }
    
    void DiParticleSystem::SetNonVisibleUpdateTimeout(float timeout)
    {
        if (timeout > 0)
        {
            mNonvisibleUpdateTimeout = timeout;
            mNonvisibleUpdateTimeoutSet = true;
        }
        else
        {
            mNonvisibleUpdateTimeout = 0;
            mNonvisibleUpdateTimeoutSet = false;
        }
    }
    
    const DiParticleSystem::LodDistanceList& DiParticleSystem::GetLodDistances(void) const
    {
        return mLodDistanceList;
    }

    void DiParticleSystem::ClearLodDistances(void)
    {
        mLodDistanceList.clear();
    }
    
    void DiParticleSystem::AddLodDistance(float distance)
    {
        mLodDistanceList.push_back(distance * distance);
    }
    
    void DiParticleSystem::SetLodDistances(const LodDistanceList& lodDistances)
    {
        LodDistanceConstIterator it, itEnd;
        itEnd = lodDistances.end();
        mLodDistanceList.clear();
        for (it = lodDistances.begin(); it != itEnd; ++it)
        {
            mLodDistanceList.push_back((*it) * (*it));
        }
    }
    
    unsigned short DiParticleSystem::GetLodIndex(float distance) const
    {
        return GetLodIndexSquaredDistance(distance * distance);
    }
    
    unsigned short DiParticleSystem::GetLodIndexSquaredDistance(float squaredDistance) const
    {
        LodDistanceConstIterator it, itEnd;
        itEnd = mLodDistanceList.end();
        unsigned short index = 0;
        for (it = mLodDistanceList.begin(); it != itEnd; ++it, ++index)
        {
            if (squaredDistance < (*it))
            {
                return index;
            }
        }

        return static_cast<unsigned short>(mLodDistanceList.size());
    }
    
    size_t DiParticleSystem::GetNumEmittedTechniques (void) const
    {
        auto itEnd = mElements.end();
        size_t count = 0;
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            if ((*it)->IsMarkedForEmission())
            {
                count++;
            }
        }

        return count;
    }
    
    void DiParticleSystem::MarkForEmission(void)
    {
        // Reset the marks on all techniques.
        ResetMarkForEmission();

        /** Force techniques to be enabled to mark the emitted objects. Do this first for all techniques, before
            the techniques perform their marking. The reason for this is, that one technique might mark 
            another technique.
        */
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->SuppressNotifyEmissionChange(false);
        }

        // Mark the emitted objects.
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->MarkForEmission();
        }
    }
    
    void DiParticleSystem::ResetMarkForEmission(void)
    {
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->SetMarkedForEmission(false);
        }
    }
    
    void DiParticleSystem::SuppressNotifyEmissionChange(bool suppress)
    {
        mSuppressNotifyEmissionChange = suppress;
    }
    
    void DiParticleSystem::NotifyEmissionChange(void)
    {
        if (mSuppressNotifyEmissionChange)
        {
            return;
        }

        // Mark all emitted techniques, which on their turn mark other techniques, emitters and affectors.
        MarkForEmission();
    }
    
    float DiParticleSystem::GetIterationInterval(void) const
    {
        return mIterationInterval;
    }
    
    void DiParticleSystem::SetIterationInterval(const float iterationInterval)
    {
        if (iterationInterval > 0)
        {
            mIterationInterval = iterationInterval;
            mIterationIntervalSet = true;
        }
        else
        {
            mIterationInterval = 0;
            mIterationIntervalSet = false;
        }
    }
    
    float DiParticleSystem::GetFixedTimeout(void) const
    {
        return mFixedTimeout;
    }
    
    void DiParticleSystem::SetFixedTimeout(const float fixedTimeout)
    {
        if (fixedTimeout > 0.0f)
        {
            mFixedTimeout = fixedTimeout;
            mFixedTimeoutSet = true;
        }
        else
        {
            mFixedTimeout = 0.0f;
            mFixedTimeoutSet = false;
        }
    }
    
    void DiParticleSystem::Prepare (void)
    {
        mState = DiParticleSystem::PSS_PREPARED;
    }
    
    void DiParticleSystem::Start(void)
    {
        PushSystemEvent(PU_EVT_SYSTEM_STARTING);

        SetVisible(true);
        mState = DiParticleSystem::PSS_STARTED;
        mTimeElapsedSinceStart = 0;
        mLastVisibleFrame = 0;
        mTimeSinceLastVisible = 0;
        mTimeSinceLastUpdate = 0;
        mBoundsUpdateTime = mOriginalBoundsUpdateTime;
        ParticleTechniqueIterator it;
        ParticleTechniqueIterator itEnd = mElements.end();
        mAABB.SetExtents(0, 0, 0, 0, 0, 0);
        mFastForwardSet = mOriginalFastForwardSet;
        position = DiVec3::ZERO;
        mDerivedPosition = DiVec3::ZERO;
        latestPosition = GetDerivedPosition();
        mLatestOrientation = GetDerivedOrientation();
        mEnabled = true;
        mStopFadeSet = false;
        mAtLeastOneParticleEmitted = false;
        mLastLodIndex = 0;

        for (it = mElements.begin(); it != itEnd; ++it)
        {
            (*it)->NotifyStart();
        }
        
        FastForward();

        PushSystemEvent(PU_EVT_SYSTEM_STARTED);
    }
    
    void DiParticleSystem::Start(float stopTime)
    {
        Start();
        Stop(stopTime);
    }
    
    void DiParticleSystem::StartAndStopFade(float stopTime)
    {
        Start();
        StopFade(stopTime);
    }
    
    void DiParticleSystem::Stop(void)
    {
        /*  Note, that the DiParticleSystem isn't visible anymore, but it is still attached to the
            node and still consumes resources, but it allows fast start/stop iterations if needed.
            An important thing why it keeps attached to the node is that attaching and detaching the 
            DiParticleSystem to/from a node must be done outside of the DiParticleSystem. If the DiParticleSystem
            isn't used anymore you have to detach it from the node yourself.
        */
        if (mState != DiParticleSystem::PSS_STOPPED)
        {
            // Generate the event
            PushSystemEvent(PU_EVT_SYSTEM_STOPPING);

            /** Notify all techniques to stop.
            */
            SetVisible(false); // Set this movable to invisible
            auto itEnd = mElements.end();
            for (auto it = mElements.begin(); it != itEnd; ++it)
            {
                (*it)->NotifyStop();
            }

            ResetBounds();
            mState = DiParticleSystem::PSS_STOPPED;

            mStopFadeSet = false;
            mEnabled = false;

            // Generate the event
            PushSystemEvent(PU_EVT_SYSTEM_STOPPED);
        }
    }
    
    void DiParticleSystem::Stop(float stopTime)
    {
        SetFixedTimeout(stopTime);
    }
    
    void DiParticleSystem::StopFade(void)
    {
        size_t i;
        size_t j;
        size_t numTechniques = GetNumElements();
        size_t numEmitters;
        DiParticleElement* technique;
        DiParticleEmitter* emitter;
        for (i = 0; i < numTechniques; ++i)
        {
            technique = GetElement(i);
            numEmitters = GetElement(i)->GetNumEmitters();
            for (j = 0; j < numEmitters; ++j)
            {
                emitter = technique->GetEmitter(j);
                emitter->SetEnabled(false);
            }
        }
        mStopFadeSet = true;
    }
    
    void DiParticleSystem::StopFade(float stopTime)
    {
        SetFixedTimeout(stopTime);
        mStopFadeSet = true;
    }
    
    void DiParticleSystem::Pause(void)
    {
        if (mState != DiParticleSystem::PSS_STOPPED)
        {
            PushSystemEvent(PU_EVT_SYSTEM_PAUSING);

            mState = DiParticleSystem::PSS_PAUSED;

            auto itEnd = mElements.end();
            for (auto it = mElements.begin(); it != itEnd; ++it)
            {
                (*it)->NotifyPause();
            }
        }

        PushSystemEvent(PU_EVT_SYSTEM_PAUSED);
    }
    
    void DiParticleSystem::Pause(float pauseTime)
    {
        Pause();
        SetPauseTime(pauseTime);
    }
    
    void DiParticleSystem::Resume(void)
    {
        if (mState != DiParticleSystem::PSS_STOPPED)
        {
            PushSystemEvent(PU_EVT_SYSTEM_RESUMING);

            mState = DiParticleSystem::PSS_STARTED;

            auto itEnd = mElements.end();
            for (auto it = mElements.begin(); it != itEnd; ++it)
            {
                (*it)->NotifyResume();
            }

            PushSystemEvent(PU_EVT_SYSTEM_RESUMED);
        }
    }
    
    size_t DiParticleSystem::GetNumberOfEmittedParticles(void)
    {
        size_t total = 0;
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            total += (*it)->GetNumberOfEmittedParticles();
        }
        return total;
    }
    
    size_t DiParticleSystem::GetNumberOfEmittedParticles(DiParticle::ParticleType particleType)
    {
        size_t total = 0;
        auto itEnd = mElements.end();
        for (auto it = mElements.begin(); it != itEnd; ++it)
        {
            total += (*it)->GetNumberOfEmittedParticles(particleType);
        }
        return total;
    }

    DiEffectManager* DiParticleSystem::GetEffectManager()
    {
        return DiEffectManager::GetInstancePtr();
    }

    DiString& DiParticleSystem::GetType()
    {
        static DiString type = "Particle";
        return type;
    }
}


