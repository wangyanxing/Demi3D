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
        
        if (!mSource)
            mSource = clip;
        else
            mTarget = clip;
    }

    void DiK2Animation::Update(float deltaTime)
    {
        if (!mSource && !mTarget)
            return;

        if (mSource)
            mSource->Update(deltaTime);
        // TODO

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
        mCurFrame = (mTime / length) * mNumFrames;
        DI_ASSERT(mCurFrame < mNumFrames);
    }

    DiK2Clip::DiK2Clip() : 
          mLoop(false)
        , mCurFrame(0)
        , mFPS(0)
        , mNumFrames(0)
        , mTime(0)
    {
    }

    DiK2Clip::~DiK2Clip()
    {
    }

    void DiK2Clip::Apply(DiK2Skeleton* skeleton)
    {
        for (auto it = mKeyFrames->boneFrames.begin(); it != mKeyFrames->boneFrames.end(); ++it)
        {
            const DiString& boneName = it->first;
            if (!skeleton->HasBone(boneName))
                continue;
         
            DiNode* bone = skeleton->GetBone(boneName);
            Trans& t = it->second[mCurFrame];
            bone->SetOrientation(t.rot);
            bone->SetPosition(t.pos);
            bone->SetScale(t.scale);
        }
    }

    DiK2Skeleton::DiK2Skeleton()
        : mRootNode(nullptr)
        , mRowData(nullptr)
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
    }
}