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
    struct K2Skeleton
    {
        DiVector<DiString>   names;
        DiVector<DiMat4>     trans;
        DiVector<DiMat4>     invtrans;
        DiVector<int>        parents;
        DiMap<DiString, int> nameMap;
    };
    
    /** K2 animation clip
     */
    struct K2Clip
    {
        DiString name;
        
        struct K2Frame
        {
            DiString bone;
            DiVector<Trans> frames;
        };
        
        void Construct(const DiString& bone, int frames)
        {
            if (boneFrames[bone].frames.empty())
                boneFrames[bone].frames.resize(frames);
        }
        
        DiStrHash<K2Frame> boneFrames;
    };

    /** k2 animation class
     */
    class DEMI_K2_API DiK2Animation
    {
    public:

        DiK2Animation();

        ~DiK2Animation();

    public:

        K2Skeleton        mSkeleton;
        
        DiStrHash<K2Clip> mClips;
    };
}

#endif
