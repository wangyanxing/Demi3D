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
#include "AnimModel.h"
#include "AssetManager.h"
#include "GfxDriver.h"
#include "Skeleton.h"
#include "AttachSet.h"
#include "CullNode.h"
#include "ClipController.h"
#include "Animation.h"
#include "AnimationClip.h"
#include "SubModel.h"
#include "RenderWindow.h"
#include "PostController.h"
#include "AlignedAllocator.h"

namespace Demi
{
    DiAnimModel::DiAnimModel( const DiString& name, 
        const DiString& modelName, const DiString& motionName )
        :DiModel(name),
        mSkeleton(NULL),
        mRefSkeleton(false),
        mBoneMatrices(nullptr),
        mBoneTransforms(nullptr),
        mRefBoneMatrics(false),
        mNumBoneMatrices(0),
        mClipSet(NULL),
        mRefClipSet(false),
        mLastUpdateBonesFrame(std::numeric_limits<uint64>::max()),
        mLastUpdateAnimFrame(std::numeric_limits<uint64>::max()),
        mAutoUpdateAnims(true),
        mSpeed(1.0f)
    {
        mMesh = DiAssetManager::GetInstance().GetAsset<DiMesh>(modelName, true);
        mMotion = DiAssetManager::GetInstance().GetAsset<DiMotion>(motionName);

        Init();
    }

    DiAnimModel::DiAnimModel( const DiString& name,DiMeshPtr model, DiMotionPtr motion)
        :DiModel(name),
        mSkeleton(NULL),
        mRefSkeleton(false),
        mBoneMatrices(NULL),
        mRefBoneMatrics(false),
        mNumBoneMatrices(0),
        mClipSet(NULL),
        mRefClipSet(false),
        mLastUpdateBonesFrame(std::numeric_limits<uint64>::max()),
        mLastUpdateAnimFrame(std::numeric_limits<uint64>::max()),
        mMotion(motion),
        mSpeed(1.0f)
    {
        mMesh = model;
        Init();
    }

    DiAnimModel::~DiAnimModel()
    {
        if(!mRefSkeleton)
        {
            SAFE_DELETE(mSkeleton);
        }

        if(!mRefClipSet)
        {
            SAFE_DELETE(mClipSet);
        }

        if(!mRefBoneMatrics)
        {
            SAFE_ARRAY_DELETE(mBoneMatrices);
            if (mBoneTransforms)
            {
                DEMI_FREE_SIMD(mBoneTransforms);
            }
        }
    }

    bool DiAnimModel::CacheBoneMatrices()
    {
        unsigned long lfm = Driver->GetFrameNum();
        if(lfm != mLastUpdateBonesFrame && mSkeleton)
        {
            mMotion->ApplySkeletonAnimation(mSkeleton,mClipSet);
            mSkeleton->UpdateTransforms();
            UpdatePostControllers();
            if(mHardwareSkining)
                mSkeleton->GetBoneMatrices(mBoneMatrices,false);
            else
                mSkeleton->GetBoneMatrices(mBoneTransforms,false);
            mLastUpdateBonesFrame = lfm;
            return true;
        }
        return false;
    }

    void DiAnimModel::Init()
    {
        if (mMotion && mMotion->GetSkeleton())
        {
            if (!mSkeleton)
            {
                mSkeleton = DI_NEW DiSkeletonInstance(mMotion->GetSkeleton());
                mRefSkeleton = false;
            }

            // link the motion and skeleton
            mMotion->AssociateNodeAnimToSkeleton(mSkeleton);

            mNumBoneMatrices = (uint32)mSkeleton->GetNumBones();

            if (!mBoneMatrices)
            {
                mBoneMatrices = DI_NEW DiMat4[mNumBoneMatrices];
                mRefBoneMatrics = false;
                mBoneTransforms = (btTransform*)DEMI_MALLOC_SIMD(mNumBoneMatrices * sizeof(btTransform));
            }

            mHardwareSkining = mNumBoneMatrices <= MAX_BONE_NUM;
        }

        InitModel();

        for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
        {
            (*it)->mBoneNum = mNumBoneMatrices;
            (*it)->mBoneMatrices = mBoneMatrices;
            (*it)->mBoneTransforms = mBoneTransforms;
        }

        if (!mClipSet)
        {
            InitClipSet();
            mRefClipSet = false;
        }
    }

    void DiAnimModel::Update(DiCamera*)
    {
        UpdateAnimation();
    }

    void DiAnimModel::UpdateAnimation()
    {
        if (mAutoUpdateAnims && mClipSet)
        {
            ConstEnabledClipsIt it = mClipSet->GetEnabledClipsIterator();
            while (it.HasMoreElements())
            {
                DiClipController* cc = it.GetNext();
                cc->AddTime(Driver->GetDeltaSecond() * mSpeed);
            }
        }

        bool animationDirty =
            (mLastUpdateAnimFrame != mClipSet->GetDirtyFrameNumber());

        if (animationDirty)
        {
            if (HasSkeleton())
                CacheBoneMatrices();

            // software skinning
            if (!mHardwareSkining)
            {
                for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
                {
                    (*it)->SoftwareVertBlend();
                }
            }

            mLastUpdateAnimFrame = mClipSet->GetDirtyFrameNumber();
        }
    }

    void DiAnimModel::UpdateAnimation(float fTimePos)
    {
        if (mClipSet)
        {
            ConstEnabledClipsIt it = mClipSet->GetEnabledClipsIterator();
            while (it.HasMoreElements())
            {
                DiClipController* cc = it.GetNext();
                cc->SetTimePosition(fTimePos);
            }
        }

        bool animationDirty =
            (mLastUpdateAnimFrame != mClipSet->GetDirtyFrameNumber());

        if (animationDirty)
        {
            if (HasSkeleton())
                CacheBoneMatrices();

            mLastUpdateAnimFrame = mClipSet->GetDirtyFrameNumber();
        }
    }

    void DiAnimModel::InitClipSet()
    {
        mClipSet = DI_NEW DiClipControllerSet();

        DiMotion::AnimationListIterator i = mMotion->GetAnimations();
        while (i.HasMoreElements())
        {
            DiAnimation* anim = i.GetNext();
            const DiString& animName = anim->GetName();
            mClipSet->CreateClipController(animName, 0.0, anim->GetLength());
        }
    }

    DiString& DiAnimModel::GetType()
    {
        static DiString type = "AnimModel";
        return type;
    }

    void DiAnimModel::AddPostController( DiPostController* postctr )
    {
        mPostControllers.push_back(postctr);
    }

    void DiAnimModel::UpdatePostControllers()
    {
        for (PostControllers::iterator it = mPostControllers.begin(); 
            it != mPostControllers.end(); ++it)
        {
            (*it)->Update(0);
        }
    }
}