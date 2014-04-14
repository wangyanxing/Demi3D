/*
-----------------------------------------------------------------------------
This source file is part of OGRE
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

#include "SkeletonAnimManager.h"
#include "SkeletonDef.h"
#include "SkeletonInstance.h"

namespace Demi
{
    BySkeletonDef::BySkeletonDef( const SkeletonDef *_skeletonDef, size_t threadCount ) :
        skeletonDef( _skeletonDef ),
        skeletonDefName( _skeletonDef->getName() )
    {
        threadStarts.resize( threadCount + 1, 0 );
    }
    //-----------------------------------------------------------------------
    void BySkeletonDef::initializeMemoryManager(void)
    {
        DiVector<size_t> bonesPerDepth;
        skeletonDef->getBonesPerDepth( bonesPerDepth );
        boneMemoryManager._growToDepth( bonesPerDepth );
        boneMemoryManager.setBoneRebaseListener( this );
    }
    //-----------------------------------------------------------------------
    void BySkeletonDef::updateThreadStarts(void)
    {
        size_t lastStart = 0;
        size_t increments = std::max<size_t>( ARRAY_PACKED_REALS,
                                              skeletons.size() / (threadStarts.size() - 1) );
        for( size_t i=0; i<threadStarts.size(); ++i )
        {
            threadStarts[i] = lastStart;
            lastStart += increments;
            lastStart = std::min( lastStart, skeletons.size() );

            while( lastStart < skeletons.size() &&
                   skeletons[lastStart]->_getMemoryBlock() ==
                   skeletons[lastStart-1]->_getMemoryBlock() )
            {
                ++lastStart;
            }
        }

        assert( threadStarts.back() <= skeletons.size() );
        threadStarts.back() = skeletons.size();
    }
    //-----------------------------------------------------------------------
    void BySkeletonDef::_updateBoneStartTransforms(void)
    {
        FastArray<SkeletonInstance*>::iterator itor = skeletons.begin();
        FastArray<SkeletonInstance*>::iterator end  = skeletons.end();

        while( itor != end )
        {
            (*itor)->_updateBoneStartTransforms();
            ++itor;
        }
    }

    //-----------------------------------------------------------------------
    SkeletonInstance* SkeletonAnimManager::createSkeletonInstance( const SkeletonDef *skeletonDef,
                                                                    size_t numWorkerThreads )
    {
        IdString defName( skeletonDef->getName() );
        BySkeletonDefList::iterator itor = std::find( bySkeletonDefs.begin(), bySkeletonDefs.end(),
                                                        defName );
        if( itor == bySkeletonDefs.end() )
        {
            bySkeletonDefs.push_front( BySkeletonDef( skeletonDef, numWorkerThreads ) );
            bySkeletonDefs.front().initializeMemoryManager();
            itor = bySkeletonDefs.begin();
        }

        BySkeletonDef &bySkelDef = *itor;
        FastArray<SkeletonInstance*> &skeletonsArray = bySkelDef.skeletons;
        SkeletonInstance *newInstance = DI_NEW SkeletonInstance( skeletonDef,
                                                                    &bySkelDef.boneMemoryManager );
        FastArray<SkeletonInstance*>::iterator it = std::lower_bound(
                                                            skeletonsArray.begin(), skeletonsArray.end(),
                                                            newInstance,
                                                            OrderSkeletonInstanceByMemory );

        //If this assert triggers, two instances got the same memory slot. Most likely we forgot to
        //remove a previous instance and the slot was reused. Otherwise something nasty happened.
        assert( it == skeletonsArray.end() || (*it)->_getMemoryUniqueOffset() != newInstance );

        skeletonsArray.insert( it, newInstance );

#if DEMI_DEBUG
        {
            //Check all depth levels respect the same ordering
            auto itSkel = skeletonsArray.begin();
            auto enSkel = skeletonsArray.end();
            while( itSkel != enSkel )
            {
                const TransformArray &transf1 = (*itSkel)-> _getTransformArray();
                FastArray<SkeletonInstance*>::const_iterator itSkel2 = itSkel+1;
                while( itSkel2 != enSkel )
                {
                    const TransformArray &transf2 = (*itSkel2)-> _getTransformArray();
                    for( size_t i=0; i<transf1.size(); ++i )
                    {
                        DiNewBone **owner1 = transf1[i].mOwner + transf1[i].mIndex;
                        DiNewBone **owner2 = transf2[i].mOwner + transf2[i].mIndex;
                        assert( owner1 < owner2 );
                    }

                    ++itSkel2;
                }

                ++itSkel;
            }
        }
#endif

        //Update the thread starts, they have changed.
        bySkelDef.updateThreadStarts();

        return newInstance;
    }
    
    void SkeletonAnimManager::destroySkeletonInstance( SkeletonInstance *skeletonInstance )
    {
        IdString defName( skeletonInstance->getDefinition()->getName() );
        BySkeletonDefList::iterator itor = std::find( bySkeletonDefs.begin(), bySkeletonDefs.end(),
                                                        defName );

        if( itor == bySkeletonDefs.end() )
        {
            DI_WARNING("Trying to remove a SkeletonInstance for which we have no definition for!");
        }

        BySkeletonDef &bySkelDef = *itor;
        FastArray<SkeletonInstance*> &skeletonsArray = bySkelDef.skeletons;

        FastArray<SkeletonInstance*>::iterator it = std::lower_bound(
                                                        skeletonsArray.begin(), skeletonsArray.end(),
                                                        skeletonInstance,
                                                        OrderSkeletonInstanceByMemory );

        if( it == skeletonsArray.end() || *it != skeletonInstance )
        {
            DI_WARNING("Trying to remove a SkeletonInstance we don't have. Have you already removed it?");
        }

        DI_DELETE *it;
        skeletonsArray.erase( it );

        //Update the thread starts, they have changed.
        bySkelDef.updateThreadStarts();
    }
}
