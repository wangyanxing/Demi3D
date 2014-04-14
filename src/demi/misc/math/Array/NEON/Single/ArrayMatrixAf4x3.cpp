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

#include "MiscPch.h"

#include "Math/Array/ArrayMatrixAf4x3.h"

#if __DEMI_HAVE_NEON

namespace Demi
{
    const ArrayMatrixAf4x3 ArrayMatrixAf4x3::IDENTITY( ArrayMatrixAf4x3::createAllFromMatrix4(
                Matrix4(    1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1 ) ) );

    const SimpleMatrixAf4x3 SimpleMatrixAf4x3::IDENTITY( (ArrayReal) { 0, 0, 0, 1 },
                                                        (ArrayReal) { 0, 0, 1, 0 },
                                                        (ArrayReal) { 0, 1, 0, 0 } );
}

#endif
