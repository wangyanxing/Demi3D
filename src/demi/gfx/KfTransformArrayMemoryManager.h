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
#ifndef _KfTransformArrayMemoryManager_H_
#define _KfTransformArrayMemoryManager_H_

#include "ArrayMemoryManager.h"

namespace Demi
{
    struct KfTransform;

    /** Implementation to create the KfTransform variables needed by SkeletonTrack
        @See SkeletonAnimationDef and @see SkeletonTrack
    @author
        Matias N. Goldberg
    @version
        1.0
    */
    class DI_GFX_API KfTransformArrayMemoryManager : public ArrayMemoryManager
    {
    public:
        enum MemoryTypes
        {
            KfTransformType = 0,
            NumMemoryTypes
        };

        static const size_t ElementsMemSize[NumMemoryTypes];

        /// @copydoc ArrayMemoryManager::ArrayMemoryManager
        KfTransformArrayMemoryManager( uint16 depthLevel, size_t hintMaxNodes,
                                size_t cleanupThreshold=100, size_t maxHardLimit=MAX_MEMORY_SLOTS,
                                RebaseListener *rebaseListener=0 );

        virtual ~KfTransformArrayMemoryManager() {}

        /** Requests memory for a new KfTransofrm (for the Array vectors & matrices)
        @remarks
            Uses all slots.
            Deletion is assumed to take place when the memory manager is destroyed;
            as this manager is run in a controlled environment.
        @param outTransform
            Out: The transform with filled memory pointers
        */
        void createNewNode( KfTransform **outTransform );
    };

    /** @} */
    /** @} */
}

#endif
