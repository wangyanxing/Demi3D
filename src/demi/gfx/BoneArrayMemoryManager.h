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

#ifndef _BoneArrayMemoryManager_H_
#define _BoneArrayMemoryManager_H_

#include "ArrayMemoryManager.h"

namespace Demi
{
 
    /** Implementation to create the Transform variables needed by Bones
    @author
        Matias N. Goldberg
    @version
        1.0
    */
    class DI_GFX_API BoneArrayMemoryManager : public ArrayMemoryManager
    {
    protected:
        /// We overload to set all mParentTransform to point to a dummy matrix
        virtual void slotsRecreated( size_t prevNumSlots );

    public:
        enum MemoryTypes
        {
            Owner = 0,
            Position,
            Orientation,
            Scale,
            ParentMat,
            ParentNode,
            WorldMat,
            FinalMat,
            InheritOrientation,
            InheritScale,
            NumMemoryTypes
        };

        static const size_t ElementsMemSize[NumMemoryTypes];
        static const CleanupRoutines BoneCleanupRoutines[NumMemoryTypes];

        /// @copydoc ArrayMemoryManager::ArrayMemoryManager
        BoneArrayMemoryManager(uint16 depthLevel, size_t hintMaxNodes,
                                size_t cleanupThreshold=100,
                                size_t maxHardLimit=MAX_MEMORY_SLOTS,
                                RebaseListener *rebaseListener=0 );

        /** Requests memory for a new Bone (for the Array vectors & matrices)
            May be also be used for a new Entity, etc.
            @param outTransform
                Out: The transform with filled memory pointers
        */
        void createNewNode( BoneTransform &outTransform );

        /** Releases memory acquired through @see createNewNode
            @remarks
                For optimal results, try to respect LIFO order in the removals
            @param inOutTransform
                Out: Transform to destroy. Pointers are nullified
        */
        void destroyNode( BoneTransform &inOutTransform );

        /** Retrieves a BoneTransform pointing to the first Bone
        @remarks
            @See BoneMemoryManager::getStart
        @param outTransform
            [out] Transform with filled pointers to the first Node in this depth
        @return
            Number of Nodes in this depth level
        */
        size_t getFirstNode( BoneTransform &outTransform );
    };

    /** @} */
    /** @} */
}

#endif
