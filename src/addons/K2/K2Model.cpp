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

#ifdef ENABLE_K2_ANIM_DEBUGGER
        if (!mDebugger)
        {
            mDebugger = make_shared<DiDebugHelper>();
            DiMaterialPtr helpermat = DiMaterial::QuickCreate(
                "basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
            helpermat->SetDepthCheck(false);
            mDebugger->SetMaterial(helpermat);
        }
        mNode->AttachObject(mDebugger);
#endif

        return mNode;
    }

    DiCullNode* DiK2Model::CreateNode(DiCullNode* parent)
    {
        if (mNode)
        {
            mNode->DetachAllObjects();
            DI_DELETE mNode;
        }
        mNode = parent->CreateChild();
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

        for (uint32 i = 0; i < mMesh->GetNumSubModels(); ++i)
        {
            DiSubModel* sm = mMesh->GetSubModel(i);
            sm->mBoneNum = mSkeleton->GetNumBones();
            sm->mBoneMatrices = mSkeleton->GetBoneMatrices();
        }
    }

    void DiK2Model::Update(float deltaTime)
    {
        mAnimation->Update(deltaTime);
        mSkeleton->Apply(mAnimation);
        mSkeleton->CacheBoneMatrices();

#ifdef ENABLE_K2_ANIM_DEBUGGER
        _UpdateDebugger();
#endif
    }

#ifdef ENABLE_K2_ANIM_DEBUGGER
    void DiK2Model::_UpdateDebugger()
    {
        mDebugger->Clear();

        uint32 numBones = mSkeleton->GetNumBones();
        for (uint32 i = 0; i < numBones; ++i)
        {
            DiNode* n = mSkeleton->GetBone(i);
            if (n->GetParent())
            {
                DiNode* p = n->GetParent();
                mDebugger->AddLine(n->GetDerivedPosition(), p->GetDerivedPosition(), DiColor::Red);
            }

            DiVec3 pos = n->GetDerivedPosition();
            DiAABB b;
            DiVec3 minv = pos - DiVec3(0.1, 0.1, 0.1);
            DiVec3 maxv = pos + DiVec3(0.1, 0.1, 0.1);
            b.SetExtents(minv, maxv);

            mDebugger->AddBoundingBox(b, DiColor::White);
        }
    }
#endif
}