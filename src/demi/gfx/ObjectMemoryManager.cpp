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

#include "ObjectMemoryManager.h"
#include "AlignedAllocator.h"
#include "CullNode.h"

namespace Demi
{
    ObjectMemoryManager::ObjectMemoryManager() :
            mTotalObjects( 0 ),
            mDummyNode( 0 ),
            mDummyObject( 0 ),
            mMemoryManagerType( SCENE_DYNAMIC ),
            mTwinMemoryManager( 0 )
    {
        //Manually allocate the memory for the dummy scene nodes (since we can't pass ourselves
        //or yet another object) We only allocate what's needed to prevent access violations.
        /*mDummyTransformPtrs.mPosition = reinterpret_cast<ArrayVector3*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayVector3 ), MEMCATEGORY_SCENE_OBJECTS ) );
        mDummyTransformPtrs.mOrientation = reinterpret_cast<ArrayQuaternion*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayQuaternion ), MEMCATEGORY_SCENE_OBJECTS ) );
        mDummyTransformPtrs.mScale = reinterpret_cast<ArrayVector3*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayVector3 ), MEMCATEGORY_SCENE_OBJECTS ) );*/

        mDummyTransformPtrs.mDerivedPosition    = reinterpret_cast<ArrayVector3*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayVector3 ) ) );
        mDummyTransformPtrs.mDerivedOrientation= reinterpret_cast<ArrayQuaternion*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayQuaternion ) ) );
        mDummyTransformPtrs.mDerivedScale       = reinterpret_cast<ArrayVector3*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayVector3 ) ) );

        mDummyTransformPtrs.mDerivedTransform   = reinterpret_cast<DiMat4*>( DEMI_MALLOC_SIMD(
                                                sizeof( DiMat4 ) * ARRAY_PACKED_REALS ) );
        /*mDummyTransformPtrs.mInheritOrientation= DEMI_MALLOC_SIMD( sizeof( bool ) * ARRAY_PACKED_REALS,
                                                                    MEMCATEGORY_SCENE_OBJECTS );
        mDummyTransformPtrs.mInheritScale       = DEMI_MALLOC_SIMD( sizeof( bool ) * ARRAY_PACKED_REALS,
                                                                    MEMCATEGORY_SCENE_OBJECTS );*/

        *mDummyTransformPtrs.mDerivedPosition       = ArrayVector3::ZERO;
        *mDummyTransformPtrs.mDerivedOrientation    = ArrayQuaternion::IDENTITY;
        *mDummyTransformPtrs.mDerivedScale          = ArrayVector3::UNIT_SCALE;
        for( size_t i=0; i<ARRAY_PACKED_REALS; ++i )
            mDummyTransformPtrs.mDerivedTransform[i] = DiMat4::IDENTITY;

        mDummyNode = DI_NEW DiCullNode( mDummyTransformPtrs );
        mDummyObject = DI_NEW NullEntity();
    }
    
    ObjectMemoryManager::~ObjectMemoryManager()
    {
        ArrayMemoryManagerVec::iterator itor = mMemoryManagers.begin();
        ArrayMemoryManagerVec::iterator end  = mMemoryManagers.end();

        while( itor != end )
        {
            itor->destroy();
            ++itor;
        }

        mMemoryManagers.clear();

        delete mDummyNode;
        mDummyNode = 0;

        delete mDummyObject;
        mDummyObject = 0;

        /*DEMI_FREE_SIMD( mDummyTransformPtrs.mPosition, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mOrientation, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mScale, MEMCATEGORY_SCENE_OBJECTS );*/

        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedPosition );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedOrientation );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedScale );

        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedTransform );
        /*DEMI_FREE_SIMD( mDummyTransformPtrs.mInheritOrientation, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mInheritScale, MEMCATEGORY_SCENE_OBJECTS );*/
        mDummyTransformPtrs = Transform();
    }
    
    void ObjectMemoryManager::_setTwin( SceneMemoryMgrTypes memoryManagerType,
                                        ObjectMemoryManager *twinMemoryManager )
    {
        mMemoryManagerType = memoryManagerType;
        mTwinMemoryManager = twinMemoryManager;
    }
    
    void ObjectMemoryManager::growToDepth( size_t newDepth )
    {
        //TODO: (dark_sylinc) give a specialized hint for each depth
        while( newDepth >= mMemoryManagers.size() )
        {
            mMemoryManagers.push_back( ObjectDataArrayMemoryManager( mMemoryManagers.size(), 100,
                                            mDummyNode, mDummyObject, 100,
                                            ArrayMemoryManager::MAX_MEMORY_SLOTS, this ) );
            mMemoryManagers.back().initialize();
        }
    }
    
    void ObjectMemoryManager::objectCreated( ObjectData &outObjectData, size_t renderQueue )
    {
        growToDepth( renderQueue );

        ObjectDataArrayMemoryManager& mgr = mMemoryManagers[renderQueue];
        mgr.createNewNode( outObjectData );

        ++mTotalObjects;
    }
    
    void ObjectMemoryManager::objectMoved( ObjectData &inOutObjectData, size_t oldRenderQueue,
                                            size_t newRenderQueue )
    {
        growToDepth( newRenderQueue );

        ObjectData tmp;
        mMemoryManagers[newRenderQueue].createNewNode( tmp );

        tmp.copy( inOutObjectData );

        ObjectDataArrayMemoryManager &mgr = mMemoryManagers[oldRenderQueue];
        mgr.destroyNode( inOutObjectData );

        inOutObjectData = tmp;
    }
    
    void ObjectMemoryManager::objectDestroyed( ObjectData &outObjectData, size_t renderQueue )
    {
        ObjectDataArrayMemoryManager &mgr = mMemoryManagers[renderQueue];
        mgr.destroyNode( outObjectData );

        --mTotalObjects;
    }
    
    void ObjectMemoryManager::migrateTo( ObjectData &inOutObjectData, size_t renderQueue,
                                         ObjectMemoryManager *dstObjectMemoryManager )
    {
        ObjectData tmp;
        dstObjectMemoryManager->objectCreated( tmp, renderQueue );
        tmp.copy( inOutObjectData );
        this->objectDestroyed( inOutObjectData, renderQueue );
        inOutObjectData = tmp;
    }
    
    size_t ObjectMemoryManager::getNumRenderQueues() const
    {
        size_t retVal = -1;
        auto begin = mMemoryManagers.begin();
        auto itor = mMemoryManagers.begin();
        auto end = mMemoryManagers.end();

        while( itor != end )
        {
            if( itor->getUsedMemory() )
                retVal = itor - begin;
            ++itor;
        }

        return retVal + 1;
    }
    
    size_t ObjectMemoryManager::getFirstObjectData( ObjectData &outObjectData, size_t renderQueue )
    {
        return mMemoryManagers[renderQueue].getFirstNode( outObjectData );
    }
    
    void ObjectMemoryManager::buildDiffList( ArrayMemoryManager::ManagerType managerType, uint16 level,
                                                const MemoryPoolVec &basePtrs,
                                                ArrayMemoryManager::PtrdiffVec &outDiffsList )
    {
        //We don't need to build the diff list as we've access to the DiTransformUnit through mOwner
        //and access to the actual ObjectData with the right pointers.
        /*ObjectData objectData;
        const size_t numObjs = this->getFirstObjectData( objectData, level );

        for( size_t i=0; i<numObjs; i += ARRAY_PACKED_REALS )
        {
            for( size_t j=0; j<ARRAY_PACKED_REALS; ++j )
            {
                if( objectData.mOwner[j] )
                {
                    outDiffsList.push_back( (objectData.mParents + objectData.mIndex) -
                                        (Ogre::Node**)basePtrs[ObjectDataArrayMemoryManager::Parent] );
                }
            }
            objectData.advancePack();
        }*/
    }
    //---------------------------------------------------------------------
    void ObjectMemoryManager::applyRebase( ArrayMemoryManager::ManagerType managerType, uint16 level,
                                            const MemoryPoolVec &newBasePtrs,
                                            const ArrayMemoryManager::PtrdiffVec &diffsList )
    {
        ObjectData objectData;
        const size_t numObjs = this->getFirstObjectData( objectData, level );

        for( size_t i=0; i<numObjs; i += ARRAY_PACKED_REALS )
        {
            for( size_t j=0; j<ARRAY_PACKED_REALS; ++j )
            {
                if( objectData.mOwner[j] )
                {
                    objectData.mIndex = j;
                    objectData.mOwner[j]->GetObjectData() = objectData;
                }
            }

            objectData.advancePack();
        }
    }
    //---------------------------------------------------------------------
    void ObjectMemoryManager::performCleanup( ArrayMemoryManager::ManagerType managerType, uint16 level,
                                        const MemoryPoolVec &basePtrs, size_t const *elementsMemSizes,
                                        size_t startInstance, size_t diffInstances )
    {
        ObjectData objectData;
        const size_t numObjs = this->getFirstObjectData( objectData, level );

        size_t roundedStart = startInstance / ARRAY_PACKED_REALS;

        objectData.advancePack( roundedStart );

        for( size_t i=roundedStart * ARRAY_PACKED_REALS; i<numObjs; i += ARRAY_PACKED_REALS )
        {
            for( size_t j=0; j<ARRAY_PACKED_REALS; ++j )
            {
                if( objectData.mOwner[j] )
                {
                    objectData.mIndex = j;
                    objectData.mOwner[j]->GetObjectData() = objectData;
                }
            }

            objectData.advancePack();
        }
    }
}
