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
#include "K2Asset.h"
#include "K2Clip.h"
#include "K2ModelSerial.h"

namespace Demi
{
    DiString DiK2ModelAsset::TYPE = "K2Model";

    DiK2ModelAsset::DiK2ModelAsset(const DiString& name)
        : DiAsset(name)
    {
    }

    DiK2ModelAsset::~DiK2ModelAsset()
    {
        for (auto i = mKeyFrames.begin(); i != mKeyFrames.end(); ++i)
        {
            DI_DELETE(i->second);
        }
        mKeyFrames.clear();
    }

    bool DiK2ModelAsset::Load(DiDataStreamPtr data)
    {
        DI_WARNING("We don't use this function to load k2 model stuff");
        return true;
    }

    bool DiK2ModelAsset::Load()
    {
        DiK2MdfSerial serial;

        DiString baseFolder = DiK2MdfSerial::GetK2MediaPath(mName);

        DiTimer timer;

        mBoneData = make_shared<DiK2BonesData>();

        serial.ParseMdf(baseFolder + "/model.mdf", this);
        serial.LoadModel(baseFolder + "/high.model", this);

        for (auto i = mAnims.begin(); i != mAnims.end(); ++i)
        {
            DiString clipFile = baseFolder + "/" + i->clip;
            serial.LoadClip(clipFile, &(*i), this);
        }

        double loadingTime = timer.GetElapse();
        DI_LOG("Loading time: %f", loadingTime);

        return true;
    }

    bool DiK2ModelAsset::LoadingComplete() const
    {
        return true;
    }

    void DiK2ModelAsset::AddKeyFrame(const DiString& animName, K2KeyFrames* kf)
    {
        auto i = mKeyFrames.find(animName);
        if (i != mKeyFrames.end())
        {
            DI_DELETE i->second;
        }
        mKeyFrames[animName] = kf;
    }

    void DiK2ModelAsset::CreateClipInstance(DiK2Animation* anim)
    {
        for (auto i = mAnims.begin(); i != mAnims.end(); ++i)
        {
            DiK2Clip* clip = anim->AddClip(i->name);
            clip->mFPS = i->fps;
            clip->mLoop = i->loop;
            clip->mNumFrames = i->numframes;
            clip->mKeyFrames = mKeyFrames[i->name];
        }
    }
}