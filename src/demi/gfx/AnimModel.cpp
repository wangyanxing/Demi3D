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

namespace Demi
{
    DiAnimModel::DiAnimModel( const DiString& name, 
        const DiString& modelName, const DiString& motionName )
        :DiModel(name,modelName),
        mSkeleton(NULL),
        mRefSkeleton(false),
        mAttachSet(NULL),
        mRefAttachSet(false),
        mBoneMatrices(NULL),
        mRefBoneMatrics(false),
        mNumBoneMatrices(0),
        mClipSet(NULL),
        mRefClipSet(false),
        mLastUpdateBonesFrame(std::numeric_limits<uint64>::max()),
        mLastUpdateAnimFrame(std::numeric_limits<uint64>::max()),
        mAutoUpdateAnims(true),
        mHardwareSkining(false),
        mSpeed(1.0f)
    {
        mMotion = DiAssetManager::GetInstance().GetAsset<DiMotion>(motionName);

        Init();
    }

    DiAnimModel::DiAnimModel( const DiString& name,DiMeshPtr model, DiMotionPtr motion)
        :DiModel(name,model),
        mSkeleton(NULL),
        mRefSkeleton(false),
        mAttachSet(NULL),
        mRefAttachSet(false),
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
        Init();
    }

    DiAnimModel::DiAnimModel( const DiString& name, const DiString& modelName,const DiString& motionName,
                            DiSkeletonInstance * pkSkeleton,DiAttachSetInstance * pkAttachSet,DiMat4 * pkBoneMatrics,
                            DiClipControllerSet* pkClipSet)
        :DiModel(name,modelName),
        mSkeleton(pkSkeleton),
        mRefSkeleton(true),
        mAttachSet(pkAttachSet),
        mRefAttachSet(true),
        mBoneMatrices(pkBoneMatrics),
        mRefBoneMatrics(true),
        mNumBoneMatrices(0),
        mClipSet(pkClipSet),
        mRefClipSet(true),
        mLastUpdateBonesFrame(std::numeric_limits<uint64>::max()),
        mLastUpdateAnimFrame(std::numeric_limits<uint64>::max()),
        mAutoUpdateAnims(true),
        mHardwareSkining(false),
        mSpeed(1.0f)
    {
        mMotion = DiAssetManager::GetInstance().GetAsset<DiMotion>(motionName);

        Init();
    }

    DiAnimModel::DiAnimModel( const DiString& name,DiMeshPtr model,DiMotionPtr motion,
                              DiSkeletonInstance * pkSkeleton,DiAttachSetInstance * pkAttachSet,
                              DiMat4 * pkBoneMatrics,DiClipControllerSet* pkClipSet)
        :DiModel(name,model),
        mSkeleton(pkSkeleton),
        mRefSkeleton(true),
        mAttachSet(pkAttachSet),
        mRefAttachSet(true),
        mBoneMatrices(pkBoneMatrics),
        mRefBoneMatrics(true),
        mNumBoneMatrices(0),
        mClipSet(pkClipSet),
        mRefClipSet(true),
        mLastUpdateBonesFrame(std::numeric_limits<uint64>::max()),
        mLastUpdateAnimFrame(std::numeric_limits<uint64>::max()),
        mMotion(motion),
        mSpeed(1.0f)
    {
        Init();
    }


    DiAnimModel::~DiAnimModel()
    {
        if(!mRefSkeleton)
        {
            SAFE_DELETE(mSkeleton);
        }

        if(!mRefAttachSet)
        {
            SAFE_DELETE(mAttachSet);
        }

        if(!mRefClipSet)
        {
            SAFE_DELETE(mClipSet);
        }

        if(!mRefBoneMatrics)
        {
            SAFE_ARRAY_DELETE(mBoneMatrices);
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
            mSkeleton->GetBoneMatrices(mBoneMatrices,false);
            mLastUpdateBonesFrame = lfm;
            return true;
        }
        return false;
    }

    void DiAnimModel::Init()
    {
        if (mMotion)
        {
            if (mMotion->GetSkeleton())
            {
                if(!mSkeleton)
                {
                    mSkeleton    = DI_NEW DiSkeletonInstance(mMotion->GetSkeleton());
                    mRefSkeleton = false;
                }

                // link the motion and skeleton
                mMotion->AssociateNodeAnimToSkeleton(mSkeleton);

                mNumBoneMatrices = (uint32)mSkeleton->GetNumBones();

                if(!mBoneMatrices)
                {
                    mBoneMatrices = DI_NEW DiMat4[mNumBoneMatrices];
                    mRefBoneMatrics = false;
                }

                for (auto it = mSubModels.begin(); it != mSubModels.end(); ++it)
                {
                    (*it)->mBoneNum = mNumBoneMatrices;
                    (*it)->mBoneMatrices = mBoneMatrices;
                }
            }

            if (mMotion->GetAttachSet())
            {
                if(!mAttachSet)
                {
                    mAttachSet    = DI_NEW DiAttachSetInstance(mMotion->GetAttachSet(),mSkeleton);
                    mRefAttachSet = false;
                }

                mMotion->AssociateNodeAnimToAttachSet(mAttachSet);
            }

            if(!mClipSet)
            {
                InitClipSet();
                mRefClipSet = false;
            }
        }
    }

    void DiAnimModel::CullingUpdate( DiRenderBatchGroup* group,DiCamera* cam)
    {
        UpdateAnimation();

        DiModel::CullingUpdate(group,cam);
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
            {
                CacheBoneMatrices();
            }

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