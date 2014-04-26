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

#ifndef DiK2ModelView_h__
#define DiK2ModelView_h__

#include "ViewerPrerequisites.h"

namespace Demi
{
    class K2ModelViewer : public DiBase
    {
    public:

        K2ModelViewer();

        ~K2ModelViewer();

    public:

        void        Init();

        void        Release();

        void        Update(float dt);

        void        LoadModel(const DiString& file);

        bool        SetWireframe(bool var);

        bool        ShowModel(bool var);

        bool        ShowBones(bool var);

        bool        ShowBoneNames(bool var);

        bool        ShowBounds(bool var);

        bool        ShowGrids(bool var);

        bool        SetLoopAnim(bool var);

        DiK2Clip*   GetCurrentClip();

        bool        PauseClip() const { return mPauseClip; }

        void        PauseClip(bool val) { mPauseClip = val; }

        DiVector<DiK2Clip*>   GetClipList();

        void        PlayClip(DiK2Clip* clip);

        int         GetNumFaces();

        int         GetNumVertices();

        int         GetNumSubModels();

        DiK2ModelPtr    GetModel() { return mModel; }

    protected:

        void        InitHelpers();

        void        UpdateSkeletonHelper();

        void        UpdateBoundsHelper();

    protected:

        DiK2ModelPtr mModel;

        DiCullNode* mSceneNode;

        DiDebugHelperPtr mBounds;

        DiDebugHelperPtr mSkeleton;

        DiString  mCurrentFile;

        DiGridPlanePtr mGridPlane;

        bool mPauseClip;
        
        bool mLoopAnimation;

        bool mShowBoneNames;
    };
}

#endif