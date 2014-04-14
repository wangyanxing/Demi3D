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

/// This file is adapted from Ogre 2.0 (working version)

#ifndef __C_BooleanMask_H__
#define __C_BooleanMask_H___

#ifndef __BooleanMask_H__
    #error "Don't include this file directly. include Math/Array/BooleanMask.h"
#endif

namespace Demi
{
    class BooleanMask4
    {
    public:
        enum
        {
            MASK_NONE           = 0,
            MASK_X              = 1,
            NUM_MASKS           = 2
        };
    public:
        inline static ArrayMaskR getMask( bool x );
        inline static ArrayMaskR getMask( bool booleans[1] );

        /// Returns true if alls bit in mask0[i] and mask1[i] are set.
        inline static bool allBitsSet( bool mask0[1], bool mask1[1] );

        /** Converts a SIMD mask into a mask that fits in 32-bit number
        @remarks
            @See IS_SET_MASK_X & co. to read the mask, since the result may need
            byteswapping in some architectures (i.e. SSE2)
        */
        inline static uint32 getScalarMask( ArrayMaskR mask );
    };
}

#include "BooleanMask.inl"

#endif
