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

#ifndef Animation_h__
#define Animation_h__

#include "AnimationClip.h"
#include "ClipController.h"

namespace Demi
{
    class DI_GFX_API DiAnimation
    {
    public:

        DiAnimation(const DiString& name, float length);

        virtual ~DiAnimation();

    public:

        // Interpolation mode
        enum InterMode
        {
            IM_LINEAR,
            IM_SPLINE
        };
        
        enum RotationInterMode
        {
            RIM_LINEAR,
            RIM_SPHERICAL
        };

        typedef DiMap<uint32, DiNodeClip*>        NodeClips;
        typedef DiConstMapIterator<NodeClips>     NodeClipsIterator;

        typedef DiStrHash<DiNodeClip*>            AttachClips;
        typedef DiConstMapIterator<AttachClips>   AttachClipsIterator;

    public:

        const DiString&     GetName(void) const{return mName;}

        void                SetName(const DiString& strNewName) {mName = strNewName;}

        float               GetLength() const { return mLength; }

        void                SetLength(float val) { mLength = val; }

        DiNodeClip*         CreateNodeClip(uint32 handle);

        DiNodeClip*         CreateNodeClip(uint32 handle,DiNode* node);

        size_t              GetNumNodeClips(void) const{return mNodeClips.size();}

        DiNodeClip*         GetNodeClip(uint32 handle) const;

        bool                HasNodeClip(uint32 handle) const;

        void                NodeClipAttachToSkeleton(DiSkeleton* skeleton);

        void                NodeClipAttachToAttachSet(DiAttachSet* attachset);

        void                DestroyNodeClip(uint32 handle);

        void                DestroyAllNodeClips();

        DiNodeClip*         CreateAttachClip(DiString strAttachName);

        DiNodeClip*         CreateAttachClip(DiString strAttachName, DiNode * pkNode);

        inline    size_t    GetNumAttachClips()    const { return mAttachClips.size(); }

        bool                HasAttachClip(DiString strAttachName);

        void                DestroyAttachClip(DiString strAttachName);

        void                DestroyAllAttachClips();

        void                DestroyAllClips();

        void                SetRotationInterMode(RotationInterMode im);

        RotationInterMode   GetRotationInterMode(void) const{return mRotationInterMode;}

        InterMode           GetInterMode() const{return mInterMode;}

        void                SetInterMode(InterMode im);

        void                KeyFrameListChanged(void) { mKeyFrameTimesDirty = true; }

        DiTimeIndex         GetTimeIndex(float timePos) const;

        void                ApplySkeleton(DiSkeleton* skel, DiTimeIndex& timeIndex, float weight, float scale);

        void                ApplySkeleton(DiSkeleton* skeleton, DiTimeIndex& timeIndex, float weight,
                                        const DiClipController::BoneBlendMask * blendMask, float scale);

        void                ApplyAttachSet(DiAttachSet* attachset, DiTimeIndex& timeIndex, float weight, float scale);

        void                ApplyAttachSet(DiAttachSet* attachset, DiTimeIndex& timeIndex, float weight,
                                        const DiClipController::AttachBlendMask& blendMask, float scale);


        NodeClipsIterator    GetNodeClipsIterator()const;

        AttachClipsIterator  GetAttachClipsIterator() const;

    protected:

        static InterMode            DEFAULT_INTER_MODE;

        static RotationInterMode    DEFAULT_ROTATION_INTER_MODE;

        void                        BuildKeyFrameTimeList(void) const;

    protected:

        NodeClips                   mNodeClips;

        AttachClips                 mAttachClips;

        DiString                    mName;

        float                       mLength;
        
        InterMode                   mInterMode;

        RotationInterMode           mRotationInterMode;

        mutable bool                mKeyFrameTimesDirty;

        typedef DiVector<float> KeyFrameTimeList;
        mutable KeyFrameTimeList    mKeyFrameTimes;
    };
}

#endif // Animation_h__