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

#include "K2Pch.h"
#include "K2Clip.h"
#include "K2Asset.h"
#include "Node.h"
#include "AlignedAllocator.h"

namespace Demi
{
    DiK2Animation::DiK2Animation()
        : mSource(nullptr)
        , mTarget(nullptr)
        , mBlendElapsed(0)
        , mBlendTime(0.3f)  // a default value
    {
        for (int i = 0; i < K2PrefabClip::MAX_PREFAB_ANIM; ++i)
            mClips[i] = nullptr;
    }

    DiK2Animation::~DiK2Animation()
    {
        for (int i = 0; i < K2PrefabClip::MAX_PREFAB_ANIM; ++i)
        {
            if (mClips[i])
            {
                DI_DELETE mClips[i];
                mClips[i] = nullptr;
            }
        }

        for (auto i = mExtraClips.begin(); i != mExtraClips.end(); ++i)
        {
            DI_DELETE i->second;
        }
        mExtraClips.clear();
    }

    void DiK2Animation::Play(const DiString& name)
    {
        auto it = mExtraClips.find(name);
        if (it == mExtraClips.end())
        {
            DI_WARNING("Cannot find the animation: %s",name.c_str());
            return;
        }

        DiK2Clip* clip = it->second;
        Play(clip);
    }

    void DiK2Animation::Play(DiK2Clip* clip)
    {
        if (clip->mNumFrames == 0)
            return;

        if (mSource && mTarget)
        {
            mSource = mTarget;
            mTarget = clip;
        }

        clip->Cleanup();

        mBlendElapsed = 0;

        if (!mSource)
            mSource = clip;
        else if (clip != mSource)
            mTarget = clip;
    }

    void DiK2Animation::Play(K2PrefabClip::Clips clip)
    {
        if (mClips[clip])
            Play(mClips[clip]);
    }

    void DiK2Animation::Update(float deltaTime)
    {
        if (!mSource && !mTarget)
            return;
        
        if (mSource && mTarget)
        {
            mBlendElapsed += deltaTime;
            if (mBlendElapsed >= mBlendTime)
            {
                mBlendElapsed = 0;
                mSource = mTarget;
                mTarget = nullptr;
            }
        }
        
        if (mSource)
            mSource->Update(deltaTime);
        
        if (mTarget)
            mTarget->Update(deltaTime);
    }

    DiK2Clip* DiK2Animation::AddClip(const DiString& name)
    {
        K2PrefabClip::Clips prefabClip = K2PrefabClip::FromString(name);
        if (prefabClip == K2PrefabClip::MAX_PREFAB_ANIM)
        {
            auto i = mExtraClips.find(name);
            DI_ASSERT(i == mExtraClips.end());
            DiK2Clip* c = DI_NEW DiK2Clip();
            mExtraClips[name] = c;
            return c;
        }
        else
        {
            DI_ASSERT(!mClips[prefabClip]);
            DiK2Clip* c = DI_NEW DiK2Clip();
            mClips[prefabClip] = c;
            return c;
        }
    }

    void DiK2Clip::Update(float deltaTime)
    {
        if (deltaTime > 0 && mNumFrames!=0)
        {
            mTime += deltaTime;
            float length = mNumFrames / mFPS;
            if (mTime >= length)
            {
                if (mLoop)
                    mTime = 0;
                else
                    mTime = length;
            }

            float realFrame = (mTime / length) * mNumFrames;
            mCurFrame = std::min(std::max(int(realFrame), 0), mNumFrames - 1);
            mInterpFactor = realFrame - mCurFrame;
        }
    }

    DiK2Clip::DiK2Clip() : 
          mLoop(false)
        , mCurFrame(0)
        , mFPS(0)
        , mNumFrames(0)
        , mTime(0)
        , mInterpFactor(0)
    {
    }

    DiK2Clip::~DiK2Clip()
    {
    }
    
    void DiK2Clip::GetFrameTrans(const DiString& bone, Trans& output)
    {
        size_t curFrame  = (size_t)mCurFrame;
        size_t nextFrame = (size_t)GetNextFrame();
        
        DiVector<Trans>& trans = mKeyFrames->boneFrames[bone];
        if (curFrame < trans.size() && nextFrame < trans.size())
        {
            Trans& source = trans[curFrame];
            Trans& target = trans[nextFrame];

            // just linear interpolate for now
            output.rot = DiQuat::Nlerp(mInterpFactor, source.rot, target.rot, true);
            output.pos = source.pos + (target.pos - source.pos) * mInterpFactor;
            output.scale = source.scale + (target.scale - source.scale) * mInterpFactor;
        }
        else
        {
            output.rot = DiQuat::IDENTITY;
            output.pos = DiVec3::ZERO;
            output.scale = DiVec3::UNIT_SCALE;
        }
    }

    DiK2Skeleton::DiK2Skeleton()
        : mRootNode(nullptr)
        , mRowData(nullptr)
        , mBoneMatrices(nullptr)
        , mBoneTransforms(nullptr)
    {
    }

    DiK2Skeleton::~DiK2Skeleton()
    {
        Destroy();
    }

    void DiK2Skeleton::Destroy()
    {
        for (auto i = mBones.begin(); i != mBones.end(); ++i)
        {
            DI_DELETE (*i);
        }
        mBones.clear();

        if (mBoneMatrices)
        {
            DI_DELETE[] mBoneMatrices;
        }

        if (mBoneTransforms)
        {
            DEMI_FREE_SIMD(mBoneTransforms);
        }
    }

    void DiK2Skeleton::CreateBones(DiK2BonesDataPtr boneData)
    {
        Destroy();
        mRowData = boneData;

        for (uint32 i = 0; i < boneData->names.size(); ++i)
        {
            DiNode* bone = DI_NEW DiNode(boneData->names[i]);
            mBones.push_back(bone);
            mBoneNames[boneData->names[i]] = bone;
        }

        for (uint32 i = 0; i < boneData->parents.size(); ++i)
        {
            int parentID = boneData->parents[i];
            if (parentID >= 0)
            {
                DiNode* child = mBones[i];
                DiNode* parent = mBones[parentID];
                parent->AddChild(child);
            }
            else
                mRootNode = mBones[i];
        }

        mBoneMatrices = DI_NEW DiMat4[mBones.size()];
        mBoneTransforms = (btTransform*)DEMI_MALLOC_SIMD(mBones.size()*sizeof(btTransform));
    }

    void DiK2Skeleton::Apply(DiK2Animation* anim)
    {
        DiK2Clip* clip = anim->mSource;
        DiK2Clip* clipTargt = anim->mTarget;
        float blendWeight = anim->mBlendElapsed / anim->mBlendTime;
        
        if (!clip)
            return;

        for (auto it = clip->mKeyFrames->boneFrames.begin(); 
            it != clip->mKeyFrames->boneFrames.end(); ++it)
        {
            const DiString& boneName = it->first;
            if (!HasBone(boneName))
                continue;

            DiNode* bone = GetBone(boneName);
            
            Trans t;
            clip->GetFrameTrans(boneName, t);
            
            if (clipTargt)
            {
                Trans tTarget;
                clipTargt->GetFrameTrans(boneName, tTarget);
                
                // linear as well
                bone->SetOrientation(DiQuat::Nlerp(blendWeight, t.rot, tTarget.rot, true));
                bone->SetPosition(t.pos + (tTarget.pos - t.pos) * blendWeight);
                bone->SetScale(t.scale + (tTarget.scale - t.scale) * blendWeight);
            }
            else
            {
                bone->SetOrientation(t.rot);
                bone->SetPosition(t.pos);
                bone->SetScale(t.scale);
            }
        }
    }
    
    void DiK2Skeleton::CacheBoneMatrices()
    {
        // yeah we just have one root bone here
        mRootNode->_Update(true, false);

        for (uint32 i = 0; i < mBones.size(); ++i)
        {
            DiNode* bone = mBones[i];
            DiMat4& invMat = mRowData->invtrans[i];
            const DiMat4& mat = bone->GetFullTransform();

            mBoneMatrices[i] = mat * invMat;
        }
    }
}