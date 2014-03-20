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

namespace Demi
{
    DiK2Animation::DiK2Animation()
        : mSource(nullptr)
        , mTarget(nullptr)
        , mBlendElapsed(0)
        , mBlendTime(0.2f)  // a default value
    {
    }

    DiK2Animation::~DiK2Animation()
    {
    }

    void DiK2Animation::Play(const DiString& name)
    {
        // TODO: make a queue or something
        if (mSource && mTarget)
            return;

        auto it = mClips.find(name);
        if (it == mClips.end())
        {
            DI_WARNING("Cannot find the animation: %s",name.c_str());
            return;
        }

        DiK2Clip* clip = it->second;
        clip->Cleanup();
        
        mBlendElapsed = 0;
        
        if (!mSource)
            mSource = clip;
        else if(clip != mSource)
            mTarget = clip;
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
        auto i = mClips.find(name);
        if (i != mClips.end())
        {
            DI_DELETE i->second;
        }
        DiK2Clip* c = DI_NEW DiK2Clip();
        mClips[name] = c;
        return c;
    }

    void DiK2Clip::Update(float deltaTime)
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
        mCurFrame = DiMath::Clamp(int(realFrame), 0, mNumFrames-1);
        mInterpFactor = realFrame - mCurFrame;
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
        int curFrame  = mCurFrame;
        int nextFrame = GetNextFrame();
        
        Trans& source = mKeyFrames->boneFrames[bone][curFrame];
        Trans& target = mKeyFrames->boneFrames[bone][nextFrame];
        
        // just linear interpolate for now
        output.rot = DiQuat::Nlerp(mInterpFactor, source.rot, target.rot,true);
        output.pos = source.pos + (target.pos - source.pos) * mInterpFactor;
        output.scale = source.scale + (target.scale - source.scale) * mInterpFactor;
    }

    DiK2Skeleton::DiK2Skeleton()
        : mRootNode(nullptr)
        , mRowData(nullptr)
        , mBoneMatrices(nullptr)
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