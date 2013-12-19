
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
        mKeepLastAni(TRUE),
        mBaseLayerWeight(1.0f),
        mAdditLayerWeight(1.0f)
    {

    }

    DiAnimationPose::~DiAnimationPose()
    {

    }

    bool DiAnimationPose::AddBasePoseBlender( DiAnimation * pkAnimation )
    {
        if(!pkAnimation)
        {
            return FALSE;
        }

        AniBlenderMap::iterator itBaseBlender = mMapBaseBlenders.find(pkAnimation->GetName());
        if(itBaseBlender != mMapBaseBlenders.end())
        {
            DI_WARNING("已存在名为\"%s\"的动画集，添加基本层动画失败！",pkAnimation->GetName().c_str());
            return FALSE;
        }

        DiClipController * pkClip = mClipSet->GetClipController(pkAnimation->GetName());
        pkClip->SetEnabled(TRUE);
        pkClip->SetTimePosition(0.0f);

        mMapBaseBlenders[pkAnimation->GetName()] = pkAnimation;

        return TRUE;
    }

    bool DiAnimationPose::AddAdditPoseBlender( DiAnimation * pkAnimation )
    {
        if(!pkAnimation)
        {
            return FALSE;
        }

        AniBlenderMap::iterator itBaseBlender = mMapAdditBlenders.find(pkAnimation->GetName());
        if(itBaseBlender != mMapAdditBlenders.end())
        {
            DI_WARNING("已存在名为\"%s\"的动画集，添加混合层动画失败！",pkAnimation->GetName());
            return FALSE;
        }

        DiClipController * pkClip = mClipSet->GetClipController(pkAnimation->GetName());
        pkClip->SetEnabled(TRUE);
        pkClip->SetTimePosition(0.0f);

        mMapAdditBlenders[pkAnimation->GetName()] = pkAnimation;

        return TRUE;
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
            DI_WARNING("不存在名为\"%s\"的动画集，删除基本层动画失败！",strBaseAni);
            return ;
        }

        DiClipController * pkClip = mClipSet->GetClipController(strBaseAni);
        pkClip->SetTimeRatio(1.0f);
        pkClip->SetEnabled(FALSE);
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
            DI_WARNING("不存在名为\"%s\"的动画集，删除混合层动画失败！",strAdditAni);
            return ;
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

        /// 计算基动画混合后的比重
        float fFactor = 1.0f;
        float fTotalWeight = 0.0f;
        AniBlenderMap::iterator itAniBlender = mMapBaseBlenders.begin();
        for( ; itAniBlender != mMapBaseBlenders.end() ; ++ itAniBlender )
        {
            fTotalWeight += mClipSet->GetClipController(itAniBlender->first)->GetWeight();
        }

        if(fTotalWeight > 0.0f)
        {
            fFactor /= fTotalWeight;
        }

        /// 检查是否有加成混合
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

                UINT unBonesNum = mSkeleton->GetNumBones();
                for(UINT unHandle = 0 ; unHandle < unBonesNum ; ++ unHandle)
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
                    /// 层权重采用基动画的权重
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
            AniBlenderMap::iterator itAniBlender = mMapAdditBlenders.begin();
            while( itAniBlender != mMapAdditBlenders.end() )
            {
                pkClipCtrl = mClipSet->GetClipController(itAniBlender->first);
                {
                    fTotalWeight += pkClipCtrl->GetWeight();
                    ++ itAniBlender;
                }
            }

            if(fTotalWeight > 0.0f)
                fFactor /= fTotalWeight;

            pkAnim = NULL;
            itAniBlender = mMapAdditBlenders.begin();
            pkClipCtrl = NULL;
            fFinalWeight = 0.0f;
            pBone = NULL;
            pkNodeClip = NULL;
            for( ; itAniBlender != mMapAdditBlenders.end() ; ++ itAniBlender )
            {
                pkAnim = itAniBlender->second;
                pkClipCtrl = mClipSet->GetClipController(itAniBlender->first);
                pkClipCtrl->AddTime(fDeltaTime);
                if(pkAnim && pkClipCtrl)
                {
                    kTimeIndex = pkAnim->GetTimeIndex(pkClipCtrl->GetTimePosition());

                    UINT unBonesNum = mSkeleton->GetNumBones();
                    for(UINT unHandle = 0 ; unHandle < unBonesNum ; ++ unHandle)
                    {
                        fFinalWeight = pkClipCtrl->GetWeight() * fFactor;

                        fFinalWeight *= (1.0f - (*pkBoneWeightMask)[unHandle]);
                        /// 层权重采用基动画的权重
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



