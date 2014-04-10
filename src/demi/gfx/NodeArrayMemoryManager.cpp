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

#include "MiscPch.h"

#include "Math/Array/ArrayMemoryManager.h"
#include "Math/Array/Transform.h"

namespace Demi
{
    const size_t NodeArrayMemoryManager::ElementsMemSize[NodeArrayMemoryManager::NumMemoryTypes] =
    {
        sizeof(DiNode**),               //ArrayMemoryManager::Parent
        sizeof(DiNode**),               //ArrayMemoryManager::Owner
        3 * sizeof( float ),       //ArrayMemoryManager::Position
        4 * sizeof( float ),       //ArrayMemoryManager::Orientation
        3 * sizeof( float ),       //ArrayMemoryManager::Scale
        3 * sizeof( float ),       //ArrayMemoryManager::DerivedPosition
        4 * sizeof( float ),       //ArrayMemoryManager::DerivedOrientation
        3 * sizeof( float ),       //ArrayMemoryManager::DerivedScale
        16 * sizeof( float ),      //ArrayMemoryManager::WorldMat
        sizeof( bool ),                 //ArrayMemoryManager::InheritOrientation
        sizeof( bool )                  //ArrayMemoryManager::InheritScale
    };
    const CleanupRoutines NodeArrayMemoryManager::NodeCleanupRoutines[NumMemoryTypes] =
    {
        cleanerFlat,                    //ArrayMemoryManager::Parent
        cleanerFlat,                    //ArrayMemoryManager::Owner
        cleanerArrayVector3,            //ArrayMemoryManager::Position
        cleanerArrayQuaternion,         //ArrayMemoryManager::Orientation
        cleanerArrayVector3,            //ArrayMemoryManager::Scale
        cleanerArrayVector3,            //ArrayMemoryManager::DerivedPosition
        cleanerArrayQuaternion,         //ArrayMemoryManager::DerivedOrientation
        cleanerArrayVector3,            //ArrayMemoryManager::DerivedScale
        cleanerFlat,                    //ArrayMemoryManager::WorldMat
        cleanerFlat,                    //ArrayMemoryManager::InheritOrientation
        cleanerFlat                     //ArrayMemoryManager::InheritScale
    };
    
    NodeArrayMemoryManager::NodeArrayMemoryManager( uint16 depthLevel, size_t hintMaxNodes,
                                                    DiNode *dummyNode, size_t cleanupThreshold,
                                                    size_t maxHardLimit,
                                                    RebaseListener *rebaseListener ) :
            ArrayMemoryManager( ArrayMemoryManager::NodeType, ElementsMemSize, NodeCleanupRoutines,
                                sizeof( ElementsMemSize ) / sizeof( size_t ), depthLevel,
                                hintMaxNodes, cleanupThreshold, maxHardLimit, rebaseListener ),
            mDummyNode( dummyNode )
    {
    }
    
    void NodeArrayMemoryManager::slotsRecreated( size_t prevNumSlots )
    {
        ArrayMemoryManager::slotsRecreated( prevNumSlots );

        DiNode **nodesPtr = reinterpret_cast<DiNode**>(mMemoryPools[Parent]) + prevNumSlots;
        for( size_t i=prevNumSlots; i<mMaxMemory; ++i )
            *nodesPtr++ = mDummyNode;

        nodesPtr = reinterpret_cast<DiNode**>(mMemoryPools[Owner]);
        for( size_t i=0; i<prevNumSlots; ++i )
        {
            if( *nodesPtr )
                (*nodesPtr)->CallMemoryChangeListeners();
            ++nodesPtr;
        }
    }
    
    void NodeArrayMemoryManager::createNewNode( Transform &outTransform )
    {
        const size_t nextSlot = createNewSlot();
        const unsigned char nextSlotIdx = nextSlot % ARRAY_PACKED_REALS;
        const size_t nextSlotBase       = nextSlot - nextSlotIdx;

        //Set memory ptrs
        outTransform.mIndex = nextSlotIdx;
        outTransform.mParents           = reinterpret_cast<DiNode**>(mMemoryPools[Parent] +
                                                nextSlotBase * mElementsMemSizes[Parent] );
        outTransform.mOwner             = reinterpret_cast<DiNode**>( mMemoryPools[Owner] +
                                                nextSlotBase * mElementsMemSizes[Owner] );
        outTransform.mPosition          = reinterpret_cast<ArrayVector3*>( mMemoryPools[Position] +
                                                nextSlotBase * mElementsMemSizes[Position] );
        outTransform.mOrientation       = reinterpret_cast<ArrayQuaternion*>(
                                                mMemoryPools[Orientation] +
                                                nextSlotBase * mElementsMemSizes[Orientation] );
        outTransform.mScale             = reinterpret_cast<ArrayVector3*>( mMemoryPools[Scale] +
                                                nextSlotBase * mElementsMemSizes[Scale] );
        outTransform.mDerivedPosition   = reinterpret_cast<ArrayVector3*>(
                                                mMemoryPools[DerivedPosition] +
                                                nextSlotBase * mElementsMemSizes[DerivedPosition] );
        outTransform.mDerivedOrientation=reinterpret_cast<ArrayQuaternion*>(
                                                mMemoryPools[DerivedOrientation] +
                                                nextSlotBase * mElementsMemSizes[DerivedOrientation] );
        outTransform.mDerivedScale      = reinterpret_cast<ArrayVector3*>( mMemoryPools[DerivedScale] +
                                                nextSlotBase * mElementsMemSizes[DerivedScale] );
        outTransform.mDerivedTransform  = reinterpret_cast<DiMat4*>( mMemoryPools[WorldMat] +
                                                nextSlotBase * mElementsMemSizes[WorldMat] );
        outTransform.mInheritOrientation= reinterpret_cast<bool*>( mMemoryPools[InheritOrientation] +
                                                nextSlotBase * mElementsMemSizes[InheritOrientation] );
        outTransform.mInheritScale      = reinterpret_cast<bool*>( mMemoryPools[InheritScale] +
                                                nextSlotBase * mElementsMemSizes[InheritScale] );

        //Set default values
        outTransform.mParents[nextSlotIdx] = mDummyNode;
        outTransform.mOwner[nextSlotIdx] = 0;
        outTransform.mPosition->setFromVector3( DiVec3::ZERO, nextSlotIdx );
        outTransform.mOrientation->setFromQuaternion( DiQuat::IDENTITY, nextSlotIdx );
        outTransform.mScale->setFromVector3( DiVec3::UNIT_SCALE, nextSlotIdx );
        outTransform.mDerivedPosition->setFromVector3( DiVec3::ZERO, nextSlotIdx );
        outTransform.mDerivedOrientation->setFromQuaternion( DiQuat::IDENTITY, nextSlotIdx );
        outTransform.mDerivedScale->setFromVector3( DiVec3::UNIT_SCALE, nextSlotIdx );
        outTransform.mDerivedTransform[nextSlotIdx] = DiMat4::IDENTITY;
        outTransform.mInheritOrientation[nextSlotIdx]   = true;
        outTransform.mInheritScale[nextSlotIdx]         = true;
    }
    
    void NodeArrayMemoryManager::destroyNode( Transform &inOutTransform )
    {
        //Zero out important data that would lead to bugs (Remember SIMD SoA means even if
        //there's one object in scene, 4 objects are still parsed simultaneously)

        inOutTransform.mParents[inOutTransform.mIndex]  = mDummyNode;
        inOutTransform.mOwner[inOutTransform.mIndex]    = 0;
        destroySlot( reinterpret_cast<char*>(inOutTransform.mParents), inOutTransform.mIndex );
        //Zero out all pointers
        inOutTransform = Transform();
    }
    
    size_t NodeArrayMemoryManager::getFirstNode( Transform &outTransform )
    {
        outTransform.mParents           = reinterpret_cast<DiNode**>( mMemoryPools[Parent] );
        outTransform.mOwner             = reinterpret_cast<DiNode**>( mMemoryPools[Owner] );
        outTransform.mPosition          = reinterpret_cast<ArrayVector3*>( mMemoryPools[Position] );
        outTransform.mOrientation       = reinterpret_cast<ArrayQuaternion*>(
                                                        mMemoryPools[Orientation] );
        outTransform.mScale             = reinterpret_cast<ArrayVector3*>( mMemoryPools[Scale] );
        outTransform.mDerivedPosition   = reinterpret_cast<ArrayVector3*>(
                                                        mMemoryPools[DerivedPosition] );
        outTransform.mDerivedOrientation= reinterpret_cast<ArrayQuaternion*>(
                                                        mMemoryPools[DerivedOrientation] );
        outTransform.mDerivedScale      = reinterpret_cast<ArrayVector3*>( mMemoryPools[DerivedScale] );
        outTransform.mDerivedTransform  = reinterpret_cast<DiMat4*>( mMemoryPools[WorldMat] );
        outTransform.mInheritOrientation= reinterpret_cast<bool*>( mMemoryPools[InheritOrientation] );
        outTransform.mInheritScale      = reinterpret_cast<bool*>( mMemoryPools[InheritScale] );

        return mUsedMemory;
    }
}
