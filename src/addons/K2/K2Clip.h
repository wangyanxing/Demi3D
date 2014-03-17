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

#ifndef DiK2Clip_h__
#define DiK2Clip_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** It is used for binding pose and bone hierarchy
     */
    struct DEMI_K2_API K2Skeleton
    {
        DiVector<DiString>   names;
        DiVector<DiMat4>     trans;
        DiVector<DiMat4>     invtrans;
        DiVector<int>        parents;
        DiMap<DiString, int> nameMap;
    };

    /** k2 animation clips
     */
    class DEMI_K2_API DiK2Animation
    {
    public:

        DiK2Animation();

        ~DiK2Animation();

    public:

        K2Skeleton      mSkeleton;
    };
}

#endif
