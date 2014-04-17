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

#include "ArrayMemoryManager.h"
#include "ObjectData.h"

#include "TransformUnit.h"

namespace Demi
{
    const size_t ObjectDataArrayMemoryManager::ElementsMemSize
                            [ObjectDataArrayMemoryManager::NumMemoryTypes] =
    {
        sizeof( DiNode** ),               //ArrayMemoryManager::Parent
        sizeof( DiTransformUnit** ),      //ArrayMemoryManager::Owner
        6 * sizeof( float ),       //ArrayMemoryManager::LocalAabb
        6 * sizeof( float ),       //ArrayMemoryManager::WorldAabb
        1 * sizeof( float ),       //ArrayMemoryManager::LocalRadius
        1 * sizeof( float ),       //ArrayMemoryManager::WorldRadius
        1 * sizeof( float ),       //ArrayMemoryManager::SquaredUpperDistance
        1 * sizeof(uint32),     //ArrayMemoryManager::VisibilityFlags
        1 * sizeof(uint32),     //ArrayMemoryManager::QueryFlags
        1 * sizeof(uint32),     //ArrayMemoryManager::LightMask
    };
    const CleanupRoutines ObjectDataArrayMemoryManager::ObjCleanupRoutines[NumMemoryTypes] =
    {
        cleanerFlat,                    //ArrayMemoryManager::Parent
        cleanerFlat,                    //ArrayMemoryManager::Owner
        cleanerArrayAabb,               //ArrayMemoryManager::LocalAabb
        cleanerArrayAabb,               //ArrayMemoryManager::WorldAabb
        cleanerFlat,                    //ArrayMemoryManager::LocalRadius
        cleanerFlat,                    //ArrayMemoryManager::WorldRadius
        cleanerFlat,                    //ArrayMemoryManager::SquaredUpperDistance
        cleanerFlat,                    //ArrayMemoryManager::VisibilityFlags
        cleanerFlat,                    //ArrayMemoryManager::QueryFlags
        cleanerFlat,                    //ArrayMemoryManager::LightMask
    };
    
    ObjectDataArrayMemoryManager::ObjectDataArrayMemoryManager( uint16 depthLevel, size_t hintMaxNodes,
                                                    DiNode *dummyNode, DiTransUnitPtr dummyObject,
                                                    size_t cleanupThreshold, size_t maxHardLimit,
                                                    RebaseListener *rebaseListener ) :
            ArrayMemoryManager( ArrayMemoryManager::ObjectDataType, ElementsMemSize, ObjCleanupRoutines,
                                sizeof( ElementsMemSize ) / sizeof( size_t ), depthLevel,
                                hintMaxNodes, cleanupThreshold, maxHardLimit, rebaseListener ),
            mDummyNode( dummyNode ),
            mDummyObject( dummyObject )
    {
    }
    
    void ObjectDataArrayMemoryManager::slotsRecreated( size_t prevNumSlots )
    {
        ArrayMemoryManager::slotsRecreated( prevNumSlots );

        DiNode **nodesPtr = reinterpret_cast<DiNode**>( mMemoryPools[Parent] ) + prevNumSlots;
        DiTransUnitPtr *ownersPtr = reinterpret_cast<DiTransUnitPtr*>(mMemoryPools[Owner]) + prevNumSlots;
        for( size_t i=prevNumSlots; i<mMaxMemory; ++i )
        {
            *nodesPtr++ = mDummyNode;
            *ownersPtr++ = mDummyObject;
        }
    }
    
    void ObjectDataArrayMemoryManager::createNewNode( ObjectData &outData )
    {
        const size_t nextSlot = createNewSlot();
        const unsigned char nextSlotIdx = nextSlot % ARRAY_PACKED_REALS;
        const size_t nextSlotBase       = nextSlot - nextSlotIdx;

        //Set memory ptrs
        outData.mIndex = nextSlotIdx;
        outData.mParents            = reinterpret_cast<DiNode**>( mMemoryPools[Parent] +
                                                nextSlotBase * mElementsMemSizes[Parent] );
        outData.mOwner              = reinterpret_cast<DiTransUnitPtr*>( mMemoryPools[Owner] +
                                                nextSlotBase * mElementsMemSizes[Owner] );
        outData.mLocalAabb          = reinterpret_cast<ArrayAabb*>( mMemoryPools[LocalAabb] +
                                                nextSlotBase * mElementsMemSizes[LocalAabb] );
        outData.mWorldAabb          = reinterpret_cast<ArrayAabb*>( mMemoryPools[WorldAabb] +
                                                nextSlotBase * mElementsMemSizes[WorldAabb] );
        outData.mLocalRadius        = reinterpret_cast<float*>( mMemoryPools[LocalRadius] +
                                                nextSlotBase * mElementsMemSizes[LocalRadius] );
        outData.mWorldRadius        = reinterpret_cast<float*>( mMemoryPools[WorldRadius] +
                                                nextSlotBase * mElementsMemSizes[WorldRadius] );
        outData.mUpperDistance      = reinterpret_cast<float*>( mMemoryPools[UpperDistance] +
                                                nextSlotBase * mElementsMemSizes[UpperDistance] );
        outData.mVisibilityFlags    = reinterpret_cast<uint32*>( mMemoryPools[VisibilityFlags] +
                                                nextSlotBase * mElementsMemSizes[VisibilityFlags] );
        outData.mQueryFlags         = reinterpret_cast<uint32*>( mMemoryPools[QueryFlags] +
                                                nextSlotBase * mElementsMemSizes[QueryFlags] );
        outData.mLightMask          = reinterpret_cast<uint32*>( mMemoryPools[LightMask] +
                                                nextSlotBase * mElementsMemSizes[LightMask] );

        //Set default values
        outData.mParents[nextSlotIdx]   = mDummyNode;
        outData.mOwner[nextSlotIdx]     = 0; //Caller has to fill it. Otherwise a crash is a good warning
        outData.mLocalAabb->setFromAabb( Aabb::BOX_INFINITE, nextSlotIdx );
        outData.mWorldAabb->setFromAabb( Aabb::BOX_INFINITE, nextSlotIdx );
        outData.mWorldRadius[nextSlotIdx]           = 0;
        outData.mUpperDistance[nextSlotIdx]         = std::numeric_limits<float>::max();
        outData.mVisibilityFlags[nextSlotIdx]       = DiTransformUnit::GetDefaultVisibilityFlags();
        outData.mQueryFlags[nextSlotIdx]            = DiTransformUnit::GetDefaultQueryFlags();
        outData.mLightMask[nextSlotIdx]             = 0xFFFFFFFF;
    }
    
    void ObjectDataArrayMemoryManager::destroyNode( ObjectData &inOutData )
    {
        //Zero out important data that would lead to bugs (Remember SIMD SoA means even if
        //there's one object in scene, 4 objects are still parsed simultaneously)
        inOutData.mParents[inOutData.mIndex]            = mDummyNode;
        inOutData.mOwner[inOutData.mIndex]              = mDummyObject;
        inOutData.mVisibilityFlags[inOutData.mIndex]    = 0;
        inOutData.mQueryFlags[inOutData.mIndex]         = 0;
        inOutData.mLightMask[inOutData.mIndex]          = 0;
        destroySlot( reinterpret_cast<char*>(inOutData.mParents), inOutData.mIndex );
        //Zero out all pointers
        inOutData = ObjectData();
    }
    
    size_t ObjectDataArrayMemoryManager::getFirstNode( ObjectData &outData )
    {
        // Quick hack to fill all pointer variables (I'm lazy to type!)
        memcpy( &outData.mParents, &mMemoryPools[0], sizeof(void*) * mMemoryPools.size() );
        return mUsedMemory;
    }
}
