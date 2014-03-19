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
#include "Model.h"
#include "K2ModelSerial.h"
#include "Command.h"
#include "ConsoleVariable.h"
#include "AssetManager.h"
#include "K2Asset.h"

namespace Demi
{
    DiK2Model::DiK2Model(const DiString& path)
        : mAnimation(DI_NEW DiK2Animation())
        , mNode(nullptr)
        , mName(path)
        , mSkeleton(nullptr)
    {
        Load(path);
    }

    DiK2Model::~DiK2Model()
    {
        DI_DELETE mAnimation;
        DI_DELETE mSkeleton;
    }

    DiCullNode* DiK2Model::CreateNode(DiSceneManager* sm)
    {
        if (mNode)
        {
            mNode->DetachAllObjects();
            DI_DELETE mNode;
        }
        mNode = sm->GetRootNode()->CreateChild();
        mNode->AttachObject(mMesh);
        return mNode;
    }

    void DiK2Model::Load(const DiString& path)
    {
        // get the asset
        mAsset = DiAssetManager::GetInstance().GetAsset<DiK2ModelAsset>(path);

        // create animation clips
        mAnimation = DI_NEW DiK2Animation();
        mAsset->CreateClipInstance(mAnimation);

        mMesh = make_shared<DiModel>(path, mAsset->GetMesh());

        mSkeleton = DI_NEW DiK2Skeleton();
        mSkeleton->CreateBones(mAsset->GetBoneData());
    }

    void DiK2Model::Update(float deltaTime)
    {
        mAnimation->Update(deltaTime);
    }
}