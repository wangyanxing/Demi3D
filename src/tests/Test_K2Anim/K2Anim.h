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

struct Clip
{
    Clip(int b = 0)
    :bone(b)
    {
    }
    int bone;
    DiVector<int> vis;
    DiVector<DiVec3> pos;
    DiVector<DiVec3> rot;
    DiVector<DiVec3> scale;

    void resize(int frames)
    {
        if (pos.empty())
            pos.resize(frames, DiVec3::ZERO);
        if (vis.empty())
            vis.resize(frames, 255);
        if (rot.empty())
            rot.resize(frames, DiVec3::ZERO);
        if (scale.empty())
            scale.resize(frames, DiVec3::UNIT_SCALE);
    }

    DiVec3 getPos(int id){
        return id >= pos.size() ? pos.back() : pos[id];
    }
    DiVec3 getRot(int id){
        return id >= rot.size() ? rot.back() : rot[id];
    }
    DiVec3 getScale(int id){
        return id >= scale.size() ? scale.back() : scale[id];
    }
};

/** K2 engine animation testing helper
 */

class K2Anim
{
public:
    
    K2Anim(const DiString& baseDir, DiSceneManager* sm);
    
    ~K2Anim();
    
public:
    
    void    Load(const DiString& model, const DiString& clip);
    
    void    _LoadBones(const DiString& model);
    
    void    _LoadClips(const DiString& clip);

    void    _UpdateBonesHelper();

    void    _UpdateClipsHelper();

    void    _UpdateClips();
    
    // bones
    DiVector<Trans> mBones;
    DiVector<int> mParents;
    DiVector<DiNode*> mBoneNodes;
    DiMap<DiString,int> mNameTable;

    DiMap<DiNode*,int> mBoneIds;
    DiNode* mRootBone;

    // clips
    DiMap<DiString,Clip> mClips;
    int mNumFrames;
    
    DiString mBaseDir;

    DiSceneManager* mSm;

    // skeleton helper
    DiDebugHelperPtr mBonesHelper;

    DiDebugHelperPtr mClipsHelper;
    
    DiVector<DiCullNode*> mBoneVisuals;
};