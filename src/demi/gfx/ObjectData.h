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

/// This file is adapted from Ogre 2.0 (unstable version)
#ifndef __ObjectData_H__
#define __ObjectData_H__

//#include "ArrayMatrix4.h"
#include "Math/Array/ArrayAabb.h"
#include "ArrayMemoryManager.h"

namespace Demi
{
    /** Represents the transform of a single object, arranged in SoA (Structure of Arrays) */
    struct ObjectData
    {
        /// Which of the packed values is ours. Value in range [0; 4) for SSE2
        unsigned char       mIndex;

        /// Holds the pointers to each parent. Ours is mParents[mIndex]
        DiNode              **mParents;

        /// The movable object that owns this ObjectData. Ours is mOwner[mIndex]
        DiTransUnitPtr       *mOwner;

        /** Bounding box in local space. It's argueable whether it should be like this, or pointer
            to a shared aabb (i.e. mesh aabb) to save RAM at the cost of another level of indirection,
            and converting AoS to SoA, but it's more convenient for SIMD processing to store each local
            aabb in the movable object (maximize throughput).
        */
        ArrayAabb   * RESTRICT_ALIAS    mLocalAabb;

        /// Bounding box in world space.
        ArrayAabb   * RESTRICT_ALIAS    mWorldAabb;

        //ArraySphere   * RESTRICT_ALIAS    mWorldSphere;

        /** @See mLocalAabb
        @remarks
            Its center is at mLocalAabb's center, not at the position or origin
        */
        float        * RESTRICT_ALIAS    mLocalRadius;

        /** Ours is mWorldRadius[mIndex]. It is the local radius transformed by scale
            An ArraySphere out of mWorldAabb & WorldRadius for computations
        @remarks
            Its center is at mWorldAabb's, not at the derived position
        */
        float        * RESTRICT_ALIAS    mWorldRadius;

        /// Upper distance to still render. Ours is mUpperDistance[mIndex]
        float        * RESTRICT_ALIAS    mUpperDistance;

        /// Flags determining whether this object is visible (compared to SceneManager mask)
        uint32      * RESTRICT_ALIAS    mVisibilityFlags;

        /// Flags determining whether this object is included / excluded from scene queries
        uint32      * RESTRICT_ALIAS    mQueryFlags;

        /** The light mask defined for this movable. This will be taken into consideration when
            deciding which light should affect this movable
        @remarks
            Since Lights object don't use this value, we reserve the value to either 0xffffffff
            or 0; using 0 to avoid adding null mOwner pointers to vectors (due to the SoA nature
            of working 4 items at a time, even if we just have 2 or 3)
        */
        uint32      * RESTRICT_ALIAS    mLightMask;

        ObjectData() :
            mIndex( 0 ),
            mParents( 0 ),
            mLocalAabb( 0 ),
            mWorldAabb( 0 ),
            mWorldRadius( 0 ),
            mUpperDistance( 0 ),
            mVisibilityFlags( 0 ),
            mQueryFlags( 0 ),
            mLightMask( 0 )
        {
        }

        /// @copydoc Transform::copy
        void copy( const ObjectData &inCopy )
        {
            mParents[mIndex]    = inCopy.mParents[inCopy.mIndex];
            mOwner[mIndex]      = inCopy.mOwner[inCopy.mIndex];

            Aabb tmp;
            inCopy.mLocalAabb->getAsAabb( tmp, inCopy.mIndex );
            mLocalAabb->setFromAabb( tmp, mIndex );

            inCopy.mWorldAabb->getAsAabb( tmp, inCopy.mIndex );
            mWorldAabb->setFromAabb( tmp, mIndex );

            mLocalRadius[mIndex]        = inCopy.mLocalRadius[inCopy.mIndex];
            mWorldRadius[mIndex]        = inCopy.mWorldRadius[inCopy.mIndex];
            mUpperDistance[mIndex]      = inCopy.mUpperDistance[inCopy.mIndex];
            mVisibilityFlags[mIndex]    = inCopy.mVisibilityFlags[inCopy.mIndex];
            mQueryFlags[mIndex]         = inCopy.mQueryFlags[inCopy.mIndex];
            mLightMask[mIndex]          = inCopy.mLightMask[inCopy.mIndex];
        }

        /** Advances all pointers to the next pack, i.e. if we're processing 4
            elements at a time, move to the next 4 elements.
        */
        void advancePack()
        {
            mParents            += ARRAY_PACKED_REALS;
            mOwner              += ARRAY_PACKED_REALS;
            ++mLocalAabb;
            ++mWorldAabb;
            mLocalRadius        += ARRAY_PACKED_REALS;
            mWorldRadius        += ARRAY_PACKED_REALS;
            mUpperDistance      += ARRAY_PACKED_REALS;
            mVisibilityFlags    += ARRAY_PACKED_REALS;
            mQueryFlags         += ARRAY_PACKED_REALS;
            mLightMask          += ARRAY_PACKED_REALS;
        }

        void advancePack( size_t numAdvance )
        {
            mParents            += ARRAY_PACKED_REALS * numAdvance;
            mOwner              += ARRAY_PACKED_REALS * numAdvance;
            mLocalAabb          += numAdvance;
            mWorldAabb          += numAdvance;
            mLocalRadius        += ARRAY_PACKED_REALS * numAdvance;
            mWorldRadius        += ARRAY_PACKED_REALS * numAdvance;
            mUpperDistance      += ARRAY_PACKED_REALS * numAdvance;
            mVisibilityFlags    += ARRAY_PACKED_REALS * numAdvance;
            mQueryFlags         += ARRAY_PACKED_REALS * numAdvance;
            mLightMask          += ARRAY_PACKED_REALS * numAdvance;
        }

        /** Advances all pointers needed by DiTransformUnit::updateAllBounds to the next pack,
            i.e. if we're processing 4 elements at a time, move to the next 4 elements.
        */
        void advanceBoundsPack()
        {
#ifndef NDEBUG
            mOwner              += ARRAY_PACKED_REALS;
#endif
            mParents            += ARRAY_PACKED_REALS;
            ++mLocalAabb;
            ++mWorldAabb;
            mLocalRadius        += ARRAY_PACKED_REALS;
            mWorldRadius        += ARRAY_PACKED_REALS;
        }

        /** Advances all pointers needed by DiTransformUnit::cullFrustum to the next pack,
            i.e. if we're processing 4 elements at a time, move to the next 4 elements.
        */
        void advanceFrustumPack()
        {
            mOwner              += ARRAY_PACKED_REALS;
            ++mWorldAabb;
            mVisibilityFlags    += ARRAY_PACKED_REALS;
        }

        /** Advances all pointers needed by InstanceBatch::_updateBounds to the next pack,
            i.e. if we're processing 4 elements at a time, move to the next 4 elements.
        */
        void advanceDirtyInstanceMgr()
        {
            ++mWorldAabb;
            mWorldRadius        += ARRAY_PACKED_REALS;
            mVisibilityFlags    += ARRAY_PACKED_REALS;
        }

        /** Advances all pointers needed by DiTransformUnit::cullLights to the next pack,
            i.e. if we're processing 4 elements at a time, move to the next 4 elements.
        */
        void advanceCullLightPack()
        {
            mOwner              += ARRAY_PACKED_REALS;
            ++mWorldAabb;
            mWorldRadius        += ARRAY_PACKED_REALS;
            mLightMask          += ARRAY_PACKED_REALS;
        }

        /** Advances all pointers needed by DiTransformUnit::buildLightList to the next pack,
            i.e. if we're processing 4 elements at a time, move to the next 4 elements.
        */
        void advanceLightPack()
        {
            mOwner              += ARRAY_PACKED_REALS;
            ++mWorldAabb;
            mWorldRadius        += ARRAY_PACKED_REALS;
            mLightMask          += ARRAY_PACKED_REALS;
        }

        void advanceLodPack()
        {
            mOwner              += ARRAY_PACKED_REALS;
            ++mWorldAabb;
            mWorldRadius        += ARRAY_PACKED_REALS;
        }
    };
}

#endif
