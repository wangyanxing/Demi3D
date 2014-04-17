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

#include "GfxPch.h"

#include "SkeletonAnimation.h"
#include "SkeletonAnimationDef.h"
#include "SkeletonInstance.h"

namespace Demi
{
    SkeletonAnimation::SkeletonAnimation( const SkeletonAnimationDef *definition,
                                            const FastArray<size_t> *_slotStarts,
                                            SkeletonInstance *owner ) :
        mDefinition( definition ),
        mBoneWeights( 0 ),
        mCurrentFrame( 0 ),
        mFrameRate( definition->mOriginalFrameRate ),
        mWeight( 1.0f ),
        mSlotStarts( _slotStarts ),
        mLoop( true ),
        mEnabled( false ),
        mOwner( owner ),
        mName( definition->mName )
    {
        mLastKnownKeyFrames.reserve( definition->mTracks.size() );
#ifndef NDEBUG
        for( size_t i=0; i<mSlotStarts->size(); ++i )
            assert( (*mSlotStarts)[i] < ARRAY_PACKED_REALS );
#endif
    }
    //-----------------------------------------------------------------------------------
    void SkeletonAnimation::initialize(void)
    {
        const FastArray<size_t> &slotStarts = *mSlotStarts;

        mBoneWeights = RawSimdUniquePtr<ArrayFloat, MEMCATEGORY_ANIMATION>( mDefinition->mTracks.size() );
        ArrayFloat *boneWeights = mBoneWeights.get();
        float *boneWeightsScalar = reinterpret_cast<float*>( mBoneWeights.get() );

        SkeletonTrackVec::const_iterator itor = mDefinition->mTracks.begin();
        SkeletonTrackVec::const_iterator end  = mDefinition->mTracks.end();

        while( itor != end )
        {
            if( itor->getUsedSlots() <= (ARRAY_PACKED_REALS >> 1) )
            {
                size_t level = itor->getBoneBlockIdx() >> 24;
                size_t slotStart = slotStarts[level];

                for( size_t i=0; i<slotStart; ++i )
                    boneWeightsScalar[i] = 0.0f;
                for( size_t i=slotStart; i<slotStart + itor->getUsedSlots(); ++i )
                    boneWeightsScalar[i] = 1.0f;
                for( size_t i=slotStart + itor->getUsedSlots(); i<ARRAY_PACKED_REALS; ++i )
                    boneWeightsScalar[i] = 0.0f;
            }
            else
            {
                *boneWeights = Mathlib::ONE;
            }

            mLastKnownKeyFrames.push_back( itor->getKeyFrames().begin() );
            boneWeightsScalar += ARRAY_PACKED_REALS;
            ++boneWeights;
            ++itor;
        }
    }
    
    void SkeletonAnimation::addFrame( float frames )
    {
        mCurrentFrame += frames;
        float maxFrame = mDefinition->mNumFrames;

        if( !mLoop )
        {
            mCurrentFrame = DiMath::Max(mCurrentFrame, 0.0f);
            mCurrentFrame = DiMath::Min(mCurrentFrame, maxFrame);
        }
        else
        {
            mCurrentFrame = fmod( mCurrentFrame, maxFrame );
            if( mCurrentFrame < 0 )
                mCurrentFrame = maxFrame - mCurrentFrame;
        }
    }
    
    void SkeletonAnimation::setBoneWeight( IdString boneName, float weight )
    {
        auto itor = mDefinition->mBoneToWeights.find( boneName );
        if( itor != mDefinition->mBoneToWeights.end() )
        {
            size_t level    = itor->second >> 24;
            size_t offset   = itor->second & 0x00FFFFFF;
            float *aliasedBoneWeights = reinterpret_cast<float*>( mBoneWeights.get() ) +
                                                offset + (*mSlotStarts)[level];
            *aliasedBoneWeights = weight;
        }
    }
    
    float SkeletonAnimation::getBoneWeight( IdString boneName ) const
    {
        float retVal = 0.0f;

        auto itor = mDefinition->mBoneToWeights.find( boneName );
        if( itor != mDefinition->mBoneToWeights.end() )
        {
            size_t level    = itor->second >> 24;
            size_t offset   = itor->second & 0x00FFFFFF;
            const float *aliasedBoneWeights = reinterpret_cast<const float*>( mBoneWeights.get() ) +
                                                            offset + (*mSlotStarts)[level];
            retVal = *aliasedBoneWeights;
        }
        return retVal;
    }
    
    float* SkeletonAnimation::getBoneWeightPtr( IdString boneName )
    {
        float* retVal = 0;

        auto itor = mDefinition->mBoneToWeights.find( boneName );
        if( itor != mDefinition->mBoneToWeights.end() )
        {
            size_t level    = itor->second >> 24;
            size_t offset   = itor->second & 0x00FFFFFF;
            retVal = reinterpret_cast<float*>( mBoneWeights.get() ) + offset + (*mSlotStarts)[level];
        }

        return retVal;
    }
    
    void SkeletonAnimation::setEnabled( bool bEnable )
    {
        if( mEnabled != bEnable )
        {
            if( bEnable )
                mOwner->_enableAnimation( this );
            else
                mOwner->_disableAnimation( this );

            mEnabled = bEnable;
        }
    }
    
    void SkeletonAnimation::_applyAnimation( const TransformArray &boneTransforms )
    {
        auto itor = mDefinition->mTracks.begin();
        auto end = mDefinition->mTracks.end();

        auto itLastKnownKeyFrame = mLastKnownKeyFrames.begin();

        ArrayFloat simdWeight = Mathlib::SetAll( mWeight );
        ArrayFloat * RESTRICT_ALIAS boneWeights = mBoneWeights.get();

        while( itor != end )
        {
            itor->applyKeyFrameRigAt( *itLastKnownKeyFrame, mCurrentFrame, simdWeight,
                                        boneWeights, boneTransforms );
            ++itLastKnownKeyFrame;
            ++boneWeights;
            ++itor;
        }
    }
}
