
/********************************************************************
    File:       AnimationPose.h
    Creator:    demiwangya
*********************************************************************/

#ifndef AnimationPose_h__
#define AnimationPose_h__

#include "KeyFrame.h"
#include "ClipController.h"

namespace Demi
{

    class DiAnimationPose
    {
        typedef DiStrHash<DiAnimation*> AniBlenderMap;

    public:

        DiAnimationPose(DiSkeleton * pkSkeleton,DiClipControllerSet * pkClipSet);

        virtual                     ~DiAnimationPose();

    public:

        bool                        AddBasePoseBlender(DiAnimation * pkAnimation);

        bool                        AddAdditPoseBlender(DiAnimation * pkAnimation);

        void                        RemoveBasePoseBlender(const DiString& strBaseAni);

        void                        RemoveAdditPoseBlender(const DiString& strAdditAni);

        void                        ClearAllBasePoseBlender();

        void                        ClearAllAdditPoseBlender();

        void                        ApplySkeletonAnimPose(float fDeltaTime,const DiVector<float>* pkBoneWeightMask);

        inline    void              SetAdditLayerWeight(float fWeight) { mAdditLayerWeight = fWeight; }

        inline    float             GetAdditLayerWeight() { return mAdditLayerWeight; }

        inline    void              SetBaseLayerWeight(float fWeight) { mBaseLayerWeight = fWeight; }

        inline    float             GetBaseLayerWeight() { return mBaseLayerWeight; }

    protected:

        bool                        mKeepLastAni;    

        AniBlenderMap               mMapBaseBlenders;

        AniBlenderMap               mMapAdditBlenders;

        DiSkeleton*                 mSkeleton;

        DiClipControllerSet*        mClipSet;

        float                       mBaseLayerWeight;

        float                       mAdditLayerWeight;
    };
}

#endif // AnimationPose_h__
