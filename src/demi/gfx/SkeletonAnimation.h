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

#ifndef __SkeletonAnimation_H__
#define __SkeletonAnimation_H__

#include "SkeletonTrack.h"
#include "IdString.h"
#include "RawPtr.h"
#include "FastArray.h"

namespace Demi
{
    class SkeletonAnimationDef;
    class SkeletonInstance;

    typedef DiVector<KeyFrameRigVec::const_iterator> KnownKeyFramesVec;

    /// Represents the instance of a Skeletal animation based on its definition
    class DI_GFX_API SkeletonAnimation : public DiBase
    {
        SkeletonAnimationDef const  *mDefinition;
    protected:
        RawSimdUniquePtr<ArrayFloat, MEMCATEGORY_ANIMATION> mBoneWeights;
        float                    mCurrentFrame;
    public:
        float                    mFrameRate;     // Playback framerate
        float                    mWeight;
        FastArray<size_t> const *mSlotStarts;   // One per parent depth level
        bool                    mLoop;
        bool                    mEnabled;
        SkeletonInstance        *mOwner;
    protected:
        IdString                mName;

        /// One per track
        KnownKeyFramesVec       mLastKnownKeyFrames;

    public:
        SkeletonAnimation( const SkeletonAnimationDef *definition, const FastArray<size_t> *slotStarts,
                            SkeletonInstance *owner );

        /// Call this function before using the animation!
        void initialize(void);

        /** Plays the animation forward (or backwards if negative)
        @param time
            Time to advance, in seconds
        */
        void addTime( float time )                                   { addFrame( time * mFrameRate ); }

        /** Plays the animation forward (or backwards if negative)
        @param frames
            Frames to advance, in frames
        */
        void addFrame( float frames );

        /// Gets the current animation time, in seconds. Prefer using getCurrentFrame
        float getCurrentTime( float time ) const                      { return mCurrentFrame / mFrameRate; }

        /// Gets the current animation frame, in frames.
        float getCurrentFrame( float time ) const                     { return mCurrentFrame; }

        IdString getName(void) const                                { return mName; }

        /** Loop setting. Looped animations will wrap back to zero when reaching the animation length
            or go back to the animation length if playing backwards.
            Non-looped animations will stop at the animation length (or at 0 if backwards) but won't
            be disabled.
        */
        void setLoop( bool bLoop )                                  { mLoop = bLoop; }

        /** Returns current loop setting. @See setLoop.
        */
        bool getLoop(void) const                                    { return mLoop; }

        /** Sets the per-bone weight to a particular bone. Useful for fine control
            over animation strength on a set of nodes (i.e. an arm)
        @remarks
            By default all bone weights are set to 1.0
        @param boneName
            The name of the bone to set. If this animation doesn't affect that bone (or the
            name is invalid) this function does nothing.
        @param weight
            Weight to apply to this particular bone. Note that the animation multiplies this
            value against the global mWeight to obtain the final weight.
            Normal range is between [0; 1] but not necessarily.
        */
        void setBoneWeight( IdString boneName, float weight );

        /** Gets the current per-bone weight of a particular bone.
        @param boneName
            The name of the bone to get. If this animation doesn't affect that bone (or the
            name is invalid) this function returns 0.
        @return
            The weight of the specified bone. 0 if not found.
        */
        float getBoneWeight( IdString boneName ) const;

        /** Gets a pointer current per-bone weight of a particular bone. Useful if you intend
            to have read/write access to this value very often.
        @remarks
            If returnPtr is the return value to bone[0], do not assume that returnPtr+1
            affects bone[1] or even any other bone. Doing so the behavior is underfined
            and most likely you could be affecting the contents of other SkeletonInstances.
        @param boneName
            The name of the bone to get. If this animation doesn't affect that bone (or the
            name is invalid) this function returns a null pointer.
        @return
            The pointer to the bone weight of the specified bone. Null pointer if not found.
        */
        float* getBoneWeightPtr( IdString boneName );

        /// Enables or disables this animation. A disabled animation won't be processed at all.
        void setEnabled( bool bEnable );
        bool getEnabled(void) const                                 { return mEnabled; }

        void _applyAnimation( const TransformArray &boneTransforms );
    };
}

#endif
