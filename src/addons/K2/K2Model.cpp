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
#include "K2Model.h"
#include "K2Clip.h"
#include "CullNode.h"
#include "SceneManager.h"
#include "SubModel.h"
#include "Model.h"
#include "K2ModelSerial.h"
#include "Command.h"
#include "ConsoleVariable.h"
#include "AssetManager.h"
#include "K2Asset.h"
#include "ShaderManager.h"

#ifdef ENABLE_K2_ANIM_DEBUGGER
#   include "DebugHelper.h"
#endif

namespace Demi
{
    DiK2Model::DiK2Model(const DiString& path)
        : DiModel(path)
        , mAnimation(DI_NEW DiK2Animation())
        , mSkeleton(nullptr)
    {
        Load(path);
        Init();
        PostLoad();
    }

    DiK2Model::~DiK2Model()
    {
        DI_DELETE mAnimation;
        DI_DELETE mSkeleton;
    }

    void DiK2Model::Load(const DiString& path)
    {
        // get the asset
        mAsset = DiAssetManager::GetInstance().GetAsset<DiK2ModelAsset>(path);

        // create animation clips
        mAnimation = DI_NEW DiK2Animation();
        mAsset->CreateClipInstance(mAnimation);

        mMesh = mAsset->GetMesh();

        mSkeleton = DI_NEW DiK2Skeleton();
        mSkeleton->CreateBones(mAsset->GetBoneData());
    }

    void DiK2Model::Update(DiCamera*)
    {
    }

    void DiK2Model::PostLoad()
    {
        for (uint32 i = 0; i < GetNumSubModels(); ++i)
        {
            DiSubModel* sm = GetSubModel(i);
            sm->mBoneNum = mSkeleton->GetNumBones();
            sm->mBoneMatrices = mSkeleton->GetBoneMatrices();
        }
    }

    void DiK2Model::UpdateAnimation(float delta)
    {
        mAnimation->Update(delta);
        mSkeleton->Apply(mAnimation);
        mSkeleton->CacheBoneMatrices();
    }
}