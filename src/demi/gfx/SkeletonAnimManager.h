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

#ifndef _SkeletonAnimManager_H_
#define _SkeletonAnimManager_H_

#include "GfxPrerequisites.h"
#include "BoneMemoryManager.h"
#include "FastArray.h"
#include "IdString.h"

namespace Demi
{
    struct BySkeletonDef
    {
        SkeletonDef const *skeletonDef;
        IdString          skeletonDefName;
        BoneMemoryManager boneMemoryManager;

        /** MUST be sorted by location in its BoneMemoryManager's slot
            (in order to update in parallel without causing race conditions)
            @See threadStarts
        */
        FastArray<SkeletonInstance*>    skeletons;

        /** One per thread (plus one), tells where we should start from in each
            thread. It's not exactly skeletons.size() / mNumWorkerThreads because
            we need to account that instances that share the same memory block.
        */
        FastArray<size_t>               threadStarts;

        BySkeletonDef( const SkeletonDef *skeletonDef, size_t threadCount );

        void initializeMemoryManager(void);

        void updateThreadStarts(void);
        void _updateBoneStartTransforms(void);

        bool operator == ( IdString name ) const { return skeletonDefName == name; }
    };

    /** This is how the Skeleton system works in 2.0:
        There is one BoneMemoryManager per skeleton. That means the same BoneMemoryManager
        manages the allocation of Bone data of all instances based on the same Skeleton
        definition.
        In other words, the skeletal animation system has been greatly optimized for the
        case where there are many instances sharing the same skeleton (regardless of how
        many animations are in use or if some instances use many more animations at the
        same time than others).
    @par
        At the same time, for multithreading purposes we keep track of all pointers we
        create and the variable "threadStarts" records where each thread should begin
        processing.
        Unlike nodes, we cannot just iterate through empty memory because each skeleton
        instance is too complex and somewhat heterogeneous. We also have to ensure
        that (e.g.) if skeletons[0] and skeletons[1] share the same memory block (which
        is granted by the BoneMemoryManager), they get updated in the same thread.
        Otherwise race conditions will ensue, due to SIMD branchless selections inside
        @SkeletonInstance::update.
    @par
        Just like other managers (@see mNodeMemoryManager), SceneManager implementations may
        want to provide more than one SkeletonAnimManager (i.e. one per octant)
    @remarks
        The same BoneMemoryManager can't be used for multiple definitions because otherwise
        many SIMD opportunities are lost due to the heterogeneity of the data (a Skeleton
        may have 3 bones, 10 animations, another may have 6 bones, 2 animations, etc)
        unless we wasted a lot of RAM or perform a lot of book-keeping.
    */
    struct SkeletonAnimManager
    {
        typedef DiList<BySkeletonDef> BySkeletonDefList;
        BySkeletonDefList bySkeletonDefs;

        /// Creates an instance of a skeleton based on the given definition.
        SkeletonInstance* createSkeletonInstance( const SkeletonDef *skeletonDef,
                                                    size_t numWorkerThreads );
        void destroySkeletonInstance( SkeletonInstance *skeletonInstance );
    };

    /** @} */
    /** @} */
}

#include "OgreHeaderSuffix.h"

#endif
