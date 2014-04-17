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
#ifndef _BoneTransform_H_
#define _BoneTransform_H_

#include "ArrayMatrixAf4x3.h"
#include "ArrayMemoryManager.h"

namespace Demi
{
    class DiNewBone;

    /** Represents the transform of a single object, arranged in SoA (Structure of Arrays) */
    struct BoneTransform
    {
        /// Which of the packed values is ours. Value in range [0; 4) for SSE2
        unsigned char   mIndex;

        /// The Bone that owns this BoneTransform. Ours is mOwner[mIndex]
        DiNewBone       **mOwner;

        /// Stores the position/translation of a node relative to its parent.
        ArrayVector3    * RESTRICT_ALIAS    mPosition;

        /// Stores the orientation of a node relative to it's parent.
        ArrayQuaternion * RESTRICT_ALIAS    mOrientation;

        /// Stores the scaling factor applied to a node
        ArrayVector3    * RESTRICT_ALIAS    mScale;

        /// Points to the Node parent's pointer.
        SimpleMatrixAf4x3 const * * RESTRICT_ALIAS mParentNodeTransform;

        /// Points to the parent's pointer.
        SimpleMatrixAf4x3 const * * RESTRICT_ALIAS mParentTransform;

        /// Caches the full transform into an affine 4x4 matrix. Note it's not Array form! (It's AoS)
        SimpleMatrixAf4x3 * RESTRICT_ALIAS  mDerivedTransform;

        /// Caches mDerivedTransform in Bone space, used for rendering. It's not Array form! (It's AoS)
        SimpleMatrixAf4x3 * RESTRICT_ALIAS  mFinalTransform;

        /// Stores whether this node inherits orientation from it's parent.
        /// Ours is mInheritOrientation[mIndex]
        bool    * RESTRICT_ALIAS mInheritOrientation;

        /// Stores whether this node inherits scale from it's parent.
        /// Ours is mInheritScale[mIndex]
        bool    * RESTRICT_ALIAS mInheritScale;

        BoneTransform() :
            mIndex( 0 ),
            mOwner( 0 ),
            mPosition( 0 ),
            mOrientation( 0 ),
            mScale( 0 ),
            mParentNodeTransform( 0 ),
            mParentTransform( 0 ),
            mDerivedTransform( 0 ),
            mFinalTransform( 0 ),
            mInheritOrientation( 0 ),
            mInheritScale( 0 )
        {
        }

        /** Copies all the scalar data from the parameter into this
        @remarks
            A normal "=" operator, or an assignment constructor BoneTransform( BoneTransform & )
            wouldn't work. This is because ArrayVector3 & co. would try to copy all the
            packed values, while we just want the scalar ones.

            Furthermore, it would be confusing, because it would be not clear whether
            those two options should memcpy memory, or rebase the pointers, hence
            explicit functions are much preferred. @See rebasePtrs

            Note that we do NOT copy the mIndex member.
        */
        void copy( const BoneTransform &inCopy )
        {
            mOwner[mIndex]      = inCopy.mOwner[inCopy.mIndex];

            DiVec3 tmp;
            DiQuat qTmp;

            //Position
            inCopy.mPosition->getAsVector3( tmp, inCopy.mIndex );
            mPosition->setFromVector3( tmp, mIndex );

            //Orientation
            inCopy.mOrientation->getAsQuaternion( qTmp, inCopy.mIndex );
            mOrientation->setFromQuaternion( qTmp, mIndex );

            //Scale
            inCopy.mScale->getAsVector3( tmp, inCopy.mIndex );
            mScale->setFromVector3( tmp, mIndex );

            mParentNodeTransform[mIndex]= inCopy.mParentNodeTransform[inCopy.mIndex];
            mParentTransform[mIndex]    = inCopy.mParentTransform[inCopy.mIndex];
            mDerivedTransform[mIndex]   = inCopy.mDerivedTransform[inCopy.mIndex];
            mFinalTransform[mIndex]     = inCopy.mFinalTransform[inCopy.mIndex];

            mInheritOrientation[mIndex] = inCopy.mInheritOrientation[inCopy.mIndex];
            mInheritScale[mIndex]       = inCopy.mInheritScale[inCopy.mIndex];
        }

        /** Advances all pointers to the next pack, i.e. if we're processing 4 elements at a time, move to
            the next 4 elements.
        */
        void advancePack()
        {
            mOwner              += ARRAY_PACKED_REALS;
            ++mPosition;
            ++mOrientation;
            ++mScale;
            mParentNodeTransform+= ARRAY_PACKED_REALS;
            mParentTransform    += ARRAY_PACKED_REALS;
            mDerivedTransform   += ARRAY_PACKED_REALS;
            mFinalTransform     += ARRAY_PACKED_REALS;
            mInheritOrientation += ARRAY_PACKED_REALS;
            mInheritScale       += ARRAY_PACKED_REALS;
        }

        void advancePack( size_t numAdvance )
        {
            mOwner              += ARRAY_PACKED_REALS * numAdvance;
            mPosition           += numAdvance;
            mOrientation        += numAdvance;
            mScale              += numAdvance;
            mParentNodeTransform+= ARRAY_PACKED_REALS * numAdvance;
            mParentTransform    += ARRAY_PACKED_REALS * numAdvance;
            mDerivedTransform   += ARRAY_PACKED_REALS * numAdvance;
            mFinalTransform     += ARRAY_PACKED_REALS * numAdvance;
            mInheritOrientation += ARRAY_PACKED_REALS * numAdvance;
            mInheritScale       += ARRAY_PACKED_REALS * numAdvance;
        }
    };
}

#endif
