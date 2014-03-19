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

#ifndef DiK2Prerequisites_h__
#define DiK2Prerequisites_h__

#include "K2Defines.h"

namespace Demi
{
    class DiK2Model;
    class DiK2Animation;
    class DiK2Clip;
    class DiK2Skeleton;
    class DiK2ModelAsset;
    class DiK2BonesData;

    struct K2KeyFrames;
    struct K2Anim;

    typedef shared_ptr<DiK2BonesData>   DiK2BonesDataPtr;
    typedef shared_ptr<DiK2ModelAsset>  DiK2ModelAssetPtr;
}

#endif
