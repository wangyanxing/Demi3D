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

#include "Demi.h"
#include "DemoFrame.h"
#include "DebugHelper.h"
#include "Bone.h"

struct Trans
{
    DiVec3 pos;
    DiQuat rot;
    DiVec3 scale;
};

/** K2 engine animation testing helper
 */

class K2Anim
{
public:
    
    K2Anim(const DiString& baseDir);
    
    ~K2Anim();
    
public:
    
    void    Load(const DiString& model, const DiString& clip);
    
    void    _LoadBones(const DiString& model);
    
    void    _LoadClips(const DiString& clip);
    
    // bones
    DiVector<Trans> mBones;
    DiVector<DiPair<int,int> > mParents;
    
    // clips
    DiVector<DiVector<Trans> > mClips;
    int mNumFrames;
    
    DiString mBaseDir;
};