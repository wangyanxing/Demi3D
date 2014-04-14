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

#include "Math/Simple/Aabb.h"

namespace Demi
{
    const Aabb Aabb::BOX_INFINITE( DiVec3( 0.0f, 0.0f, 0.0f ),
                                    DiVec3( std::numeric_limits<float>::infinity(),
                                            std::numeric_limits<float>::infinity(),
                                            std::numeric_limits<float>::infinity() ) );
    const Aabb Aabb::BOX_NULL( DiVec3( 0.0f, 0.0f, 0.0f ),
                                    DiVec3( -std::numeric_limits<float>::infinity(),
                                            -std::numeric_limits<float>::infinity(),
                                            -std::numeric_limits<float>::infinity() ) );
    const Aabb Aabb::BOX_ZERO( DiVec3( 0.0f, 0.0f, 0.0f ), DiVec3( 0.0f, 0.0f, 0.0f ) );
}
