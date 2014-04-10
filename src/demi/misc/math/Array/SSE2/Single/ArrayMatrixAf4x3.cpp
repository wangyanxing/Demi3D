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

#include "MiscPch.h"

#include "Math/Array/ArrayMatrixAf4x3.h"

#if __DEMI_HAVE_SSE

namespace Demi
{
    const ArrayMatrixAf4x3 ArrayMatrixAf4x3::IDENTITY( ArrayMatrixAf4x3::createAllFromMatrix4(
                DiMat4(    1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1 ) ) );

    const SimpleMatrixAf4x3 SimpleMatrixAf4x3::IDENTITY( _mm_setr_ps( 1, 0, 0, 0 ),
                                                         _mm_setr_ps( 0, 1, 0, 0 ),
                                                         _mm_setr_ps( 0, 0, 1, 0 ) );
}

#endif
