
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

#ifndef __SkeletonAnimationDef_H__
#define __SkeletonAnimationDef_H__

#include "SkeletonTrack.h"
#include "IdString.h"

namespace Demi
{
    class DI_GFX_API SkeletonAnimationDef : public DiBase
    {
        friend class SkeletonAnimation;
    protected:
        SkeletonTrackVec    mTracks;
        /** Number of frames. May not equal the number of keyframes
            (i.e. remain stationary at the end for a long time).
        */
        float                mNumFrames;
        float                mOriginalFrameRate;
        /** Converts bone index to consecutive slot (@see SkeletonAnimation::mBoneWeights).
            The parent level depth is in the last 8 bits
        */
        DiMap<IdString, size_t> mBoneToWeights;
        DiString mName;

        KfTransformArrayMemoryManager *mKfTransformMemoryManager;

        inline uint32 slotToBlockIdx( uint32 slotIdx ) const;
		inline uint32 blockIdxToSlotStart( uint32 blockIdx ) const;

        typedef DiVector<float> TimestampVec;
        typedef DiMap<size_t, TimestampVec> TimestampsPerBlock;

        /** Allocates enough memory in mKfTransformMemoryManager, creates all the mTracks
            (one per each entry in timestampsByBlock), and allocates all the keyframes
            from each track in a cache friendly manner, according to the usage pattern
            we'll be going to do.
        @param timestampsByBlock
            A map with block index as key; and as value an array of unique time stamps
            (a time stamp = one keyframe)
        @param frameRate
            The original recording framerate.
        */
        void allocateCacheFriendlyKeyframes( const TimestampsPerBlock &timestampsByBlock,
                                             float frameRate );

    public:
        SkeletonAnimationDef();
        ~SkeletonAnimationDef();

        void setName( const DiString &name ) { mName = name; }

        void build( const Skeleton *skeleton, const Animation *animation, float frameRate );
    };

    typedef DiVector<SkeletonAnimationDef> SkeletonAnimationDefVec;
}

#endif
