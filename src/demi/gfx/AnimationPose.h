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
