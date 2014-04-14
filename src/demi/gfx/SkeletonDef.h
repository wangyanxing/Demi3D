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

#ifndef __SkeletonDef_H__
#define __SkeletonDef_H__

#include "SkeletonAnimation.h"
#include "SkeletonAnimationDef.h"
#include "IdString.h"
#include "RawPtr.h"

#include "Math/Array/ArrayMatrixAf4x3.h"
#include "math/Array/KfTransform.h"

namespace Demi
{
    class DI_GFX_API SkeletonDef : public DiBase
    {
        friend class SkeletonInstance;
    public:
        struct BoneData
        {
            size_t      index;
            size_t      parent;
            DiVec3     vPos;
            DiQuat  qRot;
            DiVec3     vScale;
            String      name;
            uint8       bInheritOrientation:1;
            uint8       bInheritScale:1;

            BoneData( size_t _index, size_t _parent, const DiVec3 &_pos, const DiQuat &_rot,
                        const DiVec3 &_scale, const String &_name,
                        bool _inheritOrientation, bool _inheritScale ) :
                index( _index ),
                parent( _parent ),
                vPos( _pos ),
                qRot( _rot ),
                vScale( _scale ),
                name( _name ),
                bInheritOrientation( _inheritOrientation ),
                bInheritScale( _inheritScale )
            {
            }
        };
        typedef DiVector<BoneData> BoneDataVec;

        struct DepthLevelInfo
        {
            size_t  firstBoneIndex;
            size_t  numBonesInLevel;
            DepthLevelInfo() : firstBoneIndex( -1 ), numBonesInLevel( 0 ) {}
        };

        typedef DiVector<DepthLevelInfo> DepthLevelInfoVec;

    protected:
        typedef DiMap<IdString, size_t> BoneNameMap;

        BoneDataVec             mBones;
        BoneNameMap             mBoneIndexByName;
        SkeletonAnimationDefVec mAnimationDefs;

        RawSimdUniquePtr<KfTransform, MEMCATEGORY_ANIMATION>        mBindPose;
        RawSimdUniquePtr<ArrayMatrixAf4x3, MEMCATEGORY_ANIMATION>   mReverseBindPose;

        DepthLevelInfoVec       mDepthLevelInfoVec;

        /// Cached for SkeletonInstance::mUnusedNodes
        size_t                  mNumUnusedSlots;

        DiVector<DiList<size_t>> mBonesPerDepth;

        String                  mName;

    public:
        /** Constructs this Skeleton based on the old format's Skeleton. The frameRate parameter
            indicates at which framerate it was recorded (i.e. 15fps, 25fps) so that all keyframe
            time values end up rounded.
        @remarks
            If the framerate information has been lost, set it to 1.
        */
        SkeletonDef( const Skeleton *originalSkeleton, float frameRate );

        const String& getName(void) const                               { return mName; }

        const BoneDataVec& getBones(void) const                         { return mBones; }
        const SkeletonAnimationDefVec& getAnimationDefs(void) const     { return mAnimationDefs; }
        const DepthLevelInfoVec& getDepthLevelInfo(void) const          { return mDepthLevelInfoVec; }
        const KfTransform * getBindPose(void) const                     { return mBindPose.get(); }
        const RawSimdUniquePtr<ArrayMatrixAf4x3, MEMCATEGORY_ANIMATION>&
                                getReverseBindPose(void) const          { return mReverseBindPose; }
        void getBonesPerDepth( DiVector<size_t> &out ) const;

        /** Returns the total number of bone blocks to reach the given level. i.e On SSE2,
            If the skeleton has 1 root node, 3 children, and 5 children of children;
            then the total number of blocks is 1 + 1 + 2 = 4
        @param numLevels
            Level depth to reach. Must be in range [0; mDepthLevelInfoVec.size]
        */
        size_t getNumberOfBoneBlocks( size_t numLevels ) const;
    };
}

#endif
