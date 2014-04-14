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

#ifndef __NEON_BooleanMask_H__
#define __NEON_BooleanMask_H__

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

            MASK_Y              = 2,
            MASK_XY             = 3, //MASK_X|MASK_Y

            MASK_Z              = 4,
            MASK_XZ             = 5, //MASK_X|MASK_Z
            MASK_YZ             = 6, //MASK_Y|MASK_Z
            MASK_XYZ            = 7, //MASK_X|MASK_Y|MASK_Z

            MASK_W              = 8,
            MASK_XW             = 9, //MASK_X|MASK_W
            MASK_YW             =10, //MASK_Y|MASK_W
            MASK_XYW            =11, //MASK_X|MASK_Y|MASK_W
            MASK_ZW             =12, //MASK_Z|MASK_W
            MASK_XZW            =13, //MASK_X|MASK_Z|MASK_W
            MASK_YZW            =14, //MASK_Y|MASK_Z|MASK_W
            MASK_XYZW           =15, //MASK_X|MASK_Y|MASK_Z|MASK_W

            NUM_MASKS           =16
        };
    private:
        static const ArrayFloat mMasks[NUM_MASKS];
    public:
        inline static ArrayFloat getMask( bool x, bool y, bool z, bool w );
        inline static ArrayFloat getMask( bool booleans[4] );

        /// Returns true if alls bit in mask0[i] and mask1[i] are set.
        inline static bool allBitsSet( bool mask0[4], bool mask1[4] );

        /** Converts a SIMD mask into a mask that fits in 32-bit number
        @remarks
            @See IS_SET_MASK_X & co. to read the mask, since the result may need
            byteswapping in some architectures (i.e. SSE2)
        */
        inline static uint32 getScalarMask( ArrayFloat mask );

        inline static uint32 getScalarMask( ArrayInt mask );
    };
}

#include "BooleanMask.inl"

#endif
