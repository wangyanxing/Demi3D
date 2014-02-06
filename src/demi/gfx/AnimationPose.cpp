
#include "GfxPch.h"
#include "AnimationPose.h"
#include "AnimationClip.h"
#include "Animation.h"
#include "Bone.h"
#include "Skeleton.h"
#include "GfxDriver.h"

namespace Demi
{
    DiAnimationPose::DiAnimationPose( DiSkeleton * pkSkeleton,DiClipControllerSet * pkClipSet )
        : mSkeleton(pkSkeleton),
        mClipSet(pkClipSet),
        mKeepLastAni(true),
        mBaseLayerWeight(1.0f),
        mAdditLayerWeight(1.0f)
    {

    }

    DiAnimationPose::~DiAnimationPose()
    {

    }

    bool DiAnimationPose::AddBasePoseBlender( DiAnimation * animation )
    {
        if(!animation)
        {
            return false;
        }

        AniBlenderMap::iterator itBaseBlender = mMapBaseBlenders.find(animation->GetName());
        if(itBaseBlender != mMapBaseBlenders.end())
        {
            DI_WARNING("The animation %s has already existed", animation->GetName().c_str());
            return false;
        }

        DiClipController * pkClip = mClipSet->GetClipController(animation->GetName());
        pkClip->SetEnabled(true);
        pkClip->SetTimePosition(0.0f);

        mMapBaseBlenders[animation->GetName()] = animation;

        return true;
    }

    bool DiAnimationPose::AddAdditPoseBlender( DiAnimation * animation )
    {
        if(!animation)
        {
            return false;
        }

        AniBlenderMap::iterator itBaseBlender = mMapAdditBlenders.find(animation->GetName());
        if(itBaseBlender != mMapAdditBlenders.end())
        {
            DI_WARNING("The animation %s has already existed", animation->GetName().c_str());
            return false;
        }

        DiClipController * pkClip = mClipSet->GetClipController(animation->GetName());
        pkClip->SetEnabled(true);
        pkClip->SetTimePosition(0.0f);

        mMapAdditBlenders[animation->GetName()] = animation;

        return true;
    }

    void DiAnimationPose::RemoveBasePoseBlender( const DiString& strBaseAni )
    {
        if(strBaseAni.empty())
        {
            return ;
        }

        AniBlenderMap::iterator itBaseBlender = mMapBaseBlenders.find(strBaseAni);
        if(itBaseBlender == mMapBaseBlenders.end())
        {
            DI_WARNING("Cannot find and delete the animation %s", strBaseAni.c_str());
            return ;
        }

        DiClipController * pkClip = mClipSet->GetClipController(strBaseAni);
        pkClip->SetTimeRatio(1.0f);
        pkClip->SetEnabled(false);
        mMapBaseBlenders.erase(itBaseBlender);
        pkClip->SetWeight(1.0f);

    }

    void DiAnimationPose::RemoveAdditPoseBlender( const DiString& strAdditAni )
    {
        if(strAdditAni.empty())
        {
            return ;
        }

        AniBlenderMap::iterator itAdditBlender = mMapAdditBlenders.find(strAdditAni);
        if(itAdditBlender == mMapAdditBlenders.end())
        {
            DI_WARNING("Cannot find and delete the animation %s", strAdditAni.c_str());
            return;
        }

        DiClipController * pkClip = mClipSet->GetClipController(strAdditAni);
        pkClip->SetTimeRatio(1.0f);
        mMapAdditBlenders.erase(itAdditBlender);
        pkClip->SetWeight(1.0f);

    }

    void DiAnimationPose::ClearAllBasePoseBlender()
    {
        if(mMapBaseBlenders.empty())
        {
            return;
        }

        mMapBaseBlenders.clear();
    }

    void DiAnimationPose::ClearAllAdditPoseBlender()
    {
        if(mMapAdditBlenders.empty())
        {
            return;
        }

        mMapAdditBlenders.clear();
    }

    void DiAnimationPose::ApplySkeletonAnimPose(float fDeltaTime, const DiVector<float>* pkBoneWeightMask )
    {
        if(mMapBaseBlenders.empty())
        {
            return;
        }

        float fFactor = 1.0f;
        float fTotalWeight = 0.0f;
        auto itAniBlender = mMapBaseBlenders.begin();
        for( ; itAniBlender != mMapBaseBlenders.end() ; ++ itAniBlender )
        {
            fTotalWeight += mClipSet->GetClipController(itAniBlender->first)->GetWeight();
        }

        if(fTotalWeight > 0.0f)
        {
            fFactor /= fTotalWeight;
        }

        bool bHasAdditive = (pkBoneWeightMask && !mMapAdditBlenders.empty() );

        DiAnimation * pkAnim = NULL;
        itAniBlender = mMapBaseBlenders.begin();
        DiTimeIndex kTimeIndex(0.0,0);
        DiClipController * pkClipCtrl = NULL;
        float fFinalWeight = 0.0f;
        DiBone * pBone = NULL;
        DiNodeClip * pkNodeClip = NULL;
        for( ; itAniBlender != mMapBaseBlenders.end() ; ++ itAniBlender )
        {
            pkAnim = itAniBlender->second;
            pkClipCtrl = mClipSet->GetClipController(itAniBlender->first);
            
            pkClipCtrl->AddTime(fDeltaTime);
            
            if(pkAnim && pkClipCtrl)
            {
                kTimeIndex = pkAnim->GetTimeIndex(pkClipCtrl->GetTimePosition());

                uint32 unBonesNum = mSkeleton->GetNumBones();
                for(uint32 unHandle = 0 ; unHandle < unBonesNum ; ++ unHandle)
                {
                    fFinalWeight = pkClipCtrl->GetWeight() * fFactor;
                    if(bHasAdditive && mAdditLayerWeight > 0.0f)
                    {
                        //fFinalWeight *= (*pkBoneWeightMask)[unHandle];
                        if( (*pkBoneWeightMask)[unHandle] < 1.0f)
                        {
                            fFinalWeight *= DiMath::Clamp((*pkBoneWeightMask)[unHandle] + (1.0f - mAdditLayerWeight) ,0.0f ,1.0f);
                        }
                        else
                        {
                            fFinalWeight *= (*pkBoneWeightMask)[unHandle];
                        }
                    }

                    pBone = mSkeleton->GetBone(unHandle);

                    pkNodeClip = pkAnim->GetNodeClip(unHandle);

                    fFinalWeight *= mBaseLayerWeight;

                    pkNodeClip->ApplyToNode( pBone, kTimeIndex, fFinalWeight );
                }
            }
        }

        if(bHasAdditive)
        {
            fFactor = 1.0f;
            fTotalWeight = 0.0f;
            AniBlenderMap::iterator itBlender = mMapAdditBlenders.begin();
            while( itBlender != mMapAdditBlenders.end() )
            {
                pkClipCtrl = mClipSet->GetClipController(itBlender->first);
                {
                    fTotalWeight += pkClipCtrl->GetWeight();
                    ++itBlender;
                }
            }

            if(fTotalWeight > 0.0f)
                fFactor /= fTotalWeight;

            pkAnim = NULL;
            itBlender = mMapAdditBlenders.begin();
            pkClipCtrl = NULL;
            fFinalWeight = 0.0f;
            pBone = NULL;
            pkNodeClip = NULL;
            for( ; itBlender != mMapAdditBlenders.end() ; ++ itBlender )
            {
                pkAnim = itBlender->second;
                pkClipCtrl = mClipSet->GetClipController(itBlender->first);
                pkClipCtrl->AddTime(fDeltaTime);
                if(pkAnim && pkClipCtrl)
                {
                    kTimeIndex = pkAnim->GetTimeIndex(pkClipCtrl->GetTimePosition());

                    uint32 unBonesNum = mSkeleton->GetNumBones();
                    for(uint32 unHandle = 0 ; unHandle < unBonesNum ; ++ unHandle)
                    {
                        fFinalWeight = pkClipCtrl->GetWeight() * fFactor;
                        fFinalWeight *= (1.0f - (*pkBoneWeightMask)[unHandle]);

                        pBone = mSkeleton->GetBone(unHandle);
                        pkNodeClip = pkAnim->GetNodeClip(unHandle);

                        fFinalWeight *= mAdditLayerWeight;
                        pkNodeClip->ApplyToNode( pBone, kTimeIndex, fFinalWeight );
                    }
                }
            }
        }

    }

}



