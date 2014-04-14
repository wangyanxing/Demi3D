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

#include "GfxPch.h"

#include "BoneArrayMemoryManager.h"
#include "Math/Array/BoneTransform.h"

namespace Demi
{
    const size_t BoneArrayMemoryManager::ElementsMemSize[BoneArrayMemoryManager::NumMemoryTypes] =
    {
        sizeof( DiBone** ),             //ArrayMemoryManager::Owner
        3 * sizeof( float ),            //ArrayMemoryManager::Position
        4 * sizeof( float ),            //ArrayMemoryManager::Orientation
        3 * sizeof( float ),            //ArrayMemoryManager::Scale
        sizeof( SimpleMatrixAf4x3** ),  //ArrayMemoryManager::ParentNode
        sizeof( SimpleMatrixAf4x3** ),  //ArrayMemoryManager::ParentMat
        12 * sizeof( float ),           //ArrayMemoryManager::WorldMat
        12 * sizeof( float ),           //ArrayMemoryManager::FinalMat
        sizeof( bool ),                 //ArrayMemoryManager::InheritOrientation
        sizeof( bool )                  //ArrayMemoryManager::InheritScale
    };
    const CleanupRoutines BoneArrayMemoryManager::BoneCleanupRoutines[NumMemoryTypes] =
    {
        cleanerFlat,                    //ArrayMemoryManager::Owner
        cleanerArrayVector3,            //ArrayMemoryManager::Position
        cleanerArrayQuaternion,         //ArrayMemoryManager::Orientation
        cleanerArrayVector3,            //ArrayMemoryManager::Scale
        cleanerFlat,                    //ArrayMemoryManager::ParentNode
        cleanerFlat,                    //ArrayMemoryManager::ParentMat
        cleanerFlat,                    //ArrayMemoryManager::WorldMat
        cleanerFlat,                    //ArrayMemoryManager::FinalMat
        cleanerFlat,                    //ArrayMemoryManager::InheritOrientation
        cleanerFlat                     //ArrayMemoryManager::InheritScale
    };
    
    BoneArrayMemoryManager::BoneArrayMemoryManager( uint16 depthLevel, size_t hintMaxNodes,
                                                    size_t cleanupThreshold, size_t maxHardLimit,
                                                    RebaseListener *rebaseListener ) :
            ArrayMemoryManager( ArrayMemoryManager::BoneType, ElementsMemSize, BoneCleanupRoutines,
                                sizeof( ElementsMemSize ) / sizeof( size_t ), depthLevel,
                                hintMaxNodes, cleanupThreshold, maxHardLimit, rebaseListener )
    {
    }
    
    void BoneArrayMemoryManager::slotsRecreated( size_t prevNumSlots )
    {
        ArrayMemoryManager::slotsRecreated( prevNumSlots );

        bool *inheritOrientation = reinterpret_cast<bool*>(
                                        mMemoryPools[InheritOrientation] ) + prevNumSlots;
        bool *inheritScale = reinterpret_cast<bool*>( mMemoryPools[InheritScale] ) + prevNumSlots;
        SimpleMatrixAf4x3 const **parentMatPtr = reinterpret_cast<const SimpleMatrixAf4x3**>(
                                                    mMemoryPools[ParentMat] ) + prevNumSlots;
        SimpleMatrixAf4x3 const **parentNodePtr= reinterpret_cast<const SimpleMatrixAf4x3**>(
                                                    mMemoryPools[ParentNode] ) + prevNumSlots;
        for( size_t i=prevNumSlots; i<mMaxMemory; ++i )
        {
            *inheritOrientation++   = true;
            *inheritScale++         = true;
            *parentNodePtr++= &SimpleMatrixAf4x3::IDENTITY;
            *parentMatPtr++ = &SimpleMatrixAf4x3::IDENTITY;
        }
    }
    
    void BoneArrayMemoryManager::createNewNode( BoneTransform &outTransform )
    {
        const size_t nextSlot = createNewSlot();
        const unsigned char nextSlotIdx = nextSlot % ARRAY_PACKED_REALS;
        const size_t nextSlotBase       = nextSlot - nextSlotIdx;

        //Set memory ptrs
        outTransform.mIndex = nextSlotIdx;
        outTransform.mOwner             = reinterpret_cast<DiBone**>( mMemoryPools[Owner] +
                                                nextSlotBase * mElementsMemSizes[Owner] );
        outTransform.mPosition          = reinterpret_cast<ArrayVector3*>( mMemoryPools[Position] +
                                                nextSlotBase * mElementsMemSizes[Position] );
        outTransform.mOrientation       = reinterpret_cast<ArrayQuaternion*>(
                                                mMemoryPools[Orientation] +
                                                nextSlotBase * mElementsMemSizes[Orientation] );
        outTransform.mScale             = reinterpret_cast<ArrayVector3*>( mMemoryPools[Scale] +
                                                nextSlotBase * mElementsMemSizes[Scale] );
        outTransform.mParentNodeTransform=reinterpret_cast<const SimpleMatrixAf4x3**>(
                                                mMemoryPools[ParentNode] +
                                                nextSlotBase * mElementsMemSizes[ParentNode] );
        outTransform.mParentTransform   = reinterpret_cast<const SimpleMatrixAf4x3**>(
                                                mMemoryPools[ParentMat] +
                                                nextSlotBase * mElementsMemSizes[ParentMat] );
        outTransform.mDerivedTransform  = reinterpret_cast<SimpleMatrixAf4x3*>( mMemoryPools[WorldMat] +
                                                nextSlotBase * mElementsMemSizes[WorldMat] );
        outTransform.mFinalTransform    = reinterpret_cast<SimpleMatrixAf4x3*>( mMemoryPools[FinalMat] +
                                                nextSlotBase * mElementsMemSizes[FinalMat] );
        outTransform.mInheritOrientation= reinterpret_cast<bool*>( mMemoryPools[InheritOrientation] +
                                                nextSlotBase * mElementsMemSizes[InheritOrientation] );
        outTransform.mInheritScale      = reinterpret_cast<bool*>( mMemoryPools[InheritScale] +
                                                nextSlotBase * mElementsMemSizes[InheritScale] );

        //Set default values
        outTransform.mOwner[nextSlotIdx]    = 0;
        outTransform.mPosition->setFromVector3( DiVec3::ZERO, nextSlotIdx );
        outTransform.mOrientation->setFromQuaternion( DiQuat::IDENTITY, nextSlotIdx );
        outTransform.mScale->setFromVector3( DiVec3::UNIT_SCALE, nextSlotIdx );
        outTransform.mParentNodeTransform[nextSlotIdx]  = &SimpleMatrixAf4x3::IDENTITY;
        outTransform.mParentTransform[nextSlotIdx]      = &SimpleMatrixAf4x3::IDENTITY;
        outTransform.mDerivedTransform[nextSlotIdx]     = SimpleMatrixAf4x3::IDENTITY;
        outTransform.mFinalTransform[nextSlotIdx]       = SimpleMatrixAf4x3::IDENTITY;
        outTransform.mInheritOrientation[nextSlotIdx]   = true;
        outTransform.mInheritScale[nextSlotIdx]         = true;
    }
    
    void BoneArrayMemoryManager::destroyNode( BoneTransform &inOutTransform )
    {
        //Zero out important data that would lead to bugs (Remember SIMD SoA means even if
        //there's one object in scene, 4 objects are still parsed simultaneously)

        inOutTransform.mOwner[inOutTransform.mIndex]            = 0;
        inOutTransform.mParentNodeTransform[inOutTransform.mIndex]=&SimpleMatrixAf4x3::IDENTITY;
        inOutTransform.mParentTransform[inOutTransform.mIndex]  = &SimpleMatrixAf4x3::IDENTITY;
        inOutTransform.mInheritOrientation[inOutTransform.mIndex]= true;
        inOutTransform.mInheritScale[inOutTransform.mIndex]     = true;
        destroySlot( reinterpret_cast<char*>(inOutTransform.mOwner), inOutTransform.mIndex );
        //Zero out all pointers
        inOutTransform = BoneTransform();
    }
    
    size_t BoneArrayMemoryManager::getFirstNode( BoneTransform &outTransform )
    {
        outTransform.mOwner             = reinterpret_cast<DiBone**>( mMemoryPools[Owner] );
        outTransform.mPosition          = reinterpret_cast<ArrayVector3*>( mMemoryPools[Position] );
        outTransform.mOrientation       = reinterpret_cast<ArrayQuaternion*>(mMemoryPools[Orientation] );
        outTransform.mScale             = reinterpret_cast<ArrayVector3*>( mMemoryPools[Scale] );
        outTransform.mParentNodeTransform=reinterpret_cast<const SimpleMatrixAf4x3**>(mMemoryPools[ParentNode] );
        outTransform.mParentTransform   = reinterpret_cast<const SimpleMatrixAf4x3**>(mMemoryPools[ParentMat] );
        outTransform.mDerivedTransform  = reinterpret_cast<SimpleMatrixAf4x3*>( mMemoryPools[WorldMat] );
        outTransform.mFinalTransform    = reinterpret_cast<SimpleMatrixAf4x3*>( mMemoryPools[FinalMat] );
        outTransform.mInheritOrientation= reinterpret_cast<bool*>( mMemoryPools[InheritOrientation] );
        outTransform.mInheritScale      = reinterpret_cast<bool*>( mMemoryPools[InheritScale] );

        return mUsedMemory;
    }
}
