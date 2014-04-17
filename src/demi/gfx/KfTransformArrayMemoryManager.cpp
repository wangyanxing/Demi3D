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

#include "KfTransformArrayMemoryManager.h"
#include "Math/Array/KfTransform.h"

namespace Demi
{
    const size_t KfTransformArrayMemoryManager::ElementsMemSize[KfTransformArrayMemoryManager::NumMemoryTypes] =
    {
        (3 + 4 + 3) * sizeof( float ),     //ArrayMemoryManager::KfTransformType
    };
    
    KfTransformArrayMemoryManager::KfTransformArrayMemoryManager( uint16 depthLevel, size_t hintMaxNodes,
                                                    size_t cleanupThreshold, size_t maxHardLimit,
                                                    RebaseListener *rebaseListener ) :
            ArrayMemoryManager( ArrayMemoryManager::NodeType, ElementsMemSize, 0,
                                sizeof( ElementsMemSize ) / sizeof( size_t ), depthLevel,
                                hintMaxNodes, cleanupThreshold, maxHardLimit, rebaseListener )
    {
    }
    
    void KfTransformArrayMemoryManager::createNewNode( KfTransform **outTransform )
    {
        const size_t nextSlot = createNewSlot();
        for( size_t i=0; i<ARRAY_PACKED_REALS - 1; ++i )
            createNewSlot();

        const unsigned char nextSlotIdx = nextSlot % ARRAY_PACKED_REALS;
        const size_t nextSlotBase       = nextSlot - nextSlotIdx;

        //Set memory ptrs
        *outTransform = reinterpret_cast<KfTransform*>( mMemoryPools[KfTransformType] +
                                                    nextSlotBase * mElementsMemSizes[KfTransformType] );

        //Set default values
        (*outTransform)->mPosition      = ArrayVector3::ZERO;
        (*outTransform)->mOrientation   = ArrayQuaternion::IDENTITY;
        (*outTransform)->mScale         = ArrayVector3::UNIT_SCALE;
    }
}
