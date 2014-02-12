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
#include "GfxPch.h"
#include "ClipController.h"

namespace Demi
{
    DiClipController::DiClipController( DiClipControllerSet* parent,
        const DiString& name, float timePos, 
        float length, float weight /*= 1.0*/, bool enabled /*= false*/ )
        :mBoneBlendMask(NULL),
        mParent(parent),
        mAnimationName(name),
        mTimePos(timePos),
        mTimeRatio(1.0f),
        mLength(length),
        mWeight(weight),
        mEnabled(enabled),
        mLoop(true)
    {
        mParent->NotifyDirty();
    }

    DiClipController::DiClipController( DiClipControllerSet* parent, 
        const DiClipController &rhs )
        :mBoneBlendMask(NULL),
        mParent(parent),
        mAnimationName(rhs.mAnimationName),
        mTimePos(rhs.mTimePos),
        mTimeRatio(rhs.mTimeRatio),
        mLength(rhs.mLength),
        mWeight(rhs.mWeight),
        mEnabled(rhs.mEnabled),
        mLoop(rhs.mLoop)
    {
        mParent->NotifyDirty();
    }

    DiClipController::~DiClipController()
    {

    }

    void DiClipController::SetEnabled(bool enabled)
    {
        mEnabled = enabled;
        mParent->NotifyClipControllerEnabled(this, enabled);
    }

    void DiClipController::SetTimePosition( float timePos )
    {
        float fLocalTime = timePos;

        if (fLocalTime != mTimePos)
        {
            mTimePos = fLocalTime;
            if (mLoop)
            {
                mTimePos = fmod(mTimePos, mLength);
                if(mTimePos < 0)
                    mTimePos += mLength;
            }
            else
            {
                if(mTimePos < 0)
                    mTimePos = 0;
                else if (mTimePos > mLength)
                    mTimePos = mLength;
            }

            if(mMapEndCallbacks.size() && mEnabled)
            {
                PlayEndCallbackMap::iterator itCallback = mMapEndCallbacks.begin();
                for( ; itCallback != mMapEndCallbacks.end() ; )
                {
                    if(mTimePos > itCallback->second.triggerTime)
                    {
                        if(!mLoop || itCallback->second.isTriggerInLoop)
                        {
                            itCallback->second.callBackFunc(this);
                            mMapEndCallbacks.erase(itCallback ++);
                            continue;
                        }
                    }

                    ++ itCallback;
                }
            }

            if (mEnabled)
            {
                mParent->NotifyDirty();
            }
        }

    }

    void DiClipController::SetLength( float len )
    {
        mLength = len;
    }

    void DiClipController::SetWeight( float weight )
    {
        mWeight = weight;

        if (mEnabled)
        {
            mParent->NotifyDirty();
        }
    }

    void DiClipController::RegisterAniEventCallback( void * pkCallee , DiAniEvent rAniEvent )
    {
        PlayEndCallbackMap::iterator itCallee = mMapEndCallbacks.find(pkCallee);

        if(itCallee != mMapEndCallbacks.end())
        {
            DI_WARNING("RegisterAniEventCallback() - Cannot find the event callback");
            return;
        }

        mMapEndCallbacks[pkCallee] = rAniEvent;

    }

    void DiClipController::DegisterAniEventCallback( void * pkCallee )
    {
        PlayEndCallbackMap::iterator itCallee = mMapEndCallbacks.find(pkCallee);

        if(itCallee == mMapEndCallbacks.end())
        {
            DI_WARNING("RegisterAniEventCallback() - Cannot find the event callback");
            return;
        }

        mMapEndCallbacks.erase(pkCallee);
    }

    void DiClipController::AddTime( float delta )
    {
        DI_ASSERT(mTimeRatio > 0.0f);
        
        SetTimePosition(mTimePos + delta * mTimeRatio);
    }

    void DiClipController::CreateBoneBlendMask( size_t blendMaskSizeHint, 
        float initialWeight /*= 1.0f*/ )
    {
        if(!mBoneBlendMask)
        {
            if(initialWeight >= 0)
                mBoneBlendMask = DI_NEW BoneBlendMask(blendMaskSizeHint, initialWeight);
            else
                mBoneBlendMask = DI_NEW BoneBlendMask(blendMaskSizeHint);
        }
    }

    void DiClipController::DestroyBoneBlendMask()
    {
        DI_DELETE mBoneBlendMask;
        mBoneBlendMask = NULL;
    }

    void DiClipController::SetBoneBlendMaskData( const float* blendMaskData )
    {
        DI_ASSERT(mBoneBlendMask);
        
        if(!blendMaskData)
        {
            DestroyBoneBlendMask();
            return;
        }
        
        memcpy(&((*mBoneBlendMask)[0]), blendMaskData, sizeof(float) * mBoneBlendMask->size());
        if (mEnabled)
        {
            mParent->NotifyDirty();
        }
    }

    void DiClipController::SetBoneBlendMask( const BoneBlendMask* blendMask )
    {
        if(!mBoneBlendMask)
            CreateBoneBlendMask(blendMask->size(), false);
        
        SetBoneBlendMaskData(&(*blendMask)[0]);
    }

    void DiClipController::SetBoneBlendMaskEntry( size_t boneHandle, float weight )
    {
        DI_ASSERT(mBoneBlendMask && mBoneBlendMask->size() > boneHandle);
        (*mBoneBlendMask)[boneHandle] = weight;

        if (mEnabled)
            mParent->NotifyDirty();
    }

    void DiClipController::ClearAttachNodeBlendMask()
    {
        mAttachNodeBlendMask.clear();
    }

    void DiClipController::SetAttachNodeBlendMaskData( const AttachBlendMask& mapBlendMask )
    {
        if(!mapBlendMask.empty())
            mAttachNodeBlendMask = mapBlendMask;
    }

    void DiClipController::SetAttachNodeBlendMaskEntry( DiString strAttachName, float weight )
    {
        auto it = mAttachNodeBlendMask.find(strAttachName);

        if(it == mAttachNodeBlendMask.end())
        {
            DI_WARNING("Cannot find the attachment : %s",strAttachName.c_str());
            return;
        }

        it->second = weight;

        if (mEnabled)
            mParent->NotifyDirty();
    }

    //////////////////////////////////////////////////////////////////////////

    DiClipControllerSet::DiClipControllerSet()
        :mDirtyFrameNumber(std::numeric_limits<uint64>::max())
    {

    }

    DiClipControllerSet::DiClipControllerSet( const DiClipControllerSet& rhs )
        :mDirtyFrameNumber(std::numeric_limits<uint64>::max())
    {
        for (auto i = rhs.mClipControllers.begin();
            i != rhs.mClipControllers.end(); ++i)
        {
            DiClipController* src = i->second;
            mClipControllers[src->GetAnimationName()] = 
                DI_NEW DiClipController(this, *src);
        }

        for (auto it = rhs.mEnabledClips.begin();
            it != rhs.mEnabledClips.end(); ++it)
        {
            const DiClipController* src = *it;
            mEnabledClips.push_back(GetClipController(src->GetAnimationName()));
        }
    }

    DiClipControllerSet::~DiClipControllerSet()
    {
        RemoveClipControllers();
    }

    DiClipController* DiClipControllerSet::CreateClipController( const DiString& name, 
        float timePos, float length, float weight /*= 1.0*/, bool enabled /*= false*/ )
    {
        auto i = mClipControllers.find(name);
        if (i != mClipControllers.end())
        {
            DI_WARNING("The clip %s has already existed, create failed", name.c_str());
            return NULL;
        }

        DiClipController* newState = DI_NEW DiClipController(this, name,timePos, 
            length, weight, enabled);
        mClipControllers[name] = newState;
        return newState;
    }

    DiClipController* DiClipControllerSet::GetClipController( const DiString& name ) const
    {
        auto i = mClipControllers.find(name);
        if (i == mClipControllers.end())
        {
            DI_WARNING("Cannot find the animation clip: %s",name.c_str());
            return NULL;
        }
        return i->second;
    }

    bool DiClipControllerSet::HasClipController( const DiString& name ) const
    {
        return mClipControllers.find(name) != mClipControllers.end();
    }

    bool DiClipControllerSet::RenameClipController( const DiString& name,const DiString& strNewName )
    {
        if(strNewName.empty())
        {
            DI_WARNING("Clip name cannot be empty!");
            return false;
        }

        auto it = mClipControllers.find(name);
        if(it != mClipControllers.end())
        {
            DiClipController * pkClipController = (*it).second;
            mClipControllers.erase(name);

            pkClipController->SetAnimationName(strNewName);
            mClipControllers[strNewName] = pkClipController;

            NotifyDirty();

            return true;
        }

        DI_WARNING("Cannot find the animation clip : %s, rename failed.",name.c_str());
        return false;
    }

    void DiClipControllerSet::RemoveClipController( const DiString& name )
    {
        ClipControllerMap::iterator i = mClipControllers.find(name);
        if (i != mClipControllers.end())
        {
            mEnabledClips.remove(i->second);

            DI_DELETE i->second;
            mClipControllers.erase(i);
        }
    }

    void DiClipControllerSet::RemoveClipControllers( void )
    {
        for (ClipControllerMap::iterator i = mClipControllers.begin();
            i != mClipControllers.end(); ++i)
        {
            DI_DELETE i->second;
        }
        mClipControllers.clear();
        mEnabledClips.clear();
    }

    void DiClipControllerSet::NotifyDirty( void )
    {
        ++mDirtyFrameNumber;
    }

    void DiClipControllerSet::NotifyClipControllerEnabled( DiClipController* clip,bool enabled )
    {
        mEnabledClips.remove(clip);

        if (enabled)
        {
            mEnabledClips.push_back(clip);
        }

        NotifyDirty();
    }

    ConstEnabledClipsIt DiClipControllerSet::GetEnabledClipsIterator( void ) const
    {
        return ConstEnabledClipsIt(mEnabledClips.begin(),mEnabledClips.end());
    }

    Demi::ConstClipControllerIt DiClipControllerSet::GetClipControllerIterator( void ) const
    {
        return ConstClipControllerIt(mClipControllers.begin(),mClipControllers.end());
    }

    Demi::ClipControllerIt DiClipControllerSet::GetClipControllerIterator( void )
    {
        return ClipControllerIt(mClipControllers.begin(),mClipControllers.end());
    }

    void DiClipControllerSet::DisableAllClips()
    {
        for (ClipControllerMap::iterator i = mClipControllers.begin();
            i != mClipControllers.end(); ++i)
        {
            i->second->SetEnabled(false);
        }
    }

}