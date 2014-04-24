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

#include "ViewerPch.h"
#include "K2ModelView.h"

namespace Demi
{
    K2ModelViewer::K2ModelViewer()
        :mSceneNode(nullptr)
    {
        Init();
    }

    K2ModelViewer::~K2ModelViewer()
    {
        Release();
    }

    void K2ModelViewer::Init()
    {
        DiSceneManager* sm = Driver->GetSceneManager();
        mSceneNode = sm->GetRootNode()->CreateChild();
    }

    void K2ModelViewer::Release()
    {
        mModel.reset();
    }

    void K2ModelViewer::Update(float dt)
    {
        if (mModel)
        {
            mModel->UpdateAnimation(dt);
        }
    }

    void K2ModelViewer::LoadModel(const DiString& file)
    {
        mSceneNode->DetachAllObjects();
        mModel = make_shared<DiK2Model>(file);
        mSceneNode->AttachObject(mModel);
        mSceneNode->SetPosition(0, -50, 0);
        mModel->GetAnimation()->Play(K2PrefabClip::ANIM_IDLE);
    }
}