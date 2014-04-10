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
#include "NodeMemoryManager.h"
#include "AlignedAllocator.h"
#include "CullNode.h"

namespace Demi
{
    NodeMemoryManager::NodeMemoryManager() :
            mDummyNode( 0 ),
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

        /*mDummyTransformPtrs.mDerivedTransform = reinterpret_cast<ArrayMatrix4*>( DEMI_MALLOC_SIMD(
                                                sizeof( ArrayMatrix4 ), MEMCATEGORY_SCENE_OBJECTS ) );
        mDummyTransformPtrs.mInheritOrientation= DEMI_MALLOC_SIMD( sizeof( bool ) * ARRAY_PACKED_REALS,
                                                                    MEMCATEGORY_SCENE_OBJECTS );
        mDummyTransformPtrs.mInheritScale       = DEMI_MALLOC_SIMD( sizeof( bool ) * ARRAY_PACKED_REALS,
                                                                    MEMCATEGORY_SCENE_OBJECTS );*/

        *mDummyTransformPtrs.mDerivedPosition       = ArrayVector3::ZERO;
        *mDummyTransformPtrs.mDerivedOrientation    = ArrayQuaternion::IDENTITY;
        *mDummyTransformPtrs.mDerivedScale          = ArrayVector3::UNIT_SCALE;

        mDummyNode = DI_NEW DiCullNode( mDummyTransformPtrs );
    }
    
    NodeMemoryManager::~NodeMemoryManager()
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

        /*DEMI_FREE_SIMD( mDummyTransformPtrs.mPosition, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mOrientation, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mScale, MEMCATEGORY_SCENE_OBJECTS );*/

        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedPosition );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedOrientation );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedScale );

        /*DEMI_FREE_SIMD( mDummyTransformPtrs.mDerivedTransform, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mInheritOrientation, MEMCATEGORY_SCENE_OBJECTS );
        DEMI_FREE_SIMD( mDummyTransformPtrs.mInheritScale, MEMCATEGORY_SCENE_OBJECTS );*/
        mDummyTransformPtrs = Transform();
    }
    
    void NodeMemoryManager::_setTwin( SceneMemoryMgrTypes memoryManagerType,
                                        NodeMemoryManager *twinMemoryManager )
    {
        mMemoryManagerType = memoryManagerType;
        mTwinMemoryManager = twinMemoryManager;
    }
    
    void NodeMemoryManager::growToDepth( size_t newDepth )
    {
        //TODO: (dark_sylinc) give a specialized hint for each depth
        while( newDepth >= mMemoryManagers.size() )
        {
            mMemoryManagers.push_back( NodeArrayMemoryManager( mMemoryManagers.size(), 100,
                                                                mDummyNode, 100,
                                                                ArrayMemoryManager::MAX_MEMORY_SLOTS,
                                                                this ) );
            mMemoryManagers.back().initialize();
        }
    }
    
    void NodeMemoryManager::nodeCreated( Transform &outTransform, size_t depth )
    {
        growToDepth( depth );

        NodeArrayMemoryManager& mgr = mMemoryManagers[depth];
        mgr.createNewNode( outTransform );
    }
    
    void NodeMemoryManager::nodeAttached( Transform &outTransform, size_t depth )
    {
        growToDepth( depth );

        Transform tmp;
        mMemoryManagers[depth].createNewNode( tmp );

        tmp.copy( outTransform );

        NodeArrayMemoryManager &mgr = mMemoryManagers[0];
        mgr.destroyNode( outTransform );

        outTransform = tmp;
    }
    
    void NodeMemoryManager::nodeDettached( Transform &outTransform, size_t depth )
    {
        Transform tmp;
        mMemoryManagers[0].createNewNode( tmp );

        tmp.copy( outTransform );
        tmp.mParents[tmp.mIndex] = mDummyNode;

        NodeArrayMemoryManager &mgr = mMemoryManagers[depth];
        mgr.destroyNode( outTransform );

        outTransform = tmp;
    }
    
    void NodeMemoryManager::nodeDestroyed( Transform &outTransform, size_t depth )
    {
        NodeArrayMemoryManager &mgr = mMemoryManagers[depth];
        mgr.destroyNode( outTransform );
    }
    
    void NodeMemoryManager::nodeMoved( Transform &inOutTransform, size_t oldDepth, size_t newDepth )
    {
        growToDepth( newDepth );

        Transform tmp;
        mMemoryManagers[newDepth].createNewNode( tmp );

        tmp.copy( inOutTransform );

        NodeArrayMemoryManager &mgr = mMemoryManagers[oldDepth];
        mgr.destroyNode( inOutTransform );

        inOutTransform = tmp;
    }
    
    void NodeMemoryManager::migrateTo( Transform &inOutTransform, size_t depth,
                                        NodeMemoryManager *dstNodeMemoryManager )
    {
        Transform tmp;
        dstNodeMemoryManager->nodeCreated( tmp, depth );
        tmp.copy( inOutTransform );
        this->nodeDestroyed( inOutTransform, depth );
        inOutTransform = tmp;
    }
    
    size_t NodeMemoryManager::getNumDepths() const
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
    
    size_t NodeMemoryManager::getFirstNode( Transform &outTransform, size_t depth )
    {
        return mMemoryManagers[depth].getFirstNode( outTransform );
    }
    
    void NodeMemoryManager::buildDiffList( ArrayMemoryManager::ManagerType managerType, uint16 level,
                                            const MemoryPoolVec &basePtrs,
                                            ArrayMemoryManager::PtrdiffVec &outDiffsList )
    {
        //We don't need to build the diff list as we've access to the Node through mOwner
        //and access to the actual Node with the right pointers.
        /*Transform transform;
        const size_t numNodes = this->getFirstNode( transform, level );

        for( size_t i=0; i<numNodes; i += ARRAY_PACKED_REALS )
        {
            for( size_t j=0; j<ARRAY_PACKED_REALS; ++j )
            {
                if( transform.mOwner[j] )
                {
                    outDiffsList.push_back( (transform.mParents + transform.mIndex) -
                                        (Ogre::Node**)basePtrs[NodeArrayMemoryManager::Parent] );
                }
            }
            transform.advancePack();
        }*/
    }
    //---------------------------------------------------------------------
    void NodeMemoryManager::applyRebase( ArrayMemoryManager::ManagerType managerType, uint16 level,
                                            const MemoryPoolVec &newBasePtrs,
                                            const ArrayMemoryManager::PtrdiffVec &diffsList )
    {
        Transform transform;
        const size_t numNodes = this->getFirstNode( transform, level );

        for( size_t i=0; i<numNodes; i += ARRAY_PACKED_REALS )
        {
            for( size_t j=0; j<ARRAY_PACKED_REALS; ++j )
            {
                if( transform.mOwner[j] )
                {
                    transform.mIndex = j;
                    transform.mOwner[j]->GetTransform() = transform;
                }
            }

            transform.advancePack();
        }
    }
    //---------------------------------------------------------------------
    void NodeMemoryManager::performCleanup( ArrayMemoryManager::ManagerType managerType, uint16 level,
                                        const MemoryPoolVec &basePtrs, size_t const *elementsMemSizes,
                                        size_t startInstance, size_t diffInstances )
    {
        Transform transform;
        const size_t numNodes = this->getFirstNode( transform, level );

        size_t roundedStart = startInstance / ARRAY_PACKED_REALS;

        transform.advancePack( roundedStart );

        for( size_t i=roundedStart * ARRAY_PACKED_REALS; i<numNodes; i += ARRAY_PACKED_REALS )
        {
            for( size_t j=0; j<ARRAY_PACKED_REALS; ++j )
            {
                if( transform.mOwner[j] )
                {
                    transform.mIndex = j;
                    transform.mOwner[j]->GetTransform() = transform;
                }
            }

            transform.advancePack();
        }
    }
}
