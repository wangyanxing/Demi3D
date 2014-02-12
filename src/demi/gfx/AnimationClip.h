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

#ifndef DiAnimationClip_h__
#define DiAnimationClip_h__


#include "Spline.h"
#include "RotationSpline.h"

namespace Demi
{
    class DI_GFX_API DiTimeIndex
    {
    public:

        DiTimeIndex(float timePos)
            : mTimePos(timePos)
            , mKeyIndex(INVALID_KEY_INDEX)
        {
        }

        DiTimeIndex(float timePos, uint32 keyIndex)
            : mTimePos(timePos)
            , mKeyIndex(keyIndex)
        {
        }

        bool    HasKeyIndex(void) const
        {
            return mKeyIndex != INVALID_KEY_INDEX;
        }

        float    GetTimePos(void) const
        {
            return mTimePos;
        }

        uint32    GetKeyIndex(void) const
        {
            return mKeyIndex;
        }

    protected:

        float    mTimePos;

        uint32    mKeyIndex;

        static const uint32 INVALID_KEY_INDEX = (uint32)-1;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiAnimationClip
    {
    public:
        
        DiAnimationClip(DiAnimation* parent, uint32 handle);

        virtual                ~DiAnimationClip();

    public:

        DiAnimation*        GetParent() const { return mParent; }

        size_t              GetNumFrames()const;

        uint32              GetHandle()const { return mHandle; }

        DiKeyFrame*         GetKeyFrame(uint32 index) const;

        float               GetKeyFramesAtTime(const DiTimeIndex& timeIndex,
                                DiKeyFrame** keyFrame1, DiKeyFrame** keyFrame2,
                                unsigned short* firstKeyIndex = 0) const;

        DiKeyFrame*         CreateKeyFrame(float timePos);

        void                RemoveKeyFrame(uint32 index);

        void                RemoveAllKeyFrames(void);

        virtual void        Apply(const DiTimeIndex& timeIndex, 
                                float weight = 1.0, float scale = 1.0f) = 0;

        virtual void        KeyFrameDataChanged(void) const {}

        virtual void        Optimise(void) {}

        virtual void        GetInterpolatedKeyFrame(const DiTimeIndex& timeIndex,
                                DiKeyFrame* kf) const = 0;

        virtual void        CollectKeyFrameTimes(DiVector<float>& keyFrameTimes);

        virtual void        BuildKeyFrameIndexMap(const DiVector<float>& keyFrameTimes);

    protected:

        virtual DiKeyFrame* CreateKeyFrameImpl(float time) = 0;

    protected:

        typedef DiVector<DiKeyFrame*>    KeyFrameList;
        typedef DiVector<uint32>        KeyFrameIndexMap;

        KeyFrameIndexMap    mKeyFrameIndexMap;

        KeyFrameList        mKeyFrames;

        DiAnimation*        mParent;
        
        uint32              mHandle;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiNodeClip : public DiAnimationClip
    {
    public:
        
        DiNodeClip(DiAnimation* parent, unsigned int handle);

        DiNodeClip(DiAnimation* parent, unsigned int handle, 
            DiNode* targetNode);

        ~DiNodeClip();

    public:

        DiTransformKeyFrame*    CreateNodeKeyFrame(float timePos);
        
        DiTransformKeyFrame*    GetNodeKeyFrame(unsigned short index) const;

        DiNode*                 GetTargetNode(void) const{return mTargetNode;}

        void                    SetTargetNode(DiNode* node) ;

        void                    ApplyToNode(DiNode* node, const DiTimeIndex& timeIndex, float weight = 1.0, 
                                    float scale = 1.0f);

        void                    SetUseShortestRotationPath(bool useShortestPath);

        bool                    GetUseShortestRotationPath() const;

        void                    GetInterpolatedKeyFrame(const DiTimeIndex& timeIndex, DiKeyFrame* kf) const;

        void                    Apply(const DiTimeIndex& timeIndex, float weight = 1.0, float scale = 1.0f);

        void                    KeyFrameDataChanged(void) const;

        bool                    HasNonZeroKeyFrames(void) const;

        void                    Optimise(void);

        DiNodeClip*             Clone(DiAnimation* newParent) const;
        
        void                    ApplyBaseKeyFrame(const DiKeyFrame* base);
        
    protected:
        
        DiKeyFrame*             CreateKeyFrameImpl(float time);
        
        void                    BuildInterpolationSplines(void) const;

        struct Splines
        {
            DiSpline positionSpline;
            DiSpline scaleSpline;
            RotationalSpline rotationSpline;
        };

        DiNode*                mTargetNode;

        mutable Splines*       mSplines;

        mutable bool           mSplineBuildNeeded;

        mutable bool           mUseShortestRotationPath ;
    };
}

#endif
